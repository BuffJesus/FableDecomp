# Gameflow as a converter unit (2026-09-19)

The retail master campaign-stage script (`NScript::CGameflowScript`) registered as the fourth quest-unit
of the native-to-Lua converter and pushed through the whole pipeline. Output:
`refs/script_recovery/lifted/Gameflow/{draft,readable}/FSE/Gameflow/Gameflow.lua`. Nothing here was
copied from Aeon's `LUAGameflow.lua`; it is the oracle the numbers below are measured against.

## The family, from evidence (not the cluster's constructor)

| what | address | evidence |
|---|---|---|
| ctor `CGameflowScript::CGameflowScript` | **0x00CE6CB0** (`lo`) | disassembly: base ctor 0xCB8110 then `mov [esi], 0x12c3fa4`, zeroes +0x4c..+0x60; functions.tsv mislabels it `CAIStateGroup_HoverHornetQueen` |
| Init | 0x00CE6CF0 | vtable slot 3; `DeclareGossipCategories` (ego_r, 2964 B) is inlined into it (retail 2237 B; debug Init is 16 B) |
| RegisterMain | 0x00CE75B0 | vtable slot 1 |
| Main | 0x00CE7670 | vtable slot 2, 31,192 B, 1272 direct calls, 528 GSI vcalls |
| CoreQuestReminder (thread) | 0x00CEF3B0 | `mov [reg+0x34], imm` in Main after the `"CoreQuestReminder"` name push (inventory `threads`); bsim calls it `CQ_HeroSoulsNostroScript::CNostro::Main` |
| CheckBarrowFieldsGuards (thread) | 0x00CEF550 | same, name push `"CheckBarrowFieldsGuards"` |
| OnPersist | 0x00CEF8E0 | vtable slot 4 |
| Alloc | 0x00CEF950 | writes the same vtable |
| dtor | 0x00CEF9A0 | vtable slot 0 |
| `hi` | **0x00CEF9D0** | the `CGameflowAssistanceScript` ctor (stores vtable 0x012C5DE0; `GameflowAssistance.json` Init 0xCEFA00 follows) |

Vtable 0x012C3FA4 = `[CEF9A0, CE75B0, CE7670, CE6CF0, CEF8E0]`, read from the retail image; it agrees
with `refs/script_recovery/native_clusters/Gameflow.json` slot for slot. The cluster's
`constructorAddress` 0x00CB8110 is the base constructor every script cluster carries (bsim propagation:
`GameflowAssistance.json` and `Global_WatchForHeroDeath.json` name the same address), not Gameflow's.
Two thunks sit inside the range (0xCE7640 `CSpawnedFunc::SuspendableProcess`, 0xCE7650 its deleting
dtor); the export carries them and the converter ignores them.

Layout cross-check (`ghidra_out/struct_layouts_egor.tsv` `CGameflowScript`, 128 B): `CoreQuestWaiting`
ulong @92, `SavedScriptNames` vector @96, `SavedCardDefNames` vector @112. Retail OnPersist reads +0x48,
+0x4c, +0x58: the usual `-0x14` quest delta, minus one 4-byte iterator-debug pointer per container.
`PostSavePosition` is `*(this + 0x44) + 4` = `CQ_SunnyvaleMasterData::PostSavePosition` (@4, the
`EGameflowPosition` enum), i.e. master data, exactly as the parity doc says.

## Commands (in order, all from the repo root, `PYTHONPATH=.`)

```
# 1. registry: tools/script_recovery/script_units.py  'gameflow' (lo 0xCE6CB0, hi 0xCEF9D0, scripts ['Gameflow'])
python tools/script_recovery/export_guild_training.py --unit gameflow          # read-only headless ExportScriptTranslationUnit, 1 pass, 11 fns
work/pdb_locals_20260913/pdb-locals.exe "C:/Program Files/dotnet/sdk/10.0.301/TestHostNetFramework/x86/msdia140.dll" debug_build/Ego_r.pdb "*CGameflowScript*" > refs/script_recovery/gameflow/pdb/Ego_r-pdb-locals.tsv   # (then CRLF -> LF)
python tools/script_recovery/quest_unit_evidence.py --unit gameflow             # 7 quest functions, 0 unmatched helpers, 1 quest field
python tools/script_recovery/ghidra_typing_spec.py --unit gameflow --out refs/script_recovery/gameflow/typing_spec.json
python tools/script_recovery/infer_helper_prototypes.py --unit gameflow         # 12 inferred
D:/Subuwu/tools/ghidra-public/support/analyzeHeadless.bat D:/Documents/FableTLC/ghidra_proj FableTLC -process Fable.exe -readOnly -noanalysis -scriptPath D:/Documents/FableTLC/tools/ghidra_scripts -postScript ExportTypedTranslationUnit.java 0x00CE6CB0 0x00CEF9D0 refs/script_recovery/gameflow/translation_unit_typed.json refs/script_recovery/gameflow/define_addresses.txt refs/script_recovery/gameflow/typing_spec.json
python tools/script_recovery/convert_quest_unit.py --unit gameflow
python tools/script_recovery/build_readable_unit.py --unit gameflow
python tools/script_recovery/smoke_run_unit.py --unit gameflow --stage draft
python tools/script_recovery/smoke_run_unit.py --unit gameflow --stage readable
```

Logs: `work/gameflow_export/export_pass_1.log`, `work/gameflow_export/typed_pass_1.log`. The typed export
wrote 611 call-site overrides (0 by-value thing sites: Gameflow never copies a CScriptThing by value).

## Numbers

| | value |
|---|---:|
| exported functions (range) | 11 (7 quest functions + ctor, Alloc, 2 thunks) |
| converted functions | 5/5 (Main, Init, OnPersist, CoreQuestReminder, CheckBarrowFieldsGuards); `functionSyntaxPassed` 5, file syntax 1/1 |
| draft todo (CONVERSION_REPORT) | **179 -> 55** over the night (53 of the 55 are `label`/`goto` bookkeeping rows on Main) |
| draft `-- TODO(native)` lines | 27 -> 8 |
| readable | 1886 lines (draft 3516), syntax 2/2, `-- TODO(native)` 7, `goto`/label 25 per 1000 lines, `scratchValue` 19 per 1000 (Guild 104 / 70, TraderConflict 108 / 157) |
| smoke (draft / readable) | **0 / 0 problems**; every method the file calls exists in the sidecar DLL sources |
| `PostSavePosition` stage writes | **all 35 values present** in draft and readable (`0,100,...,2800`, the parity doc's set); 87 write sites (block copies), 2 reads |
| OnPersist | **all four fields** carried: `PostSavePosition` (master, `PersistTransferInt`), `CoreQuestWaiting` (`PersistTransferUInt`, the FSE-typed 0x4106F0), `SavedScriptNames` / `SavedCardDefNames` as `TODO(native)` (no list binding) |
| Init | writes `PostSavePosition = 0`, `CoreQuestWaiting = 0`, then the 40 `AddRumourCategory` calls that Aeon's `DeclareGossipCategories` lists |
| Oakvale gate | byte-identical after every converter change (5 runs) |
| targeted tests | `test_native_structured_switch.py`, `test_readable_style.py`, `test_source_hygiene.py`, `test_lift_native_lua.py`, `test_guild_woods_melee_converter.py`: 103 passed |

**Stage dispatch, measured** (`tools/script_recovery/probe_gameflow_resume.py`: run the readable `Main` under a lupa mock with
`GetMasterGameState("PostSavePosition")` = each of the 35 values, polls succeed after one frame, record the
`SetMasterGameState("PostSavePosition", N)` sequence). 32 of 35 stages resume at themselves; from 450
onward the whole chain runs in order to 2800 and then sits in the free-roam frame loop. Broken:

1. **arriving at 450 from stage <= 400 stops after 450** (`goto FLOW_native_label_2_c16` residue: the 450 -> 500
   handoff inside the copied tail; from 450 itself the handoff works).
2. **resume at 700 returns** (`if switch == 700 then return end -- TODO goto FLOW_native_label_3`: the 700 block
   is out-of-line inside the `!= 600` if-tree branch; the case is a cross-switch jump).
3. **resume at 1050 returns** (same shape, `FLOW_native_label_5`).
4. **resume at 2800 returns** instead of looping frames (native: `bVar9 = stage == 0xaf0; goto LAB_00cef014` jumps
   into the free-roam `do {} while (bVar9)` condition).
5. Orchard raid Evil/Good completion poll at 450: `if MsgOnQuestCompleted("Q_OrchardFarmRaidGood") then return end
   -- TODO goto LAB_00ce94b6_c16` (should join the Evil branch, not return).

All five are the cross-switch `goto` residue (resume order item 3 in HANDOFF); the intra-switch
fall-through is solved (below). A human should look at these five first; the rest of Main reads like Aeon's.

## Generic converter changes (all gated on Oakvale byte-identity)

* `script_units.py`: the `gameflow` entry (comments carry the boundary evidence).
* `quest_unit_evidence.py`: per-script `package` strips `Q_` only when present (`Gameflow` was `meflow`);
  `ulong` is an Int kind (`CoreQuestWaiting`, and the three `ulong` master fields every unit skipped).
* `native_evidence_lowering.py`:
  * master-data store/load accept a single-digit decimal member offset (`+ 4` = `PostSavePosition`): 179 -> 101 todo.
  * `fold_local_string_vectors`: a stack `std::vector<CCharString>` (push_back 0x44BFF0, dtor 0x414EA0, both
    disassembly-verified) filled with literals becomes a Lua sequence: `local list = {}; table.insert(list, "Q_X")`;
    the 25 `ActivateMultipleQuestsWithoutLoadingResources` operands (FSE takes a `sol::table`) lift. 101 -> 76.
  * `KEYED_LOGBOOK`: 0xCBE960 -> `quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_305")`, 0xCBE9EE ->
    `quest:AddLogbookTutorialEntry("TEXT_QST_LOG_GUILD_BOASTS")` (FSE `AddLogbookStoryEntryString_Func` /
    `AddLogbookTutorialEntry_Func`, `__thiscall(const CCharString*)`); `AddLogbookStoryEntry(iVar8)` with a
    branch-chosen id.
  * `fold_counted_map_stores`: the credits actor map (`operator[]` + out-of-line `CCountedPointer::operator=`
    0x8AB1E0 at node+8, no-op 0x99A3B0 `mov eax,ecx; ret 4`, movie base ctor 0x6E7A60) -> `resources:SetActor(map, "Hero", res)`.
* `convert_quest_unit.py`:
  * `lift_persist_evidence`: OnPersist from the lowered decompile with the transfer kind from the callee
    address (typing-spec `CPersistContext_Transfer_<k>_API`), else the member's PDB kind, else the bsim
    template; a master-data member goes through `Get/SetMasterGameState`; a `vector<CCharString>` member is
    reported. The old `lift_persist` remains the fallback (it is what the Oakvale path uses).
  * `repair_literal_receiver_labels`: re-pairs `__at` suffixes crossed by the address-order fallback of
    `disambiguate_call_labels` using the literal each site constructs into ECX (capstone, 48 bytes back).
* `native_structured_switch.py`: a switch with fall-through lowers to a straight line of guarded blocks in
  case order (`sel = NEXT` at a fall-through, a sentinel for `default`), inside the single-iteration loop;
  this removed 9 of the 13 `goto FLOW_case_*` residues and is what makes the stage chain run 450 -> 2800.
  Only Gameflow has fall-through switches (checked: no `FLOW_case_` in the other units' drafts).
* `lift_native_lua.py`: a re-assigned `native_arg_switch_*` selector is a mutable local (never
  constant-propagated); the `int|string` logbook overload accepts a number-kind local.
* `readable_style.py` `fold_guard_wrappers`: a dedented body ending in `return` followed by a statement gets
  `do return end` (the readable file failed to parse at 1883 before this).
* `report_readable_style.py`: Gameflow added to the measured units.

## Gaps / blockers

* **Sidecar binding missing**: `quest:PersistTransferStringList(context, name, table)` for the two
  `vector<CCharString>` members (row added to `docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md`). Retail `Main`
  never reads them, so the save section stays parity-incomplete but the script runs.
* **Cross-switch goto residue** (5 sites above): resume at 700 / 1050 / 2800 and the 450 -> 500 handoff from
  early stages. This is the `native_goto_scopes.duplicate_sibling_tails` frontier, not a Gameflow-specific
  fact: the compiler's binary-search dispatch puts stage blocks out of line inside other branches.
* `CoreQuestReminder` / `CheckBarrowFieldsGuards` are `quest:CreateThread(...)` from Main like the other
  units (the same FSE thread convention); they run clean in the smoke harness.
* The typed export prints `SetText(0x78)`-style integer immediates for the `__fastcall(int)` logbook helper
  and `(receiver, unaff_EDI)` for the `__thiscall` ones; both spellings are handled, the crossing of their
  labels was the real bug.
* `bsimHelperNames` in the readable report is empty: Gameflow has no helper functions (every non-lifecycle
  body is a thread).

## Using it in the playtest bundle

`build_unit_playtest_package.py` already builds identity-preserving entries from a unit's
`CONVERSION_REPORT.json` (`nativeName = unit.script` = `"Gameflow"`, `file = "Gameflow/Gameflow"`), so
staging `--unit gameflow` produces the retail override entry directly. Do NOT also pass Aeon's
`LUAGameflow.lua` (`gameflow_entry`): `check_override` rejects duplicate `nativeName`s. Not done tonight: the
bundle/Aeon zip were not rebuilt and nothing was installed or launched.

## Note on the concurrent tree

While this ran, another process regenerated `refs/script_recovery/lifted/GuildTraining/draft` (10:33) with
the converter as patched above on disk; nothing here touched that unit, but its regenerated files carry
the generic changes (the `ulong` master fields, the persist lifter fallback, the switch chain, which no
Guild function exercises). Re-check its numbers before trusting a diff against the night-7 baseline.

# 2026-09-19 (afternoon, agent): the stage chain is one switch again

Resume order item 3 done: the five cross-switch `goto` residues were one problem, not five. VC7.1 lowers the
sparse fall-through `switch (PostSavePosition)` to a binary search over the selector whose leaves are jump
tables; Ghidra renders that as nested `if (iVar8 < 0x3e9)` / `if (iVar8 != 1000)` ladders with four
`switch (iVar8)` inside, the 35 stage bodies scattered across the branches (B150/B200/B300/B400 out of line
at the end of the function, B2800 five `if (!terminating)` levels deep inside B2600) and linked by `goto`s
Lua cannot take. Copying tails block by block (`duplicate_sibling_tails`) was never going to converge.

## Generic changes (all gated on Oakvale byte-identity; nothing hand-edited)

* **`tools/script_recovery/native_switch_tree.py` (new) — switch-tree flattening.** Runs in `Lifter.lift`
  right before `lower_nonfallthrough_switches`, unit-converter path only (`accessor_kinds`). From evidence:
  the selector is the scrutinee of >= 2 `switch` statements; a dispatch node is an `if` comparing the selector
  with a constant, or a `switch` on it, not preceded in its block by a re-assignment of the selector (the
  `iVar8 = GetMasterGameState("JackBossBattleResult"); if (iVar8 == 1)` body test stays a body test); every
  value named by a case label or a comparison (and a representative of each gap) is walked through the tree
  symbolically to its entry statement; from each entry the top-level flow (falling out of dispatch branches,
  following unconditional gotos) is walked to the next entry or a `return`, which gives each body and its
  fall-through successor. When the successors form one chain covering every top-level statement, the
  function becomes `prefix; switch (sel) { case V1: body ... default: return; }` and the existing chain
  lowering does the rest. Any surprise (dispatch node inside a body, backward jump, uncovered statement,
  an out-of-range value that runs a body) rejects the rewrite and records a `todo`. Evidence:
  `Lifter.switch_tree_evidence` (`chain`, `values`, `loopConditionEntries`, `trailingIfsInverted`).
  Two Ghidra idioms are normalised first, only when a selector was found:
  * `bVar9 = iVar8 == 0xaf0; goto LAB_00cef014;` where the label is the last line of
    `do { frame } while (bVar9)` — a jump INTO the loop test — becomes `if (iVar8 == 0xaf0) goto HEAD; goto AFTER;`
    with `HEAD:` / `AFTER:` around the loop (the dead `bVar9 =` store is dropped when the loop body writes it
    first). That is how the 2800 free-roam stage becomes a body of its own.
  * a trailing `if (C) { REST }` whose block exit is `return` becomes `if (!C) { return; } REST`, recursively:
    the early-return ladder the compiler emitted and Ghidra nested (five levels in B2600). This also reads
    like Aeon (`if quest:IsActiveThreadTerminating() then return end`).
* **`native_structured_switch.py`** fall-through chain: a `goto` nested inside block W whose target is the
  head label of a later block V is a forward hop along the chain: `sel = V; goto FLOW_chain_next_W;` with
  the label right after block W (the guards in between stay closed). B450's Orchard completion join
  (`goto switchD_00ce928d_caseD_1f4` from inside its `do {} while (true)`) no longer copies the 500 tail.
* **`convert_quest_unit.py` `lift_persist_evidence`**: a `vector<CCharString>` member transfer emits
  `local savedScriptNames = quest:PersistTransferStringList(context, "SavedScriptNames", {})` (binding
  pending; row in `docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md` updated with the contract). Other vector
  member types keep the TODO.
* **`smoke_run_unit.py`**: `PENDING_BINDINGS = {'PersistTransferStringList'}` — reported per function as
  `pendingMethods` and printed, not counted as a problem; remove the name when the sidecar binds it.
* **`native_evidence_lowering.py` `canonicalise_stack_objects`** (the regression the coordinator flagged,
  bisected to the pre-constructor "own slot" rule, not to the switch work): a slot assigned before the
  constructor line keeps its own name EXCEPT when every such assignment is `*(T **)(src + k)` with `k` equal to
  the slot's offset inside the object — a member-wise element copy (`local_8 = *(p0 + 4); local_4 = *(p0 + 8)`,
  Artefact.OnPredicateFail) is the object being built. Orchard readable `nil:IsEqualTo(me)` -> back to
  `quest:GetStateListAt("CrateList", p0):IsEqualTo(me)`; CheckFriendlyAttacks unchanged (Guild 1/1 held).
* `lift_native_lua.py`: `SWITCH_DUMP=<role> SWITCH_DUMP_FILE=<path>` dumps the statements before switch
  lowering (like `GOTO_DUMP`); `NO_SWITCH_TREE=1` bypasses the flattening (bisect aid).
* `probe_gameflow_resume.py`: the last stage writes nothing in retail (the free-roam frame loop runs until
  the thread terminates) — that is `LOOPS-OK`, not `NO-RESUME`.
* `test_native_switch_tree.py` (new, 4 tests): a synthetic tree with the three shapes (jump-table leaf, loop-test
  entry, early-return ladder), the re-used-register body test, and the chain hop.

## Numbers, before -> after

| | before | after |
|---|---:|---:|
| resume probe (35 stages) | 32 resume, chain runs 450 -> 2800; 700 / 1050 / 2800 return; 0..400 stop after 450 | **34 CHAIN-OK + 2800 LOOPS-OK = 35/35**; from 0 the whole chain runs 0 -> 2800 |
| readable Main shape | 4 switch chains + copied tails + if-tree residue | one `repeat ... until false` chain of 35 `if switch == N then ... end` blocks, `default` returns (Aeon's `STAGE_HANDLERS` order) |
| readable `Gameflow.lua` lines | 1886 | **1091** (draft 3516 -> 1879) |
| readable `-- TODO(native)` | 7 | **0** |
| readable `goto` / `::label::` | 57 lines (25 per 1000) | **10** (9 per 1000): B400's two-exit poll (`LAB_00ce933c`), the 450 -> 500 hop, two loop `continue`s, CheckBarrowFieldsGuards' predicate join |
| draft `todo` (CONVERSION_REPORT) | 55 | **5** |
| `report_goto_residue.py` total / Gameflow | 136 / 5 | **131 / 0** |
| readable style | scratchValue 19 per 1000 | 27 per 1000 (fewer lines, same 30 sites) |

## Gates (final tool state, every unit regenerated)

* Oakvale: `convert_new_oakvale.py --out <scratch>` byte-identical to `refs/script_recovery/lifted/NewOakValeIntro`
  (only "Only in refs" lines) — checked after each converter change (4 runs).
* smoke: Gameflow draft 0 / readable 0 (OnPersist prints the pending binding); Guild 1 / 1; **Orchard 0 / 0**
  (was 0 / 1 after the stack-object rule); Trader 2 / 2 (baseline 8).
* `python -m unittest` test_guild_woods_melee_converter, test_new_oakvale_conversion,
  test_native_position_distance, test_native_switch_tree, test_native_structured_switch, test_readable_style,
  test_source_hygiene, test_lift_native_lua: all pass.
* Not done: bundle / Aeon zip not rebuilt, nothing installed or launched, nothing committed.

## What remains

* Sidecar `PersistTransferStringList` binding (contract in FSE_UPSTREAM_REQUIREMENTS.md); then drop it from
  `PENDING_BINDINGS`.
* The 450 -> 500 hop reads `switch = 500; goto FLOW_chain_next_3` inside `repeat ... until false`; a `break`
  would do (the block already ends with `switch = 500`) — a readable-style fold, not a converter gap.
* Readable-style residue: `scratchValue` (30 sites), `predicateResult` (18), the B400 two-exit poll, the
  CheckBarrowFieldsGuards predicate join (`pr = not Evil and not Good`).
* The flattening only triggers on >= 2 `switch (sel)`; a single scattered switch (one jump table plus a
  comparison ladder) would still go through tail duplication. No such function in the four units today.
