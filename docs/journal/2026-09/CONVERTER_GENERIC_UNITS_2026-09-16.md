# Converter generalised to quest units; Guild audited, pivot to Orchard Farm — 2026-09-16

## Decisions
- User: "Finish guild training with the converter, then we'll ship both." Then Aeon (Discord) reported
  Guild training almost hand-ported, proposed a split: **we take Orchard Farm**. User: "pivot to orchard farm".
- The converter must be used and improved, not bypassed by hand-porting (user re-confirmed mid-session).

## Aeon port audit (new tool)
`tools/script_recovery/audit_port_against_pdb.py` — compares a hand-ported package against the debug PDB
class (entities/threads/helpers present?) and the string literals the native class references (from
`ego_r.exe`). Reports in `work/aeon_port_audit/`. WaspBoss: structurally complete, leads =
`TEXT_AI_GOSSIP_WASPBOSS_KILLED`, actor keys `VICTIM`/`VILL1-3`, `$HEROTITLE`/`ToothHobbe`.
GuardianSisterInfo(2): complete (Main + `MazeAtTavern`), lead = `M_MazeExit`. Method can't see wrong
branches/timers — that needs a TU export + API-sequence diff.

## Generic converter pipeline (all new, quest-agnostic)
1. `script_units.py` — registry {evidence dir, address range, IR glob, scripts, PDB pattern, package}.
   Units: `guild_training`, `orchard_farm` (0xDCC040–0xDD2700; `Q_OrchardFarm_Barricade` is a
   resource-only section quest, no class).
2. `export_guild_training.py --unit X` / `guild_training_inventory.py --unit X` — parametrised
   (register-sourced factory stores; factory→constructor vtable follow). Guild output byte-identical.
3. `quest_unit_evidence.py --unit X` → `refs/script_recovery/<unit>/units/<Q>.json`:
   - PDB (`Ego_r`) function names; helpers matched to retail by string sets against `ego_r.exe`;
     entity binding → class by string match (`GuardTeamSpawn`→`CTeamSpawn`); entity helpers
     (`GoOnPatrol`…) classified bsim/strings/size-rank (WEAK flagged).
   - **Retail offsets = PDB − 0x14 (quest classes) − 4 per STL container member before the field**
     (debug STL carries an iterator-debugging pointer). Entities: delta 0. `CScriptThing` is 12 in both.
     Verified against retail `Init` writes for Guild (11/11) and Orchard (HeroTeam 0x78, bools 0x88–0x8b).
   - Struct arrays expanded (`Teams_1_MemberCount`@0xe4), `CScriptThing` members → `thingFields`,
     pointer members, array descriptors (base/stride/members/pointers), master-data layout
     (`CQ_SunnyvaleMasterData`, delta 0, matches FSE `MasterQuest.h`).
4. `convert_quest_unit.py --unit X` — Oakvale `Lifter` path without per-function hooks; entity
   helpers lifted into the entity file; quest helpers called from entities via `native_quest_helpers`.
   `sol::this_state` stripped from manifest arity.
5. `native_evidence_lowering.py` — pre-`annotate` source pass: master data ↔ `Get/SetMasterGameState`,
   Thing members ↔ `quest:GetStateThing/SetStateThing`, dynamic struct-array index ↔ keyed state
   (`"Teams_" .. tostring(i) .. "_MemberCount"`), pointer-into-array (`MyTeam`) as index state with
   chained `EnemyTeam`, `vector<CScriptThing>` ↔ `GetStateListCount/At`, inlined counted-pointer
   assign/release folded back to `CScriptThing::operator=`. Ghidra quirks handled: `=\r\n` wraps,
   decimal offsets, `()` on the next line.
6. Lifter change: parent-helper receiver accepts `*(T *)(this+0x14)` (one star). **Oakvale gate:
   regenerated draft byte-identical** (`scratchpad/gate.sh`).

## Numbers
- Guild (kept as a benchmark for Aeon's port): 152 fns, 120→ compile, 3699→3561 diagnostics.
- Orchard Farm: 82 native fns; unit = 12 owners / 58 fns; 32→41 compile, 660→544 diagnostics.

## Runtime bindings the Orchard Lua will need (NoviCompatibility DLL, not yet written)
`quest:GetStateThing/SetStateThing`, `GetStateListCount/GetStateListAt` (+push), `quest:RetailResources()`
(long-lived `resources` handle instead of `WithRetailResources` closures), entity-state `Thing` kind.

## Typed TU export — DONE (first version)
`tools/script_recovery/ghidra_typing_spec.py` (FSE typedefs → `refs/script_recovery/typing/gsi_prototypes.json`:
918 GSI slots, 65 fixed engine helpers, 79 CScriptThing slots) + `tools/ghidra_scripts/ExportTypedTranslationUnit.java`
(read-only in-memory: helper signatures by address, per-call-site `HighFunctionDBUtil.writeOverride` for GSI/thing
vcalls found by register provenance from `this`; Ghidra's `__thiscall` needs `this` as an explicit first param).
Orchard: 218 overrides, `RunCutsceneMacro_Func(&key,&map,0,0,false,true)` now fully argumented. The converter
prefers `translation_unit_typed.json`. New typed shapes handled in the lowering: `(int)this`, wrapped call
heads/args (`join_wrapped_statements`), `&"literal"`, `->field_0x..`, `_bv` placeholders, explicit receiver
arg stripping; `annotate` alias regexes widened to any identifier (typed exports name temporaries after
prototype params). Oakvale gate still identical. Orchard: 36/58 compile, 708 diagnostics (typed input).
Open typed-export lever: by-value `CScriptThing` params (Ghidra emits `in_stack_…` copies / `pThing` aliases).

**Tooling gotcha (cost an hour):** Git Bash heredocs and `python -c` mangle backslashes — `\1` became a literal
`\x01` byte inside regex replacements. Write patch scripts to files (Write tool) and run them; never inline.

## Next
(was) Typed TU export (Ghidra headless, read-only in-memory): per-call-site prototype overrides for the 918
GSI slots from FSE's native typedefs (`GameInterface.h`), real signatures for engine helpers
(CCharString/CScriptThing ctors, `RunCutsceneMacro_Func`, map ops). That removes the arg-less
`RunCutsceneMacro_Func(0,0,0,1)` / `SetIsPushableByHero()` class of diagnostics for every unit.

## Later rounds (same day, after the wip commit)
- Unit-function prototypes from the PDB stack parameters (`ghidra_typing_spec.py --unit`, void for
  lifecycle/threads) + `infer_helper_prototypes.py` (ret-N purge for FSE-untyped engine helpers) → 128 typed
  helpers, 218 call-site overrides.
- Lifter (all gated byte-identical on Oakvale): `RE_LOCAL_ASSIGN` accepts typed-export local names
  (not Ghidra `xVarN`/`Stack_`/`local_`), `RE_BINDING` any var, `RE_THREAD` tolerates the empty
  section-name ctor, `RE_GUI_TRANSFER_ADD` without cast, `RE_PSEUDO_CALL` pass-through for
  `QUEST*/ENTITY*/ACTORMAP_/RESOURCE_` pseudo statements.
- Lowering: `in_stack_` artefact folding, `)`+newline+`;` join, `::` continuation, `*(T **)&x->field`,
  keyed stores as `QUESTSTATE_Set*`, cutscene actor maps (`malloc(0x24)` header / `StdMap_Construct_API`,
  `operator[]` + counted assign or resource `operator=`, `RunCutsceneMacro_Func`, `StdMap_Destroy_API`)
  → `resources:NewActorMap/SetActor/RunMacro/DestroyActorMap` with `local resources = quest:RetailResources()`.
- Orchard draft: 9/9 bindings + 4 threads register like Oakvale; quest Main is readable; 469 diagnostics
  (44 cleanup-label, 42 label/goto, 17 unnamed fields, rest cutscene/resource locals). Known Ghidra
  artefact left: FPO `stack0x…` local names (stable per function).

## Evening rounds (Orchard converter, after the second wip commit)
- Ghidra tracker: callee-saved `pop`s (mid-function epilogues) no longer drop `this`; ESP-delta
  keyed stack slots → 322 call-site overrides (was 218), arg-less vcalls 126→34.
- Resource lifecycle lowered generically: `CScriptGameResourceObjectScriptedThingBase` ctor/dtor
  (0x7E72A0/0x7E74D0, FSE-proven), movie ctor/dtor, inlined vtable-store constructors
  (`PTR_…0127094c/01260ef4/01238c8c`), `StartScriptingEntity(thing,res,prio)` → `resources:TryAcquire`,
  actor maps (both `malloc(0x24)` and `StdMap_Construct_API` forms; Thing and resource values),
  `RunCutsceneMacro_Func` with literal or variable keys, `GetSquaredDistanceBetweenThings`
  (fastcall, float) → `GetDistanceBetweenThings^2`, `&DAT_` string literals (incl. pooled ""),
  global game-data reads → `quest:ReadGlobalGameData(off)`, `me:GetName()` + strcmp team derivation,
  pointer-to-array-member increments, offset-built string temporaries.
- `native_cleanup_regions.py`: cleanup epilogues (release/destroy) hoisted into
  `local function __cleanup_LAB_x()`; early exits call it instead of leaking (was a real bug in
  the early-return lowering). Stack-object slot names canonicalised to the creating base
  (`uStack_a4` → `appuStack_ac`). Shared native classes → one entity file with N bindings.
- Orchard draft: 46 fns (deduped), 33 compile, 191 diagnostics; quest Main / Evil+Good Init /
  Artefact / CrateTeamMember.Init read as retail logic. `refs/script_recovery/orchard_farm/RUNTIME_API_GAPS.md`
  lists the bindings the DLL needs. Oakvale gate identical throughout; converter tests OK.

## Night rounds 31–53 (all 11 Orchard files compile; 193 → 133 diagnostics; Oakvale gate identical throughout)

Ghidra typed export (`ExportTypedTranslationUnit.java`):
- **By-value `CScriptThing` arguments**: `sub esp,0xc; mov ecx,esp; push src; call 0x4ABE90` (copy ctor) before a
  GSI/thing call ⇒ that site's `CScriptThing *` params become 12-byte structs. Recovers the pushed immediates the
  decompiler was misattributing (`SetCombatNearbyBreakOffRange(me, 4.0)`, `SetStealStealableItems(me, true)`).
  Evidence: FSE typedefs for slots 0xcc4/0xcd0 say `CScriptThing *`; the native ABI is by value.
- **Stack keys are entry-relative** (`disp - espDelta`; the old `disp + espDelta` was inconsistent across depths);
  **mid-function epilogues** (`pop/add esp/ret` in early-return blocks) no longer corrupt the depth for later blocks
  (that bug shifted every parameter read after an early return: `speaker` decoded as `comment_to_make`).
- **GSI singleton** `mov ecx,[0x143e8f8]` tagged GSI (GetTimer/SetTimer through the global were untyped).
- **Data-pointer thing calls**: `mov ecx,[obj+off]; mov eax,[ecx]; call [eax+SLOT]` with SLOT ≥ 0x40 in the thing
  table ⇒ thing override (the Data object shares CScriptThing's slot layout — `IsAlive` at 0x12c via Data, verified
  in disassembly of CrateTeamMember::Main / IsThingCarryingCrate).
- **Parameter types from the ego_r signature** in the bsim comment (`class CScriptThing &` → `CScriptThing *`;
  the PDB locals export has no types for references). Thing params are tagged at their entry stack slot.
- Fixed helpers proven by disassembly: `0x99F570` / `0x99F600` = `CCharString operator+` (`__fastcall` dest, a; stack b).

Lowering (`native_evidence_lowering.py`):
- `LIST_At_<name>(idx)` mangled pseudo-call (no comma inside receivers); balanced-paren finishers
  (`_expand_calls`) for `ENGINE_SquaredDistance/StrCmp/Concat/Colour`; `ENGINE_IsDistanceBetweenThingsUnder/Over` by address.
- Saved-vtable temporaries, `X._4_4_` Data-pointer vcalls/validity, `**(int **)(E+off+4)` Data vcalls (GSI bases excluded),
  parameter (and alias) thing vcalls with explicit receiver / hidden result pointer stripped.
- Stack-thing copies out of list elements, tangled inline `CScriptThing::operator=` split across labels/branches,
  hidden-pointer thing returns (`ret_thing`), inline `strncmp` of two literals (constant), counted addref/release
  no-ops (comma and non-comma forms, tangled with labels), trivial vtable-only base ctor/dtor calls dropped,
  `CScriptThing::~CScriptThing` (0x4AA840) dropped for any cast spelling.
- Members: CCharString/CWideString fields and arrays (`SetStateString`, `"FailReasons_" .. idx`), anonymous/named
  enums as Int, pointer-into-array scalar sub-arrays (`Teams[MyTeam].StateCounter[state] += 1`), bare thing-member
  addresses as by-ref arguments, array index forms that are already lowered state loads.
- Stack colours built byte-wise (retail BGRA) → `{R, G, B, A}` tables (scoped to the slot's live range).
- Ghidra print shapes: `*(T *) (`, decimal member offsets, `(undefined1 [4])` casts, `1e+07`, AL-based bool returns.
- Scalar stack locals renamed (`v_stk_14`) so the lifter accepts them; stack CScriptThing handles likewise.

Lifter: nested/mixed short-circuit conditions with several call assignments lowered through a condition tree
(`native_condition_tree.py`, comma < `||` < `&&` precedence) — only when the single-operator recogniser cannot
express the condition (gate-safe). Goto-only regions (constant-false guard + label entered by `goto`) hoisted as
`__region_LAB_x()` + `goto LAB_y` (`native_cleanup_regions.py`); bare-`return` labels resolve to `return`.
`CONVERT_DUMP=<fn>` env prints the lowered C for one function.

Remaining 133 diagnostics: 68 informational (label/goto/cleanup-verify), the rest single shapes (`CCharString__AssignFromWide`
members, `std::map` cutscene actor maps in the cutscene helpers, `CreateCreature`/`EntityFollowThing` missing operands,
`GetName/IsBeingCarriedBy/SetDataString` not FSE bindings — already in the API appendix).
