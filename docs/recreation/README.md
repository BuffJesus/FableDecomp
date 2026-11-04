> Native update October24: concrete schema106 metadata-to-worker gate passes108 processes/18 controls; valid retail-schema resource gate passes256 decoded loads per alignment at1/4/16/2048, retained handles and original getter comparisons. See [current journal](../journal/2026-10/2026-10-24-native-recreation-continuation.md). Native work continues; Lua excluded.

# Native engine recreation

Start with [NEXT_SESSION](../NEXT_SESSION.md) for the active task. The latest journal entry linked there contains the exact evidence and limitations. Work continues on the original-compatible C++ engine; the modern engine is a separate lane.

Native recreation resumed at the user's request on October 15. The
[current continuation](../journal/2026-10/2026-10-15-native-recreation-continuation.md)
records the frame/clock integration runs, bounded post-presentation comparisons,
preserved failures and pending independent reviews. Lua recovery is outside this work.

## Latest loading checkpoint - October 23, 2026

The permanent loading gate passes 512 original/recreated combinations twice, 92 fault controls, 43 exact roots and four near contexts. It covers device priorities, worker writes, resizing, Close and peer callbacks, and reopening native-written archives. ResizeFile now matches the original through ordinary C++. ObjectExists and const UID-find also match the original; real query-state ownership checks are integrated, and native statistics snapshots are integrated, and staged read-state observations are integrated, and Close at each active read phase is integrated, and descriptor-only accounting edge cases are integrated, and recreation continues with bounded EOF/partial-read behavior. Full `gate_all` was not run. See [the current checkpoint](../NEXT_SESSION.md) and [the evidence journal](../journal/2026-10/2026-10-23-native-recreation-continuation.md).

## Where things belong

| Folder | What you will find |
| --- | --- |
| `rebuild/src/compiled/<aa>/<bb>/` | Original-compatible C++ functions, grouped by their retail addresses |
| `rebuild/include/` | Shared declarations and verified object layouts |
| `rebuild/tests/<aa>/<bb>/` | Permanent behavior tests |
| `rebuild/build/<kind>/<aa>/<bb>/<address>/` | Generated objects, executables and logs |
| `rebuild/modern/<subsystem>/` | Readable modern modules built from understood, tested behavior |
| `modern/` | Planned separate modern engine build; see its documented starting conditions |
| `docs/engine/` | Findings about individual engine systems |
| `docs/journal/` | Dated records of changes, results and failed attempts |
| `work/native-recreation/<area>/<address>/<date>/<attempt>/` | New local experiments, grouped by system and function |
| `work/parity_addtext/` | Existing experiments and frozen evidence retained at their recorded paths |

The public source and build layout follows [ARCHITECTURE](../ARCHITECTURE.md) and [WORKSPACE_LAYOUT](../pipeline/WORKSPACE_LAYOUT.md). The local experiment convention above is for new work. An already recorded experiment keeps its original location so earlier results remain reproducible.

## What the recent checks mean

| Piece | What it does | Latest recorded result |
| --- | --- | --- |
| Drawing helpers | Prepare pictures, text, lines and shapes | Individual checks recorded throughout the October 13 journals; some callers still miss the acceptance threshold |
| Frame start/end and clock | Begin drawing, finish it and measure elapsed time | Linked two-frame test passed; [status 49](../journal/2026-10/2026-10-13-native-recreation-live-status-49.md) |
| Screen swap | Ask the display to show the finished frame | Tested separately and in the two-frame test; graphics-device calls use controlled substitutes |
| Screen clear | Clear the drawing surface before the next picture | 1,216,000 comparisons passed; [status 50](../journal/2026-10/2026-10-13-native-recreation-live-status-50.md) |
| Frontend drawing coordinator | Tell the menu, engine and display when to do their work | 589,824 comparisons and independent saved-result reviews passed; [status 54](../journal/2026-10/2026-10-13-native-recreation-live-status-54.md) |
| Virtual screen rectangle | Return the rectangle covering the game's virtual screen | 46,080 comparisons and independent reviews passed; [status 55](../journal/2026-10/2026-10-13-native-recreation-live-status-55.md) |
| Drawing-window setter | Convert a rectangle and tell the display to use it | 7,680 comparisons and independent reviews passed with conversion/display substitutes; [status 56](../journal/2026-10/2026-10-13-native-recreation-live-status-56.md) |
| Connected window conversion | Set a drawing window using the real coordinate converter and dimension getter | 294,912 comparisons and all independent reviews passed; [bedtime handoff](../journal/2026-10/2026-10-13-native-recreation-live-status-59.md) |
| Frontend and drawing-window chain | Run the menu drawing coordinator with the real virtual rectangle, window, conversion and dimensions helpers | 3,145,728 comparisons and independent source/executable/result reviews passed; [October 14 continuation](../journal/2026-10/2026-10-14-native-recreation-continuation.md) |
| Frontend and frame presentation | Connect the coordinator to actual display/core frame-start, frame-end and screen-swap methods | 3,145,728 comparisons, eight omission checks and all independent reviews passed; [October 14 continuation](../journal/2026-10/2026-10-14-native-recreation-continuation.md) |
| Frontend, frame and clock | Run the recreated clock through the real frontend/core calls with controlled Windows timer APIs | 3,145,728 comparisons, nine omission checks, 64 tick/policy pairs and all independent reviews passed; [October 14 continuation](../journal/2026-10/2026-10-14-native-recreation-continuation.md) |
| Frontend and full screen clear | Connect both clear branches, capture restoration and the real format lookup/table to the frontend/frame/clock chain | 6,291,456 comparisons, fourteen omission checks and all independent reviews passed; shader, viewport and geometry services remain substitutes; [October 14 continuation](../journal/2026-10/2026-10-14-native-recreation-continuation.md) |
| Frontend and shader flags | Replace four flag callbacks with six real shader methods, including pixel-version lookup and reference-count release | 6,291,456 comparisons, twenty omission checks and all independent reviews passed; device/destructor and remaining shader/drawing services remain substitutes; [October 14 continuation](../journal/2026-10/2026-10-14-native-recreation-continuation.md) |
| Frontend and shader state/uploads | Connect seven more shader methods, pixel-constant initialization and scratch storage | 7,452,672 integrated/direct-method comparisons, twenty-seven omission checks and all independent reviews passed; remaining layout, vertex-shader application, projection and drawing services stay qualified; [October 14 continuation](../journal/2026-10/2026-10-14-native-recreation-continuation.md) |
| Frontend and vertex-shader application | Connect actual shader/declaration caching, software-mode selection and device callbacks | 7,649,280 integrated/direct comparisons, twenty-eight omission checks and all independent reviews passed; layout, projection, viewport and geometry remain boundaries; [October 14 continuation](../journal/2026-10/2026-10-14-native-recreation-continuation.md) |

These tests do not establish complete recreated game startup or a running world. New work-only results are kept separate from the published coverage numbers until their promotion requirements are met.

The genuine C++ integer viewport candidate passed 3,317,760 native comparisons
using the original routine and controlled device/shader callbacks. Its compiled
code misses the similarity threshold, so it remains unaccepted. Independent
source, executable and saved-result reviews passed; the functional-only evidence
is closed. See the October 14 journal for its limitations.

## A few useful terms

- **Retail:** the original shipped game.
- **Behavior test:** run original code and rebuilt code with the same starting data, then compare results and changes.
- **Test substitute / mock:** a controlled replacement for a helper or graphics device. Both sides receive the same substitute; its own behavior is outside that test's claim.
- **Near match:** rebuilt C++ passes the behavior test and is close enough to the original compiled instructions under the project's rules.
- **Work-only:** evidence is saved locally, with its qualifications, but has not been promoted into canonical coverage.
- **Frozen evidence:** recorded inputs and results whose hashes and paths must remain stable.

## Tidying and refactoring

Keep new experiments together, give files names that describe their role, and keep generated output inside the experiment or build folder. Preserve failed attempts alongside their explanation. A new attempt gets a new directory.

Refactor tested behavior into coherent modules when its types, ownership and important calls are understood. Recheck behavior after code changes. The original-compatible lane keeps the compiler and file boundaries needed for retail comparison; the modern module lane provides the larger, human-facing organization.

## The modern engine goal

Read [MODERNIZATION](../modern/MODERNIZATION.md) for 64-bit, graphics and platform work, and [IN_ENGINE_MODDING_ENVIRONMENT](../engine/IN_ENGINE_MODDING_ENVIRONMENT.md) for mod folders, live editing and in-engine tools. [SCRIPTING_REDESIGN](../engine/SCRIPTING_REDESIGN.md) describes the future runtime contract; this native recreation session is not doing Lua recovery.
