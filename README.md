# S1000D Semantic Interpreter

A C++23 library that parses S1000D defense documentation XMLs and builds typed in-memory C++ object graphs from them.

---

## What it does

S1000D is an international specification for technical documentation used in defense and aerospace programs. Its data modules are XML files, each conforming to one of many defined schemas (descriptive, procedural, fault isolation, crew, wiring data, and others).

This library takes an S1000D XML file as input, identifies its schema from the `xsi:noNamespaceSchemaLocation` attribute, traverses the XML tree, and builds typed C++ model objects for each recognized element — registering each model against its originating XML node so parent-child relationships can be resolved after the fact.

The output is a `ModelsRegistry` — a flat, ownership-managing store of `BaseModel`-derived objects, each mapped back to the pugixml node it was built from.

---

## Architecture

```
CMakeLists.txt
src/
├── main.cpp
└── core/
    ├── interpreter.cpp
    ├── navigators/
    │   ├── node_streamer.cpp     # Schema identification and traversal dispatch
    │   ├── tree_traversor.cpp    # pugixml tree-walker, calls NodeBuilder per element
    │   └── node_builder.cpp      # Looks up and invokes the builder from BUILDERS map
    ├── builders/
    │   ├── dmodule.cpp           # Builder for <dmodule> root element
    |   ├── content.cpp           # Builder for <content> element
    │   └── ... other elements will be implemented ...
    ├── builders_map/
    │   └── builders_map.cpp      # Defines the extern BUILDERS dispatch map
    └── utils/
        ├── s1000d.cpp
        └── generic.cpp
include/
├── models.hpp                    # Model structs: BaseModel, Dmodule, Content, Refs, etc.
├── registries/
│   └── models_registry.hpp       # ModelsRegistry: owns models, maps nodes to models
├── navigators/
│   ├── node_streamer.hpp
│   ├── tree_traversor.hpp
│   └── node_builder.hpp
├── builders/
│   ├── dmodule.hpp
│   ├── content.hpp
|   ├── ... other elements will be implemented ...
│   ├── refs.hpp
│   └── __elements__.hpp          # Aggregates builder includes
├── builders_map/
│   └── builders_map.hpp          # extern declaration of BUILDERS map
└── utils/
    ├── s1000d.hpp
    ├── generic.hpp
    └── static_maps.hpp           # GLYPHS_MAP and other static lookup tables
interface/
└── interpreter.hpp               # Interpreter class — public entry point
deps/
└── pugixml/                      # Vendored XML parsing dependency
```

---

## How it works

### 1. Interpreter

`Interpreter` is the public entry point. It accepts an `std::istream`, reads the XML content as a string, runs a glyph substitution pass (`transform_glyphs` using `GLYPHS_MAP`), parses it into a pugixml document, and calls `interpret()`.

`interpret()` extracts the document root element and passes it to `NodeStreamer`.

### 2. NodeStreamer — schema identification

`NodeStreamer` reads the `xsi:noNamespaceSchemaLocation` attribute on the root element, strips the path and `.xsd` suffix to get the schema name (e.g. `descript.xsd` → `descript`), and looks it up in `SCHEMA_MAP`.

`SCHEMA_MAP` maps 28 S1000D schema names to a `(Schema enum, expected root element name)` pair. Currently `descript` is the only schema with traversal implemented; the other 27 are recognized but log a "not implemented" message.

For a recognized and implemented schema, `NodeStreamer` constructs a `Traversor` on the root node.

### 3. Traversor + NodeBuilder — tree walking

`Traversor` defines an inner `Visitor` struct inheriting from `pugi::xml_tree_walker`. The root node is built explicitly first, then `_node.traverse(visitor)` walks all descendant element nodes.

For each element node, `Visitor::for_each` calls `NodeBuilder::build(node)`.

`NodeBuilder::build` looks up the element name in the `BUILDERS` dispatch map. If found, it invokes the corresponding builder lambda. If not, it logs `"<element> is not implemented!"`.

### 4. BUILDERS dispatch map

`BUILDERS` is an `extern std::unordered_map` keyed on `std::string_view` element names, with `std::function` lambdas as values. Defining it `extern` in the header and in a separate translation unit (`builders_map.cpp`) allows builder implementations to register themselves without creating circular CMake dependencies between the `builders` and `builders_map` targets.

Currently registered: `dmodule`, `content`.

### 5. Builders

Each builder is a class (`_Dmodule`, `_Content`, `_Refs`, ...) that:

- Registers a new model instance into `ModelsRegistry` via `register_model`
- Sets the model's `type` field from the node name
- Iterates over the node's attributes and resolves them via a local `Attrib` enum and `ATTRIBS` map
- Optionally calls `link()` to wire the model to its parent — e.g. `_Content::link()` retrieves the parent node's model from the registry via `get_by_node`, casts it to `Dmodule*`, and sets `dmodule->children.content = current_model`

### 6. ModelsRegistry

`ModelsRegistry` owns all model objects via `std::vector<std::unique_ptr<BaseModel>>`. It maintains a parallel `std::unordered_map<void*, BaseModel*>` keyed on `node.internal_object()` — the raw pugixml internal pointer — enabling O(1) lookup of a model by its originating XML node.

---

## Models

Defined in `include/models.hpp`:

| Struct | Description |
|---|---|
| `BaseModel` | Abstract base with a `type` string and virtual destructor |
| `Dmodule` | Root data module element; holds optional `id` and `DmoduleChildren` |
| `Content` | `<content>` element; holds optional `id` |
| `IdentAndStatusSection` | `<identAndStatusSection>` element |
| `RDF_Description` | `<rdf:Description>` element |
| ... | ... other elements |

`DmoduleChildren` inside `Dmodule` holds raw pointers to child models (`content`, `ident_and_status_section`, `rdf__description`), populated by individual builder `link()` calls after registration.

---

## S1000D schemas recognized

The following 28 schema types are identified by `NodeStreamer`:

`appliccrossreftable`, `brdoc`, `brex`, `checklist`, `comment`, `comrep`, `condcrossreftable`, `container`, `crew`, `ddn`, `descript`, `dml`, `fault`, `frontmatter`, `icnmetadata`, `ipd`, `learning`, `pm`, `prdcrossreftable`, `proced`, `process`, `sb`, `schedul`, `scocontent`, `scromcontentpackage`, `update`, `wrngdata`, `wrngflds`

Traversal is currently implemented for `descript` only. All others are recognized but not yet processed.

---

## Build

**Requirements**
- CMake ≥ 3.20
- C++23-capable compiler (GCC 13+ or Clang 16+)

```bash
git clone https://github.com/vinaykashyaphr/S1000D-Semantic-Interpreter.git
cd S1000D-Semantic-Interpreter
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

`CMAKE_EXPORT_COMPILE_COMMANDS=ON` is set by default — `compile_commands.json` is generated in the build directory for LSP tooling (clangd).

**Compiler flags** (applied to `semantic_interpreter_core`):
```
-Wall -Wextra -Wconversion -Werror
```

---

## Usage

```bash
./semantic_interpreter <input.xml>
```

The executable takes one S1000D XML file, runs the full interpretation pipeline, and prints the `id` and `type` of the root `Dmodule` and its `content` child to stdout.

---

## Status

Active development. Core pipeline — schema identification → tree traversal → builder dispatch → model registry — is complete. Builder and model coverage is currently limited to `dmodule` and `content`; remaining S1000D element builders are in progress.

---

## License

MIT
