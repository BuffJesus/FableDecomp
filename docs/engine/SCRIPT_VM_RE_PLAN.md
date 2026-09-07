# Script VM reverse-engineering plan

*Created 2026-07-19. This answers whether entity-attached scripts, quests,
dialogue, and cutscenes can be decompiled/reversed, and how that relates to the
native script VM.*

## Current evidence

- `script.bin` is readable with `forge defs`: 611 entries total.
- Current retail counts from `script.bin`:
  - `CCutsceneDef`: 595 entries.
  - `CRegionScriptDef`: 14 entries.
  - `CScriptDef`: 2 entries, including `NULLDEF_CScriptDef`.
- Loose TNG placements attach script identity with top-level fields such as
  `ScriptName` and `ScriptData`. BarrowFields examples include `MK_*` marker
  scripts, `CAM_*` camera points, and `CS_*` cutscene-related names.
- The executable contains native `NScript::*` classes and script interface calls.
  The already-decoded chest path is a concrete example:
  `NScript::CGlobal_OpenChestScript::Main` is native C++ behavior, while the
  placed TNG object supplies data such as its definition and contents.
- `RunCutsceneMacro_Func` now decompiles when Ghidra's decompiler payload cap is
  raised. This is likely a major interpreter/executor for cutscene command data.

## Answer

Yes, the entity-attached scripts can be reversed and understood. The VM is the
right long-term layer for this, but it is not the only layer.

There are three linked layers:

1. **Placement binding:** TNG `ScriptName` / `ScriptData` attach script-ish names
   and parameters to concrete world entities.
2. **Compiled script data:** `script.bin` stores `CCutsceneDef`,
   `CRegionScriptDef`, and `CScriptDef` payloads. These are the byte/data
   records FableForge should decode into readable script/cutscene records.
3. **Native executor:** `Fable.exe` contains the native C++ script classes,
   interfaces, and the interpreter/executor logic that reads those records and
   performs engine actions.

So: decomping the VM gives us the semantics. Decoding `script.bin` gives us the
actual shipped script/cutscene content. Indexing TNG gives us where those scripts
are attached in the world. FableForge needs all three.

## Practical plan

### Stage 1 - Script binding index

- Add `forge script refs <game-root>`:
  - scan all TNGs;
  - collect `ScriptName`, `ScriptData`, position, definition, level, UID;
  - group names by prefix (`CS_`, `CAM_`, `MK_`, quest-specific names);
  - cross-link matching `script.bin` entries where names match exactly or by
    known prefix rules.
- GUI use: select an object in the 3D world and see "attached scripts" directly
  in the inspector.

### Stage 2 - `script.bin` payload mapping

- For each of the three definitions, dump payloads with name, length, and hex.
- Start with `CCutsceneDef`, because there are 595 examples and their names are
  descriptive. Differential comparison across similar cutscenes should reveal
  repeated fields and command blocks quickly.
- Use `RunCutsceneMacro_Func` as the executor oracle: find where it reads each
  payload field, name the field, then re-run the decoder.
- Output: JSON/CSV with best-known fields and unknown byte ranges.

### Stage 3 - Native VM/executor decomp

- Identify and label the executor entry points:
  - `RunCutsceneMacro_Func`;
  - `CScriptDef::Transfer` / `CCutsceneDef::Transfer` /
    `CRegionScriptDef::Transfer`;
  - script lookup by name;
  - dispatch functions that call `CGameScriptInterface` methods.
- Build an opcode/command table if the payload contains bytecode-like command
  IDs. If it is instead a serialized object graph, build a field schema and
  record graph.
- Verify every decoded command against at least one shipped cutscene/script and
  one runtime API effect.

### Stage 4 - Authoring

- First authoring target: readable/decompiled cutscene timeline and entity
  script reference browser.
- Then write-back: modify `script.bin` payloads through `forge::bin::save`, with
  semantic round-trip checks like the game.bin writer.
- FSE Lua remains the near-term "make new quests/scripts" path. Native script VM
  decomp is the path to importing and editing shipped quests/cutscenes directly.

## Relationship to 3D editing

The 3D world view should make script attachments visible:

- markers and camera points are selectable scene objects;
- clicking a `CAM_*` object shows camera spline/point data;
- clicking a `CS_*` or quest marker shows linked `script.bin`/FSE/native script
  records;
- moving a placement writes back to TNG using the existing byte-safe TNG mutation
  API;
- later, editing a timeline updates `CCutsceneDef` or emits FSE Lua, depending
  on mode.

This is more intuitive than a raw table because scripts are spatial in TLC:
triggers, markers, camera points, NPCs, doors, chests, and quest regions all live
in the level.

## Current blocker state

Decompiler coverage is 49,081 / 49,082 functions clean. The one remaining
failure is `Main@0x00DECAD0` with `Pcode: Decoding error: Attribute size is not
present`.

Already checked:

- every instruction in the function decodes to Pcode;
- function metadata reset did not fix it;
- 10 direct callee prototypes look sane;
- direct local/parameter type walk found no bad type;
- whole-program data type scan found only one zero-length typedef,
  `/mmsystem.h/TIMECALLBACK`, which is probably unrelated Windows header noise.

Next attempt, if we care about true 100.000% coverage: clone the Ghidra project,
delete/recreate the function and any custom data types reachable from it, then
retry decompile in the clean clone. Do not risk the main DB until the clone proves
the fix.
