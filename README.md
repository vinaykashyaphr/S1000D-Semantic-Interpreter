# S1000D Semantic Interpreter

A C++23 library that parses S1000D defense documentation XMLs and builds a typed, queryable in-memory object graph from them.

---

## What it does

S1000D is an international specification for technical documentation used in defense and aerospace programs. Its data modules are XML files, each conforming to one of many defined schemas (`descript`, `proced`, `fault`, `crew`, `wiring`, and others).

This library takes an S1000D XML file as input, identifies its schema, walks the XML tree, and constructs typed C++ model objects for each recognized element. All models are registered against their originating XML node so parent-child relationships can be resolved in a deferred second pass — avoiding order-of-traversal constraints.

The result is a `ModelsRegistry`: a flat, ownership-managing store of `BaseModel`-derived objects, each O(1) lookupable by its XML node or by XPath.

---

## Architecture

```
CMakeLists.txt
src/
├── main.cpp
└── core/
    ├── interpreter.cpp
    ├── navigators/
    │   ├── node_streamer.cpp       # Schema identification and traversal dispatch
    │   ├── tree_traversor.cpp      # pugixml tree-walker, emits one node at a time
    │   └── node_builder.cpp        # Looks up and invokes the builder from BUILDERS map
    ├── builders/
    │   ├── dmodule.cpp             # <dmodule> root element
    │   ├── content.cpp             # <content>
    │   ├── refs.cpp                # <refs> + Refs::to_json()
    │   ├── dm_ref.cpp              # <dmRef>
    │   └── ...
    ├── builders_map/
    │   └── builders_map.cpp        # Defines the BUILDERS dispatch map
    ├── registries/
    │   └── models_registry.cpp
    └── utils/
        ├── s1000d.cpp
        └── generic.cpp
include/                            # Internal headers (not exposed to consumers)
├── builders/
│   ├── dmodule.hpp
│   ├── content.hpp
│   ├── refs.hpp
│   ├── dm_ref.hpp
│   └── ...
├── builders_map/
│   └── builders_map.hpp
├── navigators/
│   ├── node_streamer.hpp
│   ├── tree_traversor.hpp
│   └── node_builder.hpp
├── registries/
│   └── models_registry.hpp
└── utils/
    ├── s1000d.hpp
    ├── generic.hpp
    └── static_maps.hpp             # GLYPHS_MAP and other static lookup tables
interface/                          # Public API surface (exposed to consumers)
├── interpreter.hpp                 # Interpreter — public entry point
└── models/
    ├── base.hpp                    # BaseModel
    ├── factory.hpp                 # ModelsFactory
    └── defs/
        ├── dmodule.hpp
        ├── content.hpp
        ├── refs.hpp
        ├── dm_ref.hpp
        ├── pm_ref.hpp
        ├── external_pub_ref.hpp
        ├── ident_and_status_section.hpp
        └── rdf__description.hpp
deps/
└── pugixml/                        # Vendored XML parsing dependency
```

---

## How it works

### 1. Interpreter

`Interpreter` is the public entry point. It accepts an `std::istream` and an optional `ModelsFactory*`. It reads the XML content as a string, runs a glyph substitution pass (using `GLYPHS_MAP` to normalize special characters), parses it with pugixml, and hands the document root to `NodeStreamer`.

```cpp
Interpreter interpreter(in_file);

// Or with a factory override:
ModelsFactory factory;
factory.override_with<Content, MyContent>();
Interpreter interpreter(in_file, &factory);
```

After construction, models are queryable by XPath:

```cpp
Dmodule* root = interpreter.get_model<Dmodule>(".");
Content* content = interpreter.get_model<Content>("./content");
```

### 2. NodeStreamer — schema identification

`NodeStreamer` reads the `xsi:noNamespaceSchemaLocation` attribute on the root element, strips the path and `.xsd` suffix to get the schema name (e.g. `descript.xsd` → `descript`), and looks it up in `SCHEMA_MAP`.

`SCHEMA_MAP` maps 28 S1000D schema names to a `(Schema enum, expected root element name)` pair. For a recognized schema with traversal implemented, `NodeStreamer` constructs a `Traversor` on the root node.

### 3. Traversor + NodeBuilder — tree walking

`Traversor` explicitly builds the root element first, then calls `node.traverse(visitor)` to walk all descendant element nodes depth-first. For each node, `NodeBuilder::build` looks up the element name in the `BUILDERS` dispatch map and invokes the corresponding builder lambda.

### 4. BUILDERS dispatch map + self-registration

`BUILDERS` is an `std::unordered_map<std::string_view, std::function<...>>` defined in a dedicated translation unit (`builders_map.cpp`). Each builder file registers itself at static initialization time via `BuilderRegistrar`:

```cpp
static BuilderRegistrar _reg(
    "dmRef",
    [](const pugi::xml_node& n, ModelsRegistry& r, std::string_view s) {
        _DmRef(n, r, s);
    }
);
```

The `builders` library is linked with `WHOLE_ARCHIVE` into the core, which forces all builder TUs to be included and all static `BuilderRegistrar` objects to be initialized — ensuring every builder is registered regardless of whether anything directly references it.

### 5. Builders

Each builder is a class (`_Dmodule`, `_Content`, `_Refs`, `_DmRef`, ...) that:

- Constructs a model instance via `ModelsRegistry::factory().make<Model>()` and registers it with `register_model`, which maps the model to its XML node
- Sets `model->type` from the node name
- Iterates node attributes and resolves them via a local `Attrib` enum + `ATTRIBS` map
- Calls `defer_link()` on the registry to enqueue a lambda that wires the model to its parent — executed in a second pass after the full tree is walked

Deferred linking exists because when a child is built, its parent model may not exist yet. After traversal completes, `resolve_links()` runs all queued lambdas in order.

The builder file also owns the `to_json()` implementation for its model type. This is where the vtable key function lives, which ensures the vtable is always present when the `builders` library is included whole.

### 6. ModelsFactory — runtime model substitution

`ModelsFactory` allows consumers to substitute their own model subclass for any registered model type at runtime:

```cpp
struct MyContent : Content {
    nlohmann::json to_json() const override { ... }
};

factory.override_with<Content, MyContent>();
```

When a builder calls `factory.make<Content>()`, it receives a `MyContent*` instead. The registry stores it as `BaseModel*`; `get_model<Content>()` uses `dynamic_cast` to return the right type. This is the only extension point exposed to consumers — the builder pipeline itself is not user-modifiable.

### 7. ModelsRegistry

`ModelsRegistry` owns all model objects via `std::vector<std::unique_ptr<BaseModel>>`. A parallel `std::unordered_map<void*, BaseModel*>` keyed on `node.internal_object()` (the raw pugixml internal pointer) enables O(1) lookup by originating node. XPath-based lookup is also supported for post-construction queries.

---

## Directory split: `include/` vs `interface/`

The project separates two distinct audiences:

| Directory | Purpose |
|---|---|
| `include/` | Internal headers. Builders, navigators, registries, utils. Not exposed to consumers. |
| `interface/` | Public API. `Interpreter`, `ModelsFactory`, `BaseModel`, and all model `defs/`. These are the only headers consumers include. |

This prevents consumers from depending on builder internals and keeps the public API surface narrow.

---

## Models

| Struct | Description |
|---|---|
| `BaseModel` | Base class with a `type` string, virtual `to_json()`, and virtual destructor |
| `Dmodule` | Root data module; optional `id`, `DmoduleChildren` (content, identAndStatusSection, rdf:Description) |
| `Content` | `<content>`; optional `id`, `ContentChildren` (refs) |
| `Refs` | `<refs>`; vector of `RefsChildren` (dmRef, pmRef, externalPubRef) |
| `DmRef` | `<dmRef>` reference to another data module |
| `PmRef` | `<pmRef>` reference to a publication module |
| `ExternalPubRef` | `<externalPubRef>` reference to an external publication |
| `IdentAndStatusSection` | `<identAndStatusSection>` |
| `RDF_Description` | `<rdf:Description>` |

Children are stored as raw pointers (no ownership — owned by the registry) for single-occurrence children, and as `std::vector<RefsChildren>` for multiple-occurrence children like `Refs`.

---

## S1000D schemas recognized

28 schema types are identified by `NodeStreamer`:

`appliccrossreftable`, `brdoc`, `brex`, `checklist`, `comment`, `comrep`, `condcrossreftable`, `container`, `crew`, `ddn`, `descript`, `dml`, `fault`, `frontmatter`, `icnmetadata`, `ipd`, `learning`, `pm`, `prdcrossreftable`, `proced`, `process`, `sb`, `schedul`, `scocontent`, `scromcontentpackage`, `update`, `wrngdata`, `wrngflds`

Traversal is currently implemented for `descript` only.

---

## Build

**Requirements**
- CMake >= 3.20
- C++23-capable compiler (GCC 13+ or Clang 16+)

```bash
git clone https://github.com/vinaykashyaphr/S1000D-Semantic-Interpreter.git
cd S1000D-Semantic-Interpreter
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

`CMAKE_EXPORT_COMPILE_COMMANDS=ON` is set by default — `compile_commands.json` is generated in the build directory for clangd/LSP tooling.

**Compiler flags** (applied to `semantic_interpreter_core`):
```
-Wall -Wextra -Wconversion -Werror
```

---

## Usage

```bash
./semantic_interpreter <input.xml>
```

The executable takes one S1000D XML file, runs the full interpretation pipeline, and prints the root `Dmodule` as JSON to stdout.

To customize model serialization, subclass the model and register the override with `ModelsFactory` before constructing an `Interpreter` — see `src/main.cpp` for a working example.

---

## Status

Active development. The full pipeline — schema identification → glyph normalization → tree traversal → builder dispatch → deferred linking → model registry — is complete. Builder and model coverage currently includes `dmodule`, `content`, `refs`, and `dmRef`. Remaining S1000D element builders are in progress.

---

## License

MIT