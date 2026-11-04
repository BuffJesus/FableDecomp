# Community feedback: readability, structure, and a working engine

## Source and date

This record summarizes the September 4–8, 2026 Discord discussion supplied by
the project owner on October 24, 2026. Message permalinks were not supplied;
the discussion has not been independently retrieved. Dates follow the supplied
transcript; its timezone is unspecified. This is a summary, not a full chat log.

It is a different discussion from the later Lua-converter readability feedback.

## The concerns

| Concern raised | Practical requirement |
|---|---|
| Directory layout was difficult to navigate. | Provide stable entry points, subsystem navigation, and clear separation between source, generated intake, tests, builds, and historical evidence. |
| Code repeatedly added offsets instead of accessing objects or structs. | Use proven shared class layouts, original member names, and typed field access. A renamed pointer or a padding-only local struct does not resolve the underlying readability problem. |
| Matching assembly did not make the source easy to read or modify. | Treat behavioral correctness, compiler comparison, and maintainability as distinct review concerns. Recover intent, ownership, names, and useful interfaces. |
| Uncontrolled AI generation could create a maintenance burden. | Review generated work, prove its dependencies, preserve failures, and require meaningful behavioral checks before treating it as reconstruction. Output volume alone is not progress. |
| Debug executables and PDBs could make recovery much easier. | Use donor symbols, type layouts, function names, and implementations as evidence instead of rediscovering them from the stripped retail executable. |
| A working full engine matters more than isolated byte parity. | Track integrated loading, rendering, runtime ownership, and gameplay separately from per-function compiler matches. A matching function does not certify an engine. |

In the supplied discussion, name jeff raised the layout, offset-heavy code, and
AI maintainability concerns. AlbionSecrets advocated using `ego_r.exe` or
`FableWin.exe` and their symbols; they also described debug-build differences,
including missing audio-related code. odarenkoas emphasized a working engine
over byte parity. These are the participants' historical observations, not a
fresh technical audit of those executables.

## The project's September response

The September 7 response accepted the readability and layout criticism and
announced a typed-struct pass and layout cleanup. The stated goal was readable,
editable source and eventual mod support.

The follow-up said Ego_r, Ego_d, and FableWin PDBs had already been exported and
used for retail naming and shared class headers. It kept retail `Fable.exe` as
the target because that was the shipped binary and the FSE hook target, while
using debug builds as a source of names and types. This records the rationale
given then; it does not prove every recovered function is now well typed or
readable, nor settle the tradeoff between parity effort and engine integration.

## Where the requirements live now

- [Architecture](../ARCHITECTURE.md#making-functions-human-readable) describes
  evidence-backed names, shared types, field access, behavior tests, and cohesive
  subsystem interfaces. Its explanation of small parity translation units is a
  compiler constraint; it does not remove the need for human navigation.
- [Contributing](../../CONTRIBUTING.md) separates generated candidates from
  verified reconstruction and excludes handwritten assembly bakes from C++
  reconstruction coverage. That assembly rule is related project policy; the
  supplied discussion criticized readability despite matching assembly, rather
  than explicitly alleging assembly bakes.
- [Project layout](../PROJECT_LAYOUT.md) maps source and evidence locations and
  explains safe organization alongside active work.
- [Native status](../NATIVE_STATUS.md) separates bounded local verification,
  published source, authored frontend integration, and a playable engine.
- [Roadmap](../ROADMAP.md) tracks source publication and engine integration work.

These documents capture intended standards. They are not evidence that the
readability, organization, or full-engine concerns have all been resolved.
