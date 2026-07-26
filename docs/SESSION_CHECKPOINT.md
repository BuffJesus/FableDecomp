# Session checkpoint — 2026-07-23 (resume state if usage runs out)

Two tracks in flight: (A) background decomp/byte-match loop, (B) foreground custom quest-card work.
`docs/HANDOFF.md` (automation-maintained) has the canonical decomp resume; this file adds the
in-flight session state + the quest-card task the automation does NOT track.

**Current prioritized queue:** `docs/ACTIVE_TASK_LIST.md`. Its generated address lists are
`rebuild/backlog/active_candidate_queue.tsv`, `fse2_remaining_ranked.tsv`, and
`pending_batch_status.tsv`. Use that queue instead of the older candidate ordering below.

## A. Background decomp — in flight
- **Coverage:** ~1,731 landed src/compiled; ~1,588 audit-confirmed byte-matches (EXACT+RELOC).
  fse1 landed **108 verified ForgeFSE bindings** (commit `8b97302`).
- **Running workflow:** residue cracker `wg7qhm5s6` (`scratchpad/residue_wf.js`) — 8 register-alloc
  DIFFER residues (`0081ee60 0081efc0 0081f090 0081f170 0082e330 0082e3d0 0082e420 00ab4700`).
  On completion: for each self-win, OVERWRITE the landed `rebuild/src/compiled/*<addr>*.cpp` with the
  winning source (these are already in the catalog), then re-run `catalog_parity_audit.py` and commit.
- **Queued next (prepped, ready to fire):** `fse2` — 76 larger ForgeFSE binding methods (97–160B).
  Oracle+targets: `rebuild/oracles/pending/fse2_{oracle.tsv,targets.json}`; bundles+addr list in
  `scratchpad/fse2_{bundles,addr_list.json}`. Launch: `Workflow(scriptPath=scratchpad/author_wf.js,
  args={"batch":"fse2","addrs":[...fse2_addr_list.json...]})`, single lane, then verify_and_land.
- **Loop mechanics / tooling:** all committed in `tools/decomp_pipeline/` — see HANDOFF. One lane at a
  time (avoids rate-limit); watch for monthly-spend-limit failures (landing/exports are local).
- **Downstream exports (mirrored to projects):** `python tools/decomp_pipeline/export_verified_facts.py`
  regenerates + mirrors `verified_functions/struct_schema/forgefse_verified_bindings` into
  FableForge/ForgeFSE/FQT doc dirs.

## B. Custom quest card — the F9 "Active Quests" fix (foreground task)
**Goal:** custom hunt-quest cards that appear in the F9 Active Quests menu (user's mod: NPC quest-giver
with 9 cycling hunt quests; everything works EXCEPT the F9 card).

**Root cause (engine decomp, HIGH confidence — `docs/QUEST_CARD_EMPTY_FIX.md`):** the F9 list
(`ConstructQuestList param_3==1`) renders from the **runtime card THING** (`CTCQuestCard`) in the quest
manager's runtime list (`DAT_013b89fc+0x6c`), NOT from the static def or the guild-list entry. That
THING exists only after the quest is **ACTIVATED**; setters resolve it via
`GetActiveQuestCardFromScriptName` keyed on the **quest script name** (`card+0x28`). Title/summary come
from static def TextIDs (`def+0x30`/`+0x2c`) — **zero/invalid TextID => blank**.

**Required accept-handler flow (script names MUST all match):**
```
AddQuestRegion(region,...) -> AddQuestCard("OBJECT_QUEST_CARD_<n>", questScriptName, false,false)
-> ActivateQuest(questScriptName)  [creates the F9 THING]
-> SetQuestCardObjective/GoldReward/RenownReward(questScriptName, ...)
```
FQT's start-screen generator already implements this exact flow (working reference).

**CQuestCardDef fields** (`ghidra_out/def_field_layouts.json`, `docs/QUEST_CARD_SYSTEM.md`):
QuestName +0x28/QuestSummary +0x2c/QuestObjective +0x30 (TextIDs), RegionName +0x38, InventoryCategory
+0x40, RenownReward +0x44, GoldReward +0x48, RewardObjects +0x4c, IsCoreQuest +0x58.
FSE API: `AddQuestCard`@0x008913F0 (vtable292, takes questCardObjectName), `GiveQuestCardDirectly`
@0x008968C0 (transient, avoid), `SetQuestCardObjective` plus the ForgeFSE wrapper names
`SetQuestGoldReward`/`SetQuestRenownReward` for the reward fields.

**Decision (user, 2026-07-23):** author **DISTINCT card def per quest** (9 hunt quests). Use **donor
text** (clone WASP_MENACE TextIDs) for the first working test → swap in custom text.big entries later.

**DONE (2026-07-23):**
- ✅ **9 distinct hunt cards authored + byte-validated** → `work/quest_card_custom_20260723/`.
  `build/data/CompiledDefs/game.bin` = retail + `OBJECT_QUEST_CARD_HUNT_1..9` (entries 14763–14780),
  cloned from Wasp Menace (valid TextIDs 5289/6894 → non-blank), escalating rewards 200g/50r →
  2000g/550r, IsCoreQuest=1, all decode clean. Tool: `forge quest card <root> <schema> HUNT_N
  --donor OBJECT_QUEST_CARD_WASP_MENACE --in-place --gold G --renown R --core 1` (pass bare `HUNT_N`;
  the tool prepends `OBJECT_QUEST_CARD_`).
- ✅ Companion script `work/quest_card_custom_20260723/companion_hunt_quest.lua` (accept-handler flow).
- ✅ Latent retail donor found: `OBJECT_QUEST_CARD_BOUNTY_HUNT` #3725, `CREATURE_BANDIT_BOUNTYHUNT`
  #1118, `OBJECT_HERO_TITLE_SHADOWHUNTER` #3971 — Fable ships a bounty-hunt scaffold.

**PERSISTENCE SPIKE VERDICT (2026-07-23, decomp — HIGH confidence):** a quest card activated in the
childhood prologue does **NOT** survive the childhood→adult time-skip. The runtime card THING
(`CTCQuestCard`) is transient and destroyed on region/world unload; `AddQuestCard` only inserts into
the runtime guild list (CQuestManager+0x58), which is NOT entity-serialized. **BUT quest SCRIPT_DATA
variables DO persist** when the same ForgeFSE quest implements `OnPersist` and round-trips its
namespaced `SetStateBool` value with `PersistTransferBool`. (`SetStateBool` alone is in-memory;
ForgeFSE exposes no `SetQuestVariable`/`GetQuestVariable` binding.) ⇒ Use **discover-as-kid /
hunt-as-adult**: the mysterious NPC in childhood Oakvale
sets a persistent quest variable; an adult script checks it and does AddQuestCard→Activate→Set*.
Cites: `docs/QUEST_CARD_SYSTEM.md:13,23`, `docs/SAVE_ENTITY_GRAPH.md:256-280`, `FORGETEST_STATE.md:40`.

**Design (user, 2026-07-23):** mysterious NPC flavor; discover-as-kid → hunt-as-adult.

**DONE after the original checkpoint:**
1. ✅ **Mysterious NPC placement package** under
   `work/quest_card_custom_20260723/npc_placement/`. It clones the childhood
   `NOVI_BookTrader` into StartOakValeEast at `M_BarrelManHiddenPos`, offline only. Validation:
   106→107 things, exactly one addition, zero changes/removals/conflicts, base and modified TNGs
   round-trip. The live TNG SHA-256 remains equal to the untouched base.
2. ✅ **Correct persistence bridge:** `SecretHunt.lua` uses quest-local `SetStateBool` plus
   `OnPersist`/`PersistTransferBool`; the NPC entity uses real `TEXT_SECRET_HUNT_NPC_*` dialogue
   keys. No nonexistent SetQuestVariable API remains.
3. ✅ **EgoCore-compatible text writer ported into FableForge:** `forge text set` and atomic
   `forge text import`, plus quest-card `--title-text/--summary-text/--objective-text/--success-text`.
   The writer preserves retail `BIGB`, metadata, type histogram, and untouched payloads. FableForge
   test suite: 7/7 PASS.
4. ✅ **Custom text staged:** `custom_text_manifest.json` contains 36 strings. The staged
   `build/data/lang/English/text.big` retains all 28,913 retail records byte-identically and appends
   IDs 28914–28949. All nine card defs now reference distinct custom title/summary/objective IDs.
5. ✅ **Decomp resumed:** fse2 landed
   `CGameScriptInterface::CameraEarthquakeIntensityAtPos @ 0x0088ED20` as
   `RELOCATION_MATCH + behavior PASS`; fse2 is 1/76 with 75 remaining. Notes and the honest
   0x008997E0 near miss are in `work/decomp_fse2_resume/`. The scheduled
   `FableTLC Auto RE Wave 2` lane was restarted after integration and resumed Wave 3 work.

**Next steps:**
1. Register `SecretHunt` plus `MysteryHunt1..9` in ForgeFSE/QST, then run the documented offline
   deployment smoke. Back up and stage both game.bin and English text.big; do not deploy only one.
2. Confirm the live region strings used by the companion hunt table, especially Graveyard,
   HobbeCave, and Snowspire.
3. Continue fse2 from the 75 remaining staged binding methods, using the VC7.1 byte gate.

Artifacts folder: `work/quest_card_custom_20260723/` (README.md + companion_hunt_quest.lua + build/).

---

## Session addendum — 2026-07-23 (evening)

**Decomp landings:** +2 byte-exact fse2 bindings via a NEW **diff-feedback refine loop**
(`MsgOnHeroCastSpell` 0x00893f80, `IsDeedWitnessed` 0x0089c8a0; both RELOCATION_MATCH + behaviour
PASS). Byte-match count 55→57. Method: author → compile → feed annotated retail-vs-built disasm diff
back to the agent with register-alloc nudging → verify_and_land. This cracked cases the mechanical
`crack_residue.py` sweep and first-pass authoring both missed (0 wins each).

**fse2 status:** 20 of 76 authored (session-limit cut the fan-out at 55 failures); 0 byte-exact on
first pass, 2 landed after the refine round. ~55 methods still un-authored; ~8 close DIFFERs remain
(same-length near-misses, candidates for another refine round). New tool: `verify_residue.py`.

**Residues (the 8 register-alloc DIFFERs):** still DIFFER. Agent re-authoring reached 0 byte-exact;
several are now documented **IRREDUCIBLE** — VC7.1 13.10 deterministically emits a register
allocation retail doesn't (e.g. 0081f090 extra `mov esi,eax`; 008994e0 tail `Apply` this-via-eax).
These are compiler-scheduler artifacts, not source-shape bugs. Honest verdict: not all residues are
crackable from C source under this toolchain.

**Quest cards — track C DONE:** `work/quest_card_custom_20260723/deploy/` registration package
assembled + offline-validated (TNG 106→107 clean; 9 HUNT cards decode clean). Engine finding:
MysteryHunt1..9 need NO separate ForgeFSE registration — `LuaQuestState::ActivateQuest` forwards the
name string to engine ActivateQuest (vtable[276]); one host quest `SecretHunt` drives all nine.
Remaining: in-game smoke (manual, needs live game) + confirm Graveyard/HobbeCave/Snowspire region
strings.

**Major finding — dormant co-op multiplayer subsystem.** Documented in `docs/FINDINGS.md`
(2-source: retail name DB + `ego_r` debug PDB). `LHNetworkLib` sockets + `CNetworkClient`
client/host `CGameEventPackage` replication + `CPlayerManager` multi-controller + combat-capable
`CTCCoopSpirit` (COOP_SPIRIT_PLAYER_ONE..FOUR). Next probe: decompile the gate/protocol cluster
(`IsMultiplayerGameActive` 0x449d20, `InitialiseAsNetworkHost` ~0x4ae940, `GetLocalGameEventPackageSet`
0x4aeaa0, `ProcessEventPackage` 0x416670, `CTCCoopSpirit::Construct` 0x4d55d0) to gauge how gated vs
gutted the path is.
