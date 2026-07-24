# Session checkpoint — 2026-07-23 (resume state if usage runs out)

Two tracks in flight: (A) background decomp/byte-match loop, (B) foreground custom quest-card work.
`docs/HANDOFF.md` (automation-maintained) has the canonical decomp resume; this file adds the
in-flight session state + the quest-card task the automation does NOT track.

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
@0x008968C0 (transient, avoid), `SetQuestCard{Objective,GoldReward,RenownReward}`@0x008918B0/E8/920.

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
variables DO persist** (save `START_SAVED_QUESTS`/`SCRIPT_DATA`, via `Quest:SetQuestVariable` /
`GetQuestVariable`). ⇒ Use **discover-as-kid / hunt-as-adult**: the mysterious NPC in childhood Oakvale
sets a persistent quest variable; an adult script checks it and does AddQuestCard→Activate→Set*.
Cites: `docs/QUEST_CARD_SYSTEM.md:13,23`, `docs/SAVE_ENTITY_GRAPH.md:256-280`, `FORGETEST_STATE.md:40`.

**Design (user, 2026-07-23):** mysterious NPC flavor; discover-as-kid → hunt-as-adult.

**Next steps:**
1. **Mysterious NPC placement** — TNG edit to add a hidden creature/NPC to childhood Oakvale (and/or
   an adult Oakvale spot). Tooling: `forge tng` / SilverChest.TngBridge.
2. Wire the childhood NPC to `Quest:SetQuestVariable("SecretHunt","DiscoveredAsChild",1)`; adult
   quest-giver checks `GetQuestVariable` then runs the companion flow.
3. Deploy test: back up install game.bin, copy `build/.../game.bin`, FSE smoke — confirm a HUNT card
   shows in F9. (No install change until user OKs.)
4. **EgoCore-parity follow-up (custom TEXT):** port EgoCore's def/text compiler so cards get CUSTOM
   title/summary (not donor). EgoCore main-suite `Definitions/CompilerBackend.h` compiles plain-text
   defs; FableForge has `forge/textbig.hpp` decode + `forge text` (read only) — needs a text.big
   WRITER + a source-def compile frontend. This is the "custom cards via EgoCore" capability.

Artifacts folder: `work/quest_card_custom_20260723/` (README.md + companion_hunt_quest.lua + build/).
