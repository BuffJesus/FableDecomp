# Script "VM" map — Fable: The Lost Chapters (retail Fable.exe @ ImageBase 0x400000)

*Created 2026-07-19. RE of the compiled-script execution path. This is the map of
how `script.bin` / cutscene records are executed and how they bridge to
`CGameScriptInterface`. Companion plan: `docs/SCRIPT_VM_RE_PLAN.md`.*

## TL;DR — the headline architectural finding

**TLC has no numeric-opcode bytecode VM for its shipped scripts.** There is no
opcode byte, no jump table indexed by an opcode, no operand-immediate encoding, no
push/pop/load/store instruction set. What "compiled scripts" actually are is a
**list of ASCII text command lines**, each of the form

```
[Actor.]Verb arg1,arg2,arg3,...
```

The executor is a **text-keyword command interpreter**: a very long
`if (strncmp(cmd,"VerbName")==0) { parse string args; call CGameScriptInterface
method } else if (...)` chain. Dispatch is by **string compare on the verb name**,
not by an integer opcode. Arguments are parsed at runtime from their text form
(`GFStringToFloat`, `GFStringToInt`, `IsFalse`) and the handler calls a native
method on the singleton `CGameScriptInterface` through a fixed vtable offset.

This is the answer to "recover the opcode set / dispatch loop / instruction
format": the *opcode set* is the ~184-entry **verb table**, the *dispatch loop*
is the `strncmp` chain in `RunCutsceneMacro_Func`, and the *instruction format* is
`Actor.Verb comma-separated-string-args` (line-oriented text tokens).

## The interpreter / dispatch

| Item | Address | Evidence |
|------|---------|----------|
| `RunCutsceneMacro_Func` (the executor) | **retail `0x00cbfb7d`** | FSE source anchor `FableAPI.cpp:183` type `tRunCutsceneMacro` (2nd source) + Ghidra decomp `ghidra_out/runcutscenemacro_full.c` |
| `CGameScriptInterface` instance ptr | **`DAT_0143e8f8`** (`*DAT_0143e8f8` = its vtable) | 45 dispatch callsites in `RunCutsceneMacro_Func` |

Signature (Ghidra): `RunCutsceneMacro_Func(map<CCharString,C2DVector> *markerPositions,
CRegion *region, uint flags, ... )`. The `map<CCharString,C2DVector>` is the
marker/actor position table the verbs resolve names against; `CRegion*` is the
region the script runs in.

Dispatch shape (verbatim pattern, repeated ~184×):

```c
CCharString::CCharString(local, "UseCamera", -1);
sVar13 = CCharString::GetLength(local);
iVar33 = strncmp(cmdText, "UseCamera", sVar13);
if (iVar33 == 0) {
    /* parse string args from local_44, local_40, ... via GFStringToFloat/Int/IsFalse */
    /* then dispatch to a CGameScriptInterface method: */
    (**(code **)(*DAT_0143e8f8 + 0x670))(...);   // native call bridge
    goto LAB_00cd17fd;                            // = "next command"
}
/* else fall through to next strncmp against the next verb name */
```

There is **no jump table** — Ghidra shows a linear `strncmp` cascade. `LAB_00cd17fd`
is the common "command handled, advance to next command" join point (the loop
back-edge). Control-flow verbs (`GamePause`, `WaitActiveDialog`, `DoScriptFrame`)
are handled *inside* their own branch with real `do/while` loops that pump
`PlayCutsceneSound` and poll `IsCutsceneSkipped()` each frame — pacing is done by
the native handler, not by a VM program counter over a bytecode stream.

Per-verb decompiled slices (arg parsing + exact vtable offset called) are in
`ghidra_out/cutscene_verb_slices.md` for: UseCamera, NoLoadUseCamera,
DoCameraPreloading, GamePause, DoScriptFrame, Create, WaitActiveDialog,
RegisterActor, RemoveAllThings, SetDoorOpen, and more.

## The "opcode" table — the verb set (CONFIRMED)

The full recovered verb list (185 entries) is in
`ghidra_out/cutscene_native_verbs.txt`; per-verb argument signatures in
`ghidra_out/cutscene_verb_args.tsv`; native-present / actually-used-in-shipped-data
status in `ghidra_out/cutscene_verb_census.tsv`.

Two syntactic classes of verb (CONFIRMED from real data, below):

1. **Global verbs** (no actor prefix): `UseCamera`, `GamePause`, `Create`,
   `FadeIn`, `FadeOut`, `DoScriptFrame`, `DoCameraPreloading`, `NoLoadUseCamera`,
   `SetDoorOpen`, `RegisterActor`, `WaitActiveDialog`, `PutUpYourSwords`, ...
2. **Actor-targeted verbs** (`.`-prefixed in the native table, written
   `Actor.Verb` in data): `.WalkTo`, `.RunTo`, `.SneakTo`, `.Teleport`, `.Speak`,
   `.DialogSpeak`, `.PlayAnimation`, `.PlayLoopingAnim`, `.Drawable`,
   `.ClearCommands`, `.LookToThing`, `.EntitySetMaxWalkingSpeed`, ... The actor
   name before the `.` is resolved to a `CScriptThing` by
   `GetCutsceneActorScriptThing` (`0x00cbf9de`): empty/"HERO" → hero
   (`*DAT_0143e8f8 + 0x118`); a registered name → the `RegisterActor` map
   (`FindRegisteredCutsceneActor` `0x00cd2b86`); else find-by-name
   (`*DAT_0143e8f8 + 0x120`).

Argument encoding is **not** fixed-width binary. Each verb reads a fixed *count* of
comma-separated **text** tokens and coerces per-token:
- float:  `GFStringToFloat` / `FUN_0099e690` (`GFCharStringToFloat`, `0x0099e690`)
- int:    `GFStringToInt`
- bool:   `IsFalse` (token == "FALSE"/"0"/empty ⇒ false)
- string / name: used raw (object def name, marker name, actor name, TEXT_ tag)

`cutscene_verb_args.tsv` records the recovered per-verb arg count + signature, e.g.
`.SneakTo` = `arg1:str arg2:str arg3:bool arg4:bool arg5:bool arg6:bool`;
`.PlayLoopingAnim` = `arg1:str arg2:int arg3:bool arg4:bool arg5:bool arg6:bool`;
`.SetPushable` = `arg1:bool`. These are CONFIRMED by matching the decompiled
`GFStringTo*`/`IsFalse` parse order in each branch.

### Native-call bridge (CONFIRMED)

Every verb that does engine work calls `(**(code **)(*DAT_0143e8f8 + OFF))(...)`.
`DAT_0143e8f8` = pointer to the singleton `CGameScriptInterface`;
`*DAT_0143e8f8` = its vtable (the same 891-method interface catalogued in
`ghidra_out/gamescriptinterface_catalog.tsv`, whose method bodies live at
`0x0088xxxx–0x008axxxx`). Distinct offsets seen in `RunCutsceneMacro_Func`:

```
0x1c  0x118(GetHero) 0x120(FindThingByName) 0x150 0x194 0x208 0x2f4 0x314 0x318
0x3bc 0x1f4(500) 0x5c0 0x5d0 0x5d4 0x5d8 0x5dc 0x5e0 0x618 0x61c 0x620 0x670
0x680 0x684 0x800 0x870
```

`+0x118` and `+0x120` are confirmed as GetHero / find-thing-by-name by
`GetCutsceneActorScriptThing`. The remaining offsets are the per-verb engine
actions (camera, fade, create, door, etc.); mapping each offset → catalog method
name is straightforward follow-up (the catalog gives every method's own body
address; the vtable at instance-ptr `*DAT_0143e8f8` orders them by these offsets).

> Note vs. HANDOFF chest path: HANDOFF cites the `CGameScriptInterface` **vtable**
> at base `0x1260F0C` (e.g. `+0x6D4` GetNumberOfKeysNeededToUnlockChest). Here
> `DAT_0143e8f8` is the **instance pointer**; `*DAT_0143e8f8` dereferences to that
> same vtable region. Same interface, two ways of naming the anchor.

## Compiled-script container layout (`script.bin` / CompiledDefs)

`script.bin` (611 def entries) holds three script-bearing def types:
`CCutsceneDef` (595), `CRegionScriptDef` (14), `CScriptDef` (2, incl.
`NULLDEF_CScriptDef`). A `CCutsceneDef` payload is a **named list of command
strings**. Validated live with `forge script cutscene <root> CS_ARENA_ROUND_1`:

```
entry 15: CS_ARENA_ROUND_1 (2310 bytes, 69 commands)
0000  UseCamera        UseCamera P_ARENACUOC,-1,NULL,0,0
0002  Create           Create CREATURE_BS_VILLAGER_MALE, P_CROWD1, Crowd1
0006  Create           Create CREATURE_HORNET_01, P_ENEMYPOS1, Wasp1, false, ArenaEnemy
0007  DoScriptFrame    DoScriptFrame 1
0018  Wasp1.Drawable   Wasp1.Drawable FALSE
0029  Hero.Teleport    Hero.Teleport P_ARENAHEROSTART
0033  Hero.SneakTo     Hero.SneakTo P_ARENAHEROEND, 0.0, false, false, false
0036  Hero.DialogSpeak Hero.DialogSpeak HERO,$HEROTITLE,true
0037  GamePause        GamePause 3.5
0038  SetDoorOpen      SetDoorOpen ArenaHeroGate, true
0046  WaitActiveDialog WaitActiveDialog
```

So a compiled cutscene record = header (name + count/size) + a sequence of
newline/record-delimited **command strings**. Each command string is exactly the
text the `strncmp` dispatch matches against, args and all. There is no separate
constant pool and no compiled bytecode stream — the "constants" are inline in each
command's text (object def names, marker names, `TEXT_*` dialogue tags, `$VAR`
substitutions like `$HEROTITLE`). Cross-scene verb histogram (proof of a stable
command grammar over all 595 scenes) is in
`ghidra_out/cutscene_verb_census.tsv` and via `forge script command-stats`:
`GamePause`=2982, `UseCamera`=1713, `DoScriptFrame`=762, `FadeOut`=684, ...

`CScriptDef` (only 2 entries, mostly the NULL template) and `CRegionScriptDef`
(14) are the same family; region scripts run through the same `RunCutsceneMacro_Func`
(its signature takes a `CRegion*`), i.e. cutscenes and region scripts share one
text-command executor.

## VALIDATION — real instructions disassembled with the recovered map

The proof required by the plan. Taking real bytes from shipped `CS_ARENA_ROUND_1`
and "disassembling" them with the recovered verb map, then confirming each maps to
a decompiled dispatch branch and forms sane control flow ending sanely:

| # | Raw command (from script.bin) | Verb (matched branch) | Decoded semantics |
|---|-------------------------------|-----------------------|-------------------|
| 0000 | `UseCamera P_ARENACUOC,-1,NULL,0,0` | `UseCamera` @ dispatch xref 00cc9f39 | switch to camera marker `P_ARENACUOC`, no blend |
| 0002 | `Create CREATURE_BS_VILLAGER_MALE, P_CROWD1, Crowd1` | `Create` @ 00ccc246 | spawn creature def at marker `P_CROWD1`, register as actor `Crowd1` |
| 0007 | `DoScriptFrame 1` | `DoScriptFrame` @ 00cc7085 | advance/pump 1 script frame (the frame-pacing op) |
| 0033 | `Hero.SneakTo P_ARENAHEROEND, 0.0, false×3` | actor `.SneakTo` @ 00cc0cba | move Hero (actor-resolved via +0x118) to marker, speed 0.0, flags false |
| 0037 | `GamePause 3.5` | `GamePause` @ 00cc88d1 | busy-wait 3.5s pumping sound, breakable by `IsCutsceneSkipped()` |
| 0046 | `WaitActiveDialog` | `WaitActiveDialog` @ 00cc656b | block until active dialog finishes (loop on `+0x5c0`) |

All 69 commands of the scene decode cleanly under the map, in order, ending with
fade/return commands — sane, terminating control flow. The verb strings in the
shipped data are byte-identical to the string literals inside the decompiled
dispatch branches (`ghidra_out/runcutscene_string_literals.txt`). That two-way
match — data string ⇄ decompiled `strncmp` literal ⇄ live forge decode — is the
confirmation the map is correct.

## Script-modding feasibility verdict

**Highly feasible, and easier than a bytecode VM would be.** Because compiled
scripts are line-oriented **text commands** (not packed bytecode with offsets/jump
targets), editing is edit-in-place of ASCII with no relocation math:

- **Read:** already done — `forge script cutscene/cutscenes/command-stats` decode
  all 595 cutscenes to readable command lists.
- **Author/edit:** a new command is just a new text line using a known verb +
  its arg signature (`cutscene_verb_args.tsv`). Grammar is fully enumerated
  (185 verbs). Risk is low: unknown verbs simply fall through the `strncmp` chain
  and are ignored (no crash), so additive edits are safe.
- **Write-back:** needs `forge` `CCutsceneDef` payload re-serialization (header
  count/size + the command-string blob) with the existing byte-safe def writer;
  this is a string-blob rewrite, the easiest def class to round-trip.
- **Semantics are known:** each verb's engine effect = the `CGameScriptInterface`
  method it calls; that interface is the 891-method catalog already reversed.

Bottom line: shipped quests/cutscenes/region scripts can be decompiled, edited,
and re-emitted as text; the FSE-Lua path remains for brand-new logic, but direct
editing of shipped `script.bin` content is unblocked by this map.

## What stays open (follow-ups, not blockers)

1. **Full offset→method table.** Map each of the ~24 distinct
   `*DAT_0143e8f8 + OFF` vtable offsets in `RunCutsceneMacro_Func` to its named
   catalog method (mechanical: index the vtable at the instance ptr against
   `gamescriptinterface_catalog.tsv`). Only `+0x118`/`+0x120` are name-confirmed here.
2. **Exact `CCutsceneDef` byte header.** `forge` already parses it; record the
   precise header field layout (name offset, command count, per-command length
   prefix vs delimiter) in the def schema for a clean write-back path.
3. **`$VAR` / `TEXT_*` substitution.** Confirm where `$HEROTITLE`-style tokens and
   `TEXT_*`/`'…'` dialogue tags are resolved (likely a pre-pass or inside
   `DialogSpeak`/`Speak` handlers) — needed for editing dialogue lines.
4. **`CRegionScriptDef` (14) / `CScriptDef` (2) payload shape.** Confirmed to
   share the executor; dump their payloads to verify identical text-command
   layout vs. a thin wrapper.

## Evidence index (all under `ghidra_out/`)

- `runcutscenemacro_full.c` — full decomp of `RunCutsceneMacro_Func` @ 0x00cbfb7d (FSE-anchored).
- `cutscene_verb_slices.md` — per-verb decompiled dispatch slices + arg parsing + vtable offset.
- `cutscene_native_verbs.txt` (185 verbs), `cutscene_verb_args.tsv` (arg signatures),
  `cutscene_verb_census.tsv` (native/used status).
- `runcutscene_string_literals.txt` — verb string literals inside the dispatch (data⇄code match).
- `labels_cutscene_helpers.tsv` — `GetCutsceneActorScriptThing` 0x00cbf9de,
  `FindRegisteredCutsceneActor` 0x00cd2b86, `GFCharStringToFloat` 0x0099e690.
- `gamescriptinterface_catalog.tsv` — all 891 `CGameScriptInterface` methods + addresses.
- `scriptvm_decomp.c` — 1010 script-related functions decompiled (prior pass).
- Live: `forge script cutscene "<root>" CS_ARENA_ROUND_1` and `forge script command-stats "<root>"`.

*No new headless Ghidra pass was run: the Ghidra project was locked by another
single-writer agent (java.exe holding `ghidra_proj/FableTLC`), and the prior
decomp artifacts above already contained the interpreter, verb table, and
dispatch slices. Validation was done with read-only `forge.exe` against the retail
install at `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters`.*
