Rating: 6.5 / 10

The layering idea is solid for a project at this stage, but there are several structural issues that will create pain as more elements are added.

What's good

Clean separation: Interpreter → NodeStreamer → Traversor → NodeBuilder → _Builder is a well-defined pipeline
ModelsRegistry as a central ownership store (with unique_ptr) is the right ownership model
Separating the builder (_Dmodule) from the model (Dmodule) is a good pattern — construction logic doesn't pollute the data structure
Improvement areas

1. Manual registry in builders_map.cpp is a maintenance trap (high impact)
Every new element requires two manual steps: add to __elements__.hpp and add to BUILDERS. Missing either causes a silent "not implemented" skip — exactly what caused your refs issue. Consider a self-registration pattern or at minimum a static assert that catches the mismatch.

2. Children structs inherit BaseModel unnecessarily
ContentChildren, DmoduleChildren, RefsChildren all inherit BaseModel but they aren't models — they're just groupings. This gives them a type field they never use and adds virtual dispatch overhead for no reason. They should be plain structs.

3. Raw pointer children with no lifetime guarantees
Content* content, Refs* refs, etc. are raw pointers into the registry. There's no compile-time or runtime enforcement that they're set before to_json() is called. You're relying on XML parse order matching link order, which is fragile. Even a simple assert(content != nullptr) before dereferencing would surface bugs earlier.

4. to_json() is called twice in the empty-check pattern


if (!children.to_json().empty()) j["children"] = children.to_json();
You've already fixed this in the latest version with auto children_json = ..., but the older pattern is still present in some places.

5. Two separate include directories (include/, interface/)
interface/ and include/ serve overlapping purposes. Typically one directory holds public headers. The split between interface/definitions/models.hpp and include/builders/ is inconsistent — a consumer of this library doesn't need builder headers but does need models.

6. No tests
With a schema as large as S1000D, element builders will silently produce wrong output without test coverage against known XML fixtures.