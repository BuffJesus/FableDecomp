# HANDOFF — resume here

*Last updated: 2026-07-23 16:xx MDT (local deterministic byte-match lane + SDK exports online).*

## Current authoritative resume point (2026-07-23 late) - verified coverage + local tiny lane

Current audited buildable-source coverage:
- `rebuild/src/compiled/`: 1,623 landed source files.
- `rebuild/compile-gate/parity_audit.tsv`: 1,588 true byte-matches (`EXACT`/`RELOC`), 34 known
  `DIFFER` behavior-tier functions, 1 `NO_ORACLE` orphan.
- SDK exports are current:
  - `rebuild/sdk/verified_functions.json`: 1,588 byte-verified functions for downstream tooling.
  - `rebuild/sdk/struct_schema.json`: 225 reconstructed class schemas (134 with class size facts).
  - `rebuild/sdk/forgefse_verified_bindings.json`: 73 ForgeFSE binding-queue targets verified.

New local deterministic lane:
- `tools/decomp_pipeline/auto_author_tiny.py` authors trivial tiny functions without an LLM and
  still lands only through `verify_and_land.py`.
- Supported patterns include `ret`, `ret 4`, constant returns, `return self` fastcall helpers,
  `return self & imm8`, and tiny `*self = imm32` stores. No inline/naked asm is used.
- This locally landed +9 from batch14/15, +39 more fastcall helper shapes from batch14, +69 from
  batch16, and +5 from batch17 after the earlier batch14 +76 and SDK export commits.

Pending production batches:
- `batch15`: staged; 108 unlanded candidates remain after local tiny draining.
- `batch16`: staged; 205 unlanded candidates remain; bundles and UTF-8 address list exist under
  the session scratchpad.
- `batch17`: staged; 125 unlanded candidates remain; bundles/address list exist under scratchpad.
- Run the ultracode authoring workflow on these remaining candidates, then wrap output as
  `{"result":{"authored":[...]}}` and land with `verify_and_land.py <wrapped> <batch_oracle> --land`.

Selector/oracle improvements:
- `next_batch.py` now splits merged oracle rows at `ret -> prologue` and also `ret -> ret/retN`,
  preventing fused tiny functions from becoming unwinnable over-length rows.
- Continue using one authoring lane at a time unless rate limits are clearly stable.

*Last updated: 2026-07-23 08:xx MDT (self-sustaining byte-match promotion loop online).*

## Current authoritative resume point (2026-07-23) — self-sustaining byte-match loop

The buildable-decomp promotion lane is now a self-refilling cycle. Candidate supply is
effectively unlimited (32,687 untried high-yield functions in the manifest); the bottleneck is
authoring throughput, not candidates.

**This session landed 142 byte-identical promotions** (12 pilot + 130 batch3-rest), taking
`rebuild/src/compiled/` from 131 → 261 files (roughly tripling the prior byte-identical count of
55). batch4 (123 promotion-queue accessors) and batch5 (130 manifest-sweep accessors) are
authoring in parallel at handoff; land them on completion.

**The loop (one round):**
1. `python tools/decomp_pipeline/next_batch.py batchN <count> [max_len=96]` — selects the next N
   untried high-yield accessors (complete proto, known CC, accessor-ish return), mints their retail
   oracle **straight from Fable.exe (Ghidra-free**, next-address length heuristic + ends-in-ret
   confidence filter), writes `rebuild/oracles/pending/batchN_oracle.tsv` + `_targets.json`, prints
   the address list. **No Ghidra window / no lock contention** — safe to run anytime.
2. `python tools/decomp_pipeline/gen_bundles.py <oracle> <targets> <bundles_dir> <manifest.json> 96`
   — disasm bundles for the authoring fleet.
3. Launch the authoring workflow (scratchpad `author_wf.js`): ultracode `Workflow`, one agent per
   candidate, each authors `source_cpp`/`test_cpp`/`pass_pattern` and self-verifies with
   `check_one.py` (iterates to a byte-match). Args: `{"batch":"batchN","addrs":[...]}`.
4. `python tools/decomp_pipeline/verify_and_land.py <wf_output.json> <oracle> --land` — re-verifies
   every candidate independently under VC7.1 (agent self-reports cannot cause a false land), writes
   `src/compiled` + `tests` + catalog + oracle for exact/relocation MATCH + behavior PASS wins.
   Wrap the workflow return as `{"result":{"authored":[...]}}` before feeding it.
5. `git add rebuild/src/compiled rebuild/tests rebuild/build_candidates.ps1
   rebuild/oracles/auto-re-candidates.tsv && git commit`. Keep ~2 lanes in flight; each workflow
   completion is the trigger to land+commit+refill+relaunch.

**Durable tooling added this session:** `tools/decomp_pipeline/next_batch.py` (selector+oracle
minter), `tools/decomp_pipeline/check_one.py` (per-agent single-candidate scorer, isolated workdir
so parallel `cl.exe` don't collide). `pe_oracle.py` self-validates 103/130 (79%) exact vs known
rows; every miss is *over*-long (shared epilogue / SEH tail), never wrong content — so a stray row
can only waste authoring effort, never mis-land.

**Yield notes:** 3–48 byte accessors/stubs win at ~76–100%; larger bodies with engine calls are
behavioral reimplementations that byte-match rarely (correctly deprioritized). `pe_oracle` over-long
rows and equal-length register-allocation/scheduler artifacts (see `docs/PARITY_TRIAGE.md`) are the
two non-winnable classes — not source-reachable, do not regrind.

## Current authoritative resume point (2026-07-22)

- Read `docs/FULL_DECOMP.md` and generated `rebuild/COVERAGE.md` before the older session history
  below. The current catalog is 49,553 functions: 100% mechanically named, 99.913% usable
  reconstruction/navigation names, 77.656% known calling conventions, and 69.023% complete
  prototypes. These database metrics are not buildable-source coverage.
- Buildable reconstruction remains below 1%: 94 VC7.1-compiled and behavior-tested candidates
  (0.190%), 71 verified functional-or-matching functions (0.143%), and 51 raw byte-identical
  functions (0.103%). The first 1% compile+behavior milestone is 496, leaving 402 promotions.
- `CNavQuadTree::InitialiseLines @ 0x00A7A5E0` is the newest curated promotion. Its focused
  grow/shrink/zero oracle passes and all 102 instruction bytes match retail after masking the six
  expected COFF relocation fields.
- The first ForgeFSE binding batch completed 16/16 structural PASS. The generated binding queue is
  462 unique target addresses: 16 agent-reviewed, 446 still needing reversal, 167 direct-signature,
  269 adapter, and 26 signature/arity-review rows. Binding approval intentionally remains zero.
- The recurring task `FableTLC Auto RE Wave 2` actually launches the Wave 3 runner every 15 minutes;
  its next refill is automatic, uses `IgnoreNew`, and writes durable state under `lift/state/`.
  `FableTLC Rebuild Refresh` runs on the same cadence and defers while the queue owns Ghidra.
- The ForgeFSE wrappers at `0x0088EDB0` and `0x0088EDA0` are now raw retail matches. Next manual
  wrapper target: `CancelRadialBlurFade @ 0x00890180`. Next deeper nav batch:
  `IsAreaBlockedByLines @ 0x00A76F30`, then shared declarations for `UpdateLines`, `Initialise`, and
  `CNavQuadTreeNode::Initialise`.
- Focused quest-card/terrain smoke artifacts are under
  `work/runtime_smoke_quest_terrain_20260722/`. The game install has been restored: original
  `FableScriptExtender.dll`, `game.bin`, `names.bin`, and `FSE_Master.lua` are back, and
  `FSE/map_resource_alias.ini` is absent.
- Runtime control now proves ForgeFSE can register/run a quest card when the card asset exists:
  `OBJECT_QUEST_CARD_WASP_MENACE` displayed as Wasp Menace with 1337 gold, 500 renown, card art,
  and Guildmaster "new quest available" audio. The failure is the custom/repurposed card asset or
  text/definition binding, not the base quest API.
- Do not combine active quest-card tests with terrain teleport tests. The active quest triggered the
  engine's "leave a Quest region" abandon/reload modal during the ForgeTest teleport flow, which can
  block or confuse manual teleport validation even when `GoToMapSlot` returns `ok=true`.
- Terrain status: whole-LEV aliasing is disproven as a fix. Run 3 replaced black ground with a
  white/empty donor-space hole because donor bounds/bank state came across. Run 4 opened native
  `Data\Levels\FinalAlbion\ForgeTest.lev` at bank entry 426 and captured stronger foreground
  render/layer telemetry, but it was contaminated by the active quest flow. Next terrain run should
  disable quest activation and compare native ForgeTest foreground layer material/texture handles to
  a healthy retail patch.
- Current Claude Code decomp pointer: review
  `lift/reports/wave3/code/0x0089B330_global_IsHeroNaked_CGameScriptInterface_UBE_NXZ.cpp`.
  It names `CGameScriptInterface::IsHeroNaked`, but its current reconstruction dereferences
  `pEntry->m_pInterface` even after assigning the map end sentinel on a failed `LowerBound`. Treat
  that as a correctness hazard before promotion; likely target lane is a small ForgeFSE wrapper/API
  batch after `CancelRadialBlurFade`.

## Status
Active reverse-engineering pipeline completed the **Phase 1 FableWin donor expansion**. After
conservative FSE labels, `Fable.exe` is now **35,479 / 44,269 functions named (80.14%)**. The watcher
finished at 14:11 and is no longer running.

**★ End goal (user directive 2026-07-17): FULL decompilation** — a complete, buildable C/C++ source
reconstruction (`docs/FULL_DECOMP.md`), not just a modding surface. Modding surface is a dividend.
Bar #1 (100% Ghidra pseudo-C coverage) is the near-term milestone; bar #2 (functional buildable
source, module-by-module via `auto-re-agent` + VC7.1) is the long horizon.

## Verified this session
- `Fable.exe` = clean **PE32, x86-32, ImageBase `0x400000`**, no packer/DRM. Ghidra-ready as-is.
- FSE is installed & running in the game; its **931-function API manifest** is copied to `refs/`
  and converts cleanly to `tools/fse_import/fse_api.h` + `fse_api_index.md` (Entity API 88, Quest API 843).
- FSE **source is local** at `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\`.
- **FQT** (`D:\Code\FQT`) is the user's quest editor over FSE — a downstream consumer.
- Ghidra RE script suite (30) + Lua tooling copied from Fable2RE into `tools/`.
- **EgoCore** (by FSE's author) is the completed data-format RE + our headless extraction engine at
  `C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\`. **Run headless only** (`--list`/
  `--batch`/`--extract-textures`/`--extract-audio`); bare launch = ImGui GUI trap that hangs the shell.
  It is the answer key for decompiling all asset/resource/level/def/audio loaders. See `docs/ECOSYSTEM.md`.

## Phase 0 — DONE (2026-07-17)
Both programs imported into project `ghidra_proj/FableTLC` (Ghidra **12.1** @ `D:\Subuwu\tools\ghidra-public`):
- `Fable.exe` (shipping TLC, blind): 44,268 fns, 1,730 named (3.9%). Baseline target.
- `ego_r.exe` (debug, +`Ego_r.pdb`): 65,565 fns, **28,057 PDB-named**, 215,509 symbols. Donor DB.
- Numbers + reading in `ghidra_out/coverage_baseline.md`. Scripts: `SetPdbEgoR.java`, `DumpStats.java`.

## ★★★ Phase 1 — BSim name port — DONE (2026-07-17). `Fable.exe` named 3.9% → **78.4%** (34,714 fns).
+24,923 names from `ego_r` (PDB) via BSim @ sim≥0.7, then **+7,961 more @ sim≥0.5** (precision audited,
`bsim_port_audit_05final.tsv`), then **DemangleAll.java: 32,689 demangled, 0 failures** → readable
`CBar::Foo` + class namespaces + signatures (symbols 120,764 → 195,822). Full story + pipeline in
`ghidra_out/coverage_baseline.md`; runbook `docs/BSIM_PORT.md`.
Key gotcha: Ghidra's PDB analyzer applies types but NOT function symbols → had to extract them via
`pdb.exe` (DIA `msdia140.dll` registered) → XML → `parse_pdb_xml.py` → `ApplyNames.java` on ego_r first.

## Session 2026-07-18 — donor upgrade + FableWin 2nd donor (IN FLIGHT)
**Done this session:**
- **ego_r donor upgraded:** `CreateMissingFunctions.java` created 5,946 of the 6,541 missing PDB fns
  (5,224 named; log `create_missing_egor.log`); `ApplyNames.java` now sanitizes whitespace in PDB
  names ("dynamic initializer for …") → +1,291 more (log `apply_pdb_egor2.log`). Donor now ~42,900
  named fns (was 36,400).
- **FableWin.pdb extracted:** pdb.exe → `ghidra_out/fablewin_pdb.xml` (1.5 GB) →
  `ghidra_out/fablewin_pdb_names.tsv` = **164,501 names**, base 0x400000 (verified PE header).
  FableWin includes the level editor (`CEdit*` classes) — richest donor by far.
- **FableWin.exe import + auto-analysis COMPLETE:** `analyze_fablewin.log` reports analysis succeeded
  and save/import succeeded at 13:49. The slow `X86FunctionPurgeAnalyzer` phase took ~13,297 seconds
  but finished cleanly.
- **Resume continuation:** fresh BSim DB `bsim/fabletlc2` created. `CreateMissingFunctions.java` and
  `BSimApplyNames.java` now sanitize whitespace in imported names before `setName`; BSim comments are
  donor-neutral.
- **FableWin donor named:** `ApplyNames.java` on `fablewin_pdb_names.tsv` applied 157,042 names,
  `CreateMissingFunctions.java` created/named 38 missed functions, and the second apply skipped
  164,499 already-named functions with only 2 failures.
- **BSim pipeline advanced:** upgraded `ego_r` signatures generated and committed. Current watcher
  step as of 13:53 is `Generate FableWin BSim signatures` (`ghidra_out/bsim_gensigs_fablewin.log`).
  This is expected to be the long step because FableWin has ~165k functions.
- **Name import hardening:** `ApplyNames.java`, `CreateMissingFunctions.java`, and `BSimApplyNames.java`
  now keep demangle-friendly names intact on the first attempt, but fall back to
  `SymbolUtilities.replaceInvalidChars(...)` if Ghidra rejects a symbol. `tools/AuditNameTsv.ps1`
  audited `fablewin_pdb_names.tsv`: 164,501 rows, 164,501 unique addresses, 0 bad addresses, 2,775
  whitespace-containing names.
- **Report helpers added:** `tools/StatusFablePipeline.ps1`, `tools/AuditNameTsv.ps1`,
  `tools/SummarizeBsimAudit.ps1`, `tools/SummarizeStatsLog.ps1`, and
  `tools/WriteFablePipelineReport.ps1` make status, audit, and final coverage reporting one-command
  after the watcher advances.
- **FSE source mined:** `tools/ExtractFseAddresses.ps1` extracts literal `ASLR<...>(0x...)` engine
  addresses from local FSE source. Current output: `refs/fse_engine_addresses.tsv` = 47 addresses
  (functions + data/vtables), `ghidra_out/labels_fse_addresses.tsv` = 38 conservative function
  labels, audited clean. Apply later with `LabelApply.java` after the BSim pipeline settles.
- **Editor/tooling survey:** `tools/SummarizeFableWinEditorSymbols.ps1` and
  `tools/WriteEditorToolingNotes.ps1` generated `ghidra_out/fablewin_editor_symbols.*` and
  `docs/EDITOR_TOOLING_NOTES.md`. Key finding: FableWin contains Lionhead editor code
  (`CEditComponent`, `CEditWorldMap`, `CEditControlCentre`, `CEditLevelMerger`, `ParseTngFile`,
  `LoadLevFile`, region graph/navmesh/mapwho, compiled defs). `docs/TOOLING_INTEGRATION_MATRIX.md`
  maps this against ChocolateBox, SilverChest/WadBridge, FQT, and EgoCore.
- **FQT/editor bridge work:** `docs/FQT_EDITOR_BRIDGE_PLAN.md` records how FableWin editor symbols
  relate to FQT, ChocolateBox, SilverChest/WadBridge, and EgoCore. `tools/ExportFqtEditorTargets.ps1`
  generated `ghidra_out/fablewin_fqt_editor_targets.tsv` + `docs/FQT_EDITOR_TARGETS.md` with 5,793
  high-value symbols for level/world map editing, TNG/LEV merge, region/nav/mapwho, definitions, and
  chests/items. First post-import inspect targets: `CEditLevelMerger::ParseTngFile`,
  `CEditLevelMerger::LoadLevFile`, `CEditWorldMap::LoadSingleLevelFromFiles`,
  `CEditWorldMap::SaveSingleLevel`, `CScriptThing::MsgOpenedChest`.
- **Local ChocolateBox/SilverChest source found:** `D:\Code\Fable\ChocolateBox\ChocolateBox\src`
  contains `SilverChest.Modern` plus source for `SilverChest.WadBridge`, `SilverChest.TngBridge`,
  `SilverChest.LevBridge`, and `SilverChest.ModPackageBridge`; summary in
  `docs/LOCAL_CHOCOLATEBOX_SILVERCHEST.md`. Quick check: `SilverChest.TngBridge --list` on the FSE
  sample `BarrowFields.tng` returns 270 things, matching the raw `NewThing` count, so FQT's simple
  parser is OK for loose-TNG discovery; the richer bridge matters for edits, CTCs, variables,
  validation, and LEV-derived metadata.
- **Comprehensive modding environment plan:** `docs/COMPREHENSIVE_MODDING_ENVIRONMENT.md` now frames
  the real target as two reinforcing products: a Creation Kit-like TLC editor and a decompiled retail
  source base. User's installed game path is
  `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters`; `tools/InventoryInstalledGame.ps1`
  generated `ghidra_out/installed_game/*` confirming FSE installed, `FinalAlbion.wad` present, 397
  loose TNGs, 0 loose LEVs. `SilverChest.WadBridge --list` on that WAD found 796 entries = 398 LEV +
  398 TNG. Extracted BarrowFields LEV/TNG non-destructively to `work/level_samples/BarrowFields`;
  `SilverChest.LevBridge --export-theme-grid` produced `BarrowFields_theme_grid.csv`. EgoCore
  `--list` on installed `graphics.big` wrote `ghidra_out/egocore_graphics_big_list.tsv` with 8,112
  entries.
- **Installed game asset indexes:** `tools/ExportInstalledAssetIndexes.ps1`,
  `tools/ExportWorldAndChestIndexes.ps1`, `tools/WriteInstalledAssetReport.ps1`,
  `tools/WriteWorldChestReport.ps1`, and `tools/BuildFqtModdingCatalog.ps1` generated
  `docs/INSTALLED_GAME_ASSET_REPORT.md`, `docs/WORLD_AND_CHEST_INDEX.md`, and
  `ghidra_out/installed_game/fqt_modding_catalog.json`. Current installed-game counts: 57,128 BIG
  entries, 796 WAD entries, 398 WLD maps, 141 WLD regions, 21,764 loose-TNG things, and 391
  chest/key/reward/shop/container-like entities (148 chest, 16 silver-key, 16 key, 211 shop/trader).
- **Automation active:** watcher PID 4852 is alive (log `ghidra_out/continue_fablewin_pipeline.log`).
  Do not start duplicate Ghidra/BSim jobs while it is alive.
- **FSE labels applied safely after pipeline:** `LabelApplyDefaultOnly.java` applied
  `ghidra_out/labels_fse_addresses.tsv` without overwriting BSim names: rows=39, renamed=7,
  commented=38, created=1, skippedNamed=31, failed=1. Final stats:
  `stats_fable_after_fse_labels.log` = 35,479 named / 44,269 total (80.14%).

▶▶ **RESUME HERE if session died mid-pipeline:**
0. Run `powershell -ExecutionPolicy Bypass -File tools/StatusFablePipeline.ps1`. If watcher PID 4852
   is active, do not start duplicate Ghidra/BSim jobs; monitor the driver log and current step log.
   If the watcher died, check `analyze_fablewin.log` for "Save succeeded" before manually continuing.
1. Apply names to FableWin: `analyzeHeadless ... -process FableWin.exe -noanalysis -postScript
   ApplyNames.java "D:/Documents/FableTLC/ghidra_out/fablewin_pdb_names.tsv"` then same with
   `CreateMissingFunctions.java` (same TSV), then `ApplyNames.java` once more (sanitized stragglers).
2. Fresh BSim DB (avoids stale-record conflicts with old `bsim/fabletlc`):
   `bsim createdatabase file:/D:/Documents/FableTLC/bsim/fabletlc2 medium_nosize` (done if
   `bsim/fabletlc2.mv.db` exists)
3. `bsim generatesigs "ghidra:/D:/Documents/FableTLC/ghidra_proj/FableTLC?/ego_r.exe" bsim/sigs_egor2 --config medium_nosize`
   → `bsim commitsigs file:/... bsim/sigs_egor2`; then same for `FableWin.exe` → `bsim/sigs_fablewin`
   (~165k fns to decompile — hours; run in background, sequential — project lock).
4. `BSimApplyNames.java` on `Fable.exe` vs `fabletlc2` @ **0.7** then **0.5** (already-named skipped →
   gap-fill only), audit TSVs → then `DemangleAll.java` + `DumpStats.java`.
5. Update `coverage_baseline.md` + this file with the new named %.
6. Optional follow-up after BSim: apply FSE labels with `LabelApply.java` from
   `ghidra_out/labels_fse_addresses.tsv`, then inspect conflicts against BSim/PDB names.

## Later (Phase 2 — subsystem deep-dives, now on a 60%-named DB)
5. Parse `Data\Defs\DevHeaders\*.h` (extract from the archive) into the DBs for struct layouts.
6. Install `GhidraMCP` (from `D:\Documents\Fable2RE\REPlugins\GhidraMCP`, port 8089) for AI-driven
   review; `curl http://127.0.0.1:8089/check_connection`. **No XEXLoaderWV.**
7. FSE-target + string-xref + Lua-natives passes fill remaining gaps; begin `SYSTEMS_ANALYSIS.md`.
3. `python tools/fse_import/fse_manifest_to_ghidra.py`; in Ghidra `File > Parse C Source` →
   `tools/fse_import/fse_api.h` → *Apply Function Datatypes*.
4. Point Ghidra Script Manager at `tools/ghidra_scripts/`.
5. Phase 1 passes: RTTI/vtable → `FindStrXref` string sweep → FSE call-site pass (produce
   `ghidra_out/labels_fse_targets.tsv`) → `FindLuaNatives*`. Write the first `docs/SYSTEMS_ANALYSIS.md`.
6. Read FSE `dllmain.cpp` for the exact hook + pointer-init addresses; mirror key fabletlcmod.com
   wiki format pages into `refs/formats/`.

## Open questions to resolve early
- TLC's exact **Lua version** (5.0 vs 5.1) — picks `luadis.py` vs `luadis51.py`.
- Does the PC MSVC build retain full **RTTI**? (Expected yes → big class-naming win.)
- The engine's **string-hash** constant (Fable2 used `0x5BD5E995`) — naming the hash fn unlocks
  resource lookup.

## Docs to keep current
`FINDINGS.md`, `SYSTEMS_ANALYSIS.md`, `PROGRESS.md`, and this file. (Created on first RE session.)

## Current resume point (2026-07-18 13:55, authoritative)

Run `powershell -ExecutionPolicy Bypass -File tools/StatusFablePipeline.ps1`. If watcher PID 4852 is
active, do not start duplicate Ghidra/BSim jobs; monitor the driver log and current step log.

Current expected step: `bsim generatesigs ... FableWin.exe` to `bsim/sigs_fablewin` (~165k functions
to decompile; hours). After that, the watcher should commit FableWin signatures to `bsim/fabletlc2`,
run `BSimApplyNames.java` on `Fable.exe` at 0.7 then 0.5, demangle, dump stats, and update coverage.

The older mid-pipeline checklist above predates the completed FableWin import/name-apply steps; use
this section if the two differ.

## Current resume point (2026-07-18 14:12, authoritative)

The watcher completed. Do not restart the FableWin pipeline unless intentionally rebuilding the BSim
database.

Final RE state:

- Retail `Fable.exe`: 44,269 functions, 35,479 named, 8,790 default-named, 80.14% named.
- FableWin BSim @0.7: 188 applied, 0 failed.
- FableWin BSim @0.5: 570 applied, 0 failed.
- Demangle after FableWin: 715 demangled, 0 failed.
- Conservative FSE labels: 7 renamed, 1 function created, 31 existing names preserved.
- Report: `ghidra_out/fablewin_pipeline_report.md`.

Next best RE tasks:

1. Inspect first native gameplay targets in `docs/NATIVE_GAMEPLAY_TARGETS.md`, starting with
   `CTCChest`, `CChestDef`, `CEventOpenedChest`, `CGlobal_OpenChestScript`, and
   `CGlobal_GiveHeroItemsFromRewardChestScript`.
2. Begin subsystem notes in `docs/SYSTEMS_ANALYSIS.md` for chest/reward/inventory and TNG/LEV editor
   save/load behavior.

Next best editor/tooling tasks:

1. Implement FQT Milestone 1 from `docs/FQT_CREATION_KIT_TASK_PLAN.md`.
2. Use `ghidra_out/installed_game/fqt_modding_catalog.json` as the read-only bootstrap catalog.
3. Wire WLD/WAD/STB/TNG consistency and gameplay-definition validation into FQT diagnostics.

## Current resume point (2026-07-18 15:00, authoritative)

Post-pipeline subsystem/tooling work started.

Chest/reward native findings are now documented in `docs/SYSTEMS_ANALYSIS.md`, backed by:

- `ghidra_out/decomp_chest_core.log`
- `ghidra_out/decomp_reward_container.log`
- `ghidra_out/list_reward_container_symbols_fable.log`

Key confirmed retail addresses:

- `00750050` `CTCChest::GetNumberOfKeysToUnlock`
- `00750110` `CTCChest::DisplayLockedMessage`
- `00750540` `CTCChest::Open`
- `00750780` `CTCChest::FrameUpdate`
- `0074e500` `CTCContainerRewardHero::HasReward`
- `0074e860` `CTCContainerRewardHero::GetRewardItems`
- `0074ec50` `CTCContainerRewardHero::CreateItemPickupContainer`
- `0074ee90` `CTCContainerRewardHero::RewardHeroWithItems`
- `0074e240` `CTCContainerRewardHero::OnSerialise`

Important editor implications:

- Reward editing must model direct items, gold, dynamic potion/resurrection, and deferred pickup
  containers separately.
- `RewardEnabled` is persisted runtime/save state, not just static definition data.
- `OBJECT_CHOCOLATE_BOX_01` and `OBJECT_CHOCOLATE_BOX_UNGIVEABLE` are confirmed compiled object
  definitions and should be allowed in object/reward pickers.
- `CChestDef::Transfer` is still not mapped to a retail address; donor addresses from FableWin are
  not retail locations.

FQT code change made in `D:\Code\FQT\FQT`:

- `FableQuestTool\Services\GameDataCatalogService.cs`: WAD-extracted TNGs are now scanned from temp
  cache instead of copied into the Steam install. Loose TNG files take priority; cache fills missing
  files such as `creature_hub`.
- `FableQuestTool.Tests\GameDataCatalogServiceTests.cs`: added coverage for loose-vs-cache TNG
  selection.
- Verified with `dotnet test ... --filter FullyQualifiedName~GameDataCatalogServiceTests`: 4 passed.

Next best tasks:

1. Add an FQT service/model for `ghidra_out/installed_game/fqt_modding_catalog.json`.
2. Add FQT diagnostics for WLD/WAD/STB/TNG consistency and game.bin definition resolution.
3. Target-map retail `CChestDef::Transfer` via BSim nearest or donor string/reference matching, then
   decode chest reward/key fields from compiled `game.bin` payloads.

## Current resume point (2026-07-18 late, authoritative) — ★ PIVOT: C++ modding tool

**User directive (2026-07-18): build a NEW modding tool rewritten in C++ from all of this
work/resources/data.** This supersedes the FQT C# catalog-service task above (no FQT changes were
made beyond the earlier TNG-cache fix). FQT remains a feature/concept donor, not the build target.

**FableForge created at `D:\Code\FableForge`** (git repo, initial commit `bc7a3b3`):

- C++20, CMake + Ninja + WinLibs mingw64 g++; statically linked (mingw runtime DLL mismatch on
  PATH causes 0xC0000139 at startup otherwise — gotcha).
- `forgecore` library: WAD reader/extractor (layout from EgoCore `WADBackend.h`: entryCount@20,
  footerOff@28, per-entry magic 42), TNG parser (things/properties/CTC blocks + byte-exact
  round-trip serialization), and `fqt_modding_catalog.json` loader (nlohmann/json vendored;
  handles the string-encoded numbers PowerShell's ConvertTo-Json emitted).
- `forge` CLI: `wad list|extract`, `tng list|roundtrip`, `catalog info`.
- **Verified against the install:** FinalAlbion.wad = 796 entries (matches SilverChest ground
  truth); BarrowFields.tng (WAD-extracted) = 257 things, 191,614 bytes round-trips byte-identical;
  catalog counts match. Unit tests pass (`ctest`).
- EgoCore is **MIT-licensed** — its C++ format code is legally usable as reference/vendor source
  (credited in `THIRD_PARTY_NOTICES.md`).
- Roadmap in `D:\Code\FableForge\docs\ARCHITECTURE.md`: M2 LEV/WLD/STB readers + WLD↔WAD↔STB↔TNG
  validation → M3 TNG editing/WAD repack/chest-reward semantics from the retail RE → M4 ImGui GUI
  → M5 FSE runtime bridge.

Next best tasks (post-pivot):

1. FableForge M2: LEV reader (theme grid, vs `SilverChest.LevBridge --export-theme-grid` output at
   `work/level_samples/BarrowFields/BarrowFields_theme_grid.csv`) and WLD map/region graph reader
   (vs `ghidra_out/installed_game/wld_maps.csv` / `wld_regions.csv`).
2. RE task unchanged: target-map retail `CChestDef::Transfer`, then decode chest reward/key fields
   from compiled `game.bin` payloads — feeds FableForge M3 chest/reward editing.
3. Port FQT's WLD/WAD/STB/TNG consistency diagnostics into `forge validate` (native, not C#).

## Current resume point (2026-07-18 evening, authoritative) — autonomous session results

All three "next best tasks" above are DONE, plus the three long-standing open questions. Evidence
for everything in `docs/FINDINGS.md`; chest specifics in `docs/SYSTEMS_ANALYSIS.md`.

**Open questions resolved (binary-evidence scans, `tools/` + scratchpad `scan_fable.py`):**
- **Retail TLC embeds NO Lua at all** — zero version/API strings in the exe. All ecosystem Lua is
  FSE's own vendored **Lua 5.4.8 + Sol2**. `luadis*` is irrelevant to retail TLC.
- **RTTI fully present:** 1,973 unique `.?AV` class descriptors + 8 `.?AU` structs.
- **No murmur (`0x5BD5E995`)/FNV constants;** CRC32 machinery present (file offsets `0xE9A368`,
  `0xEAF980`) — hash-fn identification still open, CRC32 sites are first candidates.

**★ Retail `CChestDef::Transfer` = `0x004DE204`** (+ `Copy` `0x4E0564`, `GetSizeofClass`
`0x4D3840`, dtor `0x4D8072`) via the new **`tools/rtti_map.py`** RTTI vtable-slot porter
(donor thunk-resolving; validated against confirmed `CTCChest::FrameUpdate` `0x750780` at slot 9).
Labels TSV: `ghidra_out/labels_chestdef_rtti.tsv` (not yet applied to the Ghidra DB).
Key insight: retail strips def field-name strings → `game.bin` decodes by field ORDER; donor
Transfer names + retail Transfer offsets together give the decode map. Donor caveat: FableWin is
Anniversary-era (CTCChest 32 vs retail 28 slots) — re-align slots per class.

**FableForge (`D:\Code\FableForge`, commit `1aaddbc`) — M2 essentially shipped:**
- `forge wld info` — 398 maps/141 regions, byte-exact round-trip.
- `forge lev info|themegrid` — LEV layout from decompiled `FableMod.LEV`
  (`D:\Code\Fable\ChocolateBox\ChocolateBox\DecompiledDLLs\FableMod.LEV`): 25B header (version
  6404), 22B map header, 256×(128B name+u32) ground themes, 33,792B palette, (W+1)×(H+1)×21B
  cells — height float@+5 ×2048, theme idx @+10..12, strengths @+13..14, walk @+15. Theme-grid
  convention = **dominant (max-strength) blend layer**. Verified 16,384/16,384 cells vs LevBridge.
- `forge validate <game-root>` — install checks CLEAN, numbers match the catalog exactly.

Next best tasks:

1. Apply `labels_chestdef_rtti.tsv` with `LabelApply.java`; decode `CChestDef` field map by reading
   donor+retail Transfer side by side in Ghidra → answer SYSTEMS_ANALYSIS "Next Questions" 1.
2. Generalize `tools/rtti_map.py` into a bulk pass: port ALL 1,973 RTTI classes' vtable slots
   donor→retail where slot counts match (huge naming win beyond BSim), emit labels TSV.
3. FableForge: STB reader; then M3 TNG mutation API + WAD repack.

## Current resume point (2026-07-18 evening, authoritative) - RTTI force-apply + chest field map

The first two queue items above are DONE.

**Ghidra DB naming state:**
- `tools/rtti_port_all.py` bulk-ported 6,653 virtual method labels from the PDB-named FableWin
  donor to retail `Fable.exe` via MSVC RTTI vtable-slot identity.
- Conservative apply created 4,642 functions and renamed 4,473 default-named functions.
- `tools/ghidra_scripts/CompareLabels.java` audited the already-named rows:
  4,474 agreed with existing names, 2,179 disagreed, 0 missing. Disagreements were BSim
  generic/collided names; RTTI slot identity is stronger evidence.
- `tools/ghidra_scripts/LabelApplyForce.java` force-applied the RTTI source over those conflicts:
  2,180 renamed, 0 failures. `DemangleAll.java` then demangled 6,650 labels with 0 failures.
- Final stats after the force pass: 49,082 total functions, 40,187 named, 8,895 default-named,
  427 thunks, 202,189 symbols. Named count did not rise after force-apply because wrong existing
  names were replaced, but semantic correctness improved substantially.
- `0x004DE204` now decompiles as `CChestDef::Transfer`, not the stale BSim guess.

**Chest field result:**
- `CChestDef::Transfer` own fields are now mapped:
  `+0x25 PersistOnOpening`, `+0x28 OpenParticleEffect`, `+0x2C DisplayMessageOnEmpty`,
  `+0x30 OpenAnimationForCreature`, `+0x34 OpenerObject`, `+0x38 OpenersRequired`.
- These are opening/behavior fields only. Silver-key requirement and reward contents are not in
  the six own fields; continue into `CTCChest::GetNumberOfKeysToUnlock`, chest sub-defs, and reward
  scripts.

Next best tasks:

1. FableForge STB reader (FinalAlbion.stb has 424 entries per catalog), then wire STB into
   `forge validate`.
2. RE: decompile/name `CTCChest::GetNumberOfKeysToUnlock` dependencies and the reward scripts to
   locate the authoritative key-count and reward payload path.
3. FableForge M3: TNG mutation API + WAD repack once STB and reward semantics are grounded.

## Current resume point (2026-07-18 evening, authoritative) - FableForge STB shipped

FableForge queue item 1 above is DONE in `D:\Code\FableForge`:

- Added `forge::stb` reader for `FinalAlbion_RT.stb`: BBB-style entries,
  extraction, and `__STATIC_MAP_COMMON_HEADER__` static-map offsets.
- Added CLI: `forge stb list <file.stb>` and `forge stb extract <file.stb> <outdir> [filter]`.
- Wired STB into `forge validate`: retail install reports 424 entries, 398 static maps,
  and `stb static maps: 0 missing`; validation remains CLEAN.
- Added synthetic STB unit coverage; `ctest` passes.

Next best tasks:

1. RE: decompile/name `CTCChest::GetNumberOfKeysToUnlock` dependencies and reward scripts to
   locate the authoritative key-count and reward payload path.
2. FableForge M3: TNG mutation API over the byte-preserving document model.
3. FableForge WAD repack/mod-package staging, after TNG mutation tests exist.

## Current resume point (2026-07-18 evening, authoritative) - chest key semantics corrected

`CTCChest::GetNumberOfKeysToUnlock` is now read closely enough to correct the earlier caveat:

- It reads the chest sub-def from the owning thing definition (`thing + 0x70`), which is the
  retail `CChestDef` payload mapped above.
- It uses `CChestDef::OpenerObject` (`+0x34`) and `CChestDef::OpenersRequired` (`+0x38`).
- If both are positive, it resolves `OpenerObject` to a def-name string, writes that to the output
  `CCharString`, and returns `OpenersRequired`; otherwise it clears the string and returns `0`.
- `CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest` only validates the script thing, finds
  `CTCChest` via interface type `0x3c`, and calls `CTCChest::GetNumberOfKeysToUnlock`.

So: **required key object + count ARE in `CChestDef`; reward contents are still elsewhere** (likely
`CTCContainerRewardHero` data/sub-definitions/scripts). Evidence added to `docs/FINDINGS.md` and
`docs/SYSTEMS_ANALYSIS.md`; new decompile log:
`ghidra_out/decomp_chest_interface_after_rtti.log`.

Next best tasks:

1. Find where the runtime/script checks hero inventory against `OpenerObject` / `OpenersRequired`.
2. Use `CContainerRewardHeroDef::ObjectFamilies` (`+0x28`) plus compiled def payloads to resolve
   the actual reward family names/entries for chest UI columns.
3. Begin FableForge M3 TNG mutation API once reward read semantics are stable enough for UI columns.

Reward-definition follow-up completed after the above section:

- `CContainerRewardHeroDef::Transfer` (`0x004E8987`) has one own persisted field:
  `ObjectFamilies` at `+0x28`.
- `GetSizeofClass` returns `0x34`; `Copy` assigns the same tail field; donor Transfer names it
  `ObjectFamilies`.
- This means rewards are not in `CChestDef`; they are in `CContainerRewardHeroDef::ObjectFamilies`
  plus runtime expansion by `CTCContainerRewardHero::GetRewardItems` and possible attached
  `CTCGold`.

## CURRENT AUTHORITATIVE MEMORY (2026-07-18 16:10 MDT) - resume here

Project direction:

- Active build target is **FableForge** at `D:\Code\FableForge`, the native C++ rewrite consuming
  this RE project's data. FQT is a donor/reference only.
- `D:\Documents\FableTLC` is not a git repo; docs/scripts/logs here are updated on disk. FableForge
  is a git repo and is clean at commit `17b7b8e`.

FableForge state:

- Latest commits:
  - `17b7b8e` `M2: add STB static-map reader`
  - `1aaddbc` `M2: WLD + LEV readers and forge validate`
  - `bc7a3b3` initial C++ core
- Implemented formats/commands:
  - WAD: `forge wad list|extract`
  - TNG: `forge tng list|roundtrip`
  - WLD: `forge wld info`
  - LEV: `forge lev info|themegrid`
  - STB: `forge stb list|extract`
  - catalog: `forge catalog info`
  - validation: `forge validate <game-root>`
- Verification:
  - `ctest --test-dir D:\Code\FableForge\build --output-on-failure` passes.
  - `forge validate "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters"` reports CLEAN.
  - Retail counts: WLD 398 maps / 141 regions, WAD 796 entries, STB 424 entries / 398 static maps,
    all 398 WLD maps resolve in STB, all LEV/TNG resolve in WAD, 0 bad region refs, 21,764 things
    across 397 loose TNGs with 0 missing `DefinitionType`/`UID`.

Ghidra naming / RTTI state:

- `tools/rtti_port_all.py` bulk-ported 6,653 RTTI vtable-slot labels from FableWin donor to retail.
- `CompareLabels.java` audited conflicts; `LabelApplyForce.java` force-applied RTTI over stale BSim
  names where slot identity was stronger.
- Force pass results: 2,180 overridden, 0 failures; `DemangleAll.java` demangled 6,650, 0 failures.
- Final stats: 49,082 total functions, 40,187 named, 8,895 default-named, 427 thunks, 202,189
  symbols.

Chest/key/reward facts:

- `CChestDef::Transfer` = `0x004DE204`; `Copy` = `0x004E0564`; `GetSizeofClass` = `0x004D3840`;
  vec-dtor = `0x004D8072`.
- Retail `CChestDef` own fields:
  - `+0x25 PersistOnOpening`
  - `+0x28 OpenParticleEffect`
  - `+0x2C DisplayMessageOnEmpty`
  - `+0x30 OpenAnimationForCreature`
  - `+0x34 OpenerObject`
  - `+0x38 OpenersRequired`
- Corrected conclusion: **required key object + count ARE in `CChestDef`** via `OpenerObject` and
  `OpenersRequired`.
- `CTCChest::GetNumberOfKeysToUnlock` reads the owning thing's `CChestDef`, resolves
  `OpenerObject` to a def-name string, returns `OpenersRequired`, or returns `0` with an empty
  string if no gate is active. It does not check hero inventory itself.
- `CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest` only validates the script thing, finds
  `CTCChest` via interface type `0x3c`, and calls `CTCChest::GetNumberOfKeysToUnlock`.
- Reward contents are **not** in `CChestDef`.
- `CContainerRewardHeroDef::Transfer` = `0x004E8987`; one own persisted field:
  `ObjectFamilies` at `+0x28`. `GetSizeofClass` returns `0x34`; `Copy` assigns the same tail field.
- Runtime `CTCContainerRewardHero::GetRewardItems` expands `ObjectFamilies` into actual rewards,
  with separate handling for item rewards, attached `CTCGold`, dynamic potion dummy, and dynamic
  resurrection dummy.

Important evidence files:

- `docs/FINDINGS.md`
- `docs/SYSTEMS_ANALYSIS.md`
- `ghidra_out/labels_rtti_port.tsv`
- `ghidra_out/rtti_port_compare.tsv`
- `ghidra_out/decomp_chest_core.log`
- `ghidra_out/decomp_chest_interface_after_rtti.log`
- `ghidra_out/decomp_container_reward_def_transfer.log`
- `D:\Code\FableForge\docs\ARCHITECTURE.md`

Next best tasks:

1. Find where runtime/script checks hero inventory against `OpenerObject` / `OpenersRequired`.
2. Resolve `CContainerRewardHeroDef::ObjectFamilies` from compiled def payloads into actual family
   names/items for chest UI columns.
3. Start FableForge M3 TNG mutation API with tests, then WAD repack/mod-package staging.

## Current resume point (2026-07-18 ~23:00, authoritative) - chest chain closed end-to-end

Queue tasks 1 and 2 from the previous section are DONE (full evidence in FINDINGS.md, summary in
SYSTEMS_ANALYSIS.md "Answered Questions"):

- Key gate chain: `CGlobal_OpenChestScript::Main` (`0xEEC890`) -> `CGameScriptInterface`
  vtable (base `0x1260F0C`): `+0x6D4` GetNumberOfKeysNeededToUnlockChest (`0x8966C0`),
  `+0x2DC` GetNumberOfItemsOfTypeInInventory (`0x897190`) -> `GetDefGlobalIndexFromName`
  (`0x9AD410`) -> hero `CTCInventoryBase` count (`0x5BDF08`). Opens if required <= have; else
  "CHEST_OPEN_FAIL" + DisplayLockedChestMessage (`+0x6D8`, `0x896730`). Keys counted, NOT consumed.
- Reward model: `CContainerRewardHeroDef::ObjectFamilies` (+0x28, vector<long> of family def
  indices, random pick) -> `CObjectFamilyDef` (+0x3C, vector<pair<objectDefIdx,weight>>,
  weighted pick via `GetRandomObject` `0x8ED590`). Both Transfers are single-field thunks into
  persist-vector helpers (renamed in DB via `ghidra_out/labels_chest_reward_chain.tsv`).
  Retail CheckCRC field name is the empty string. Gold = attached `CTCGold` (iface 0xCC, thing
  flag +0x38 & 0x1000). Dynamic potion picks by hero inventory; failed resurrection -> +20 gold.
- Rewards live in compiled `game.bin` def payloads, not TNG properties.

New decomp/evidence logs: `findrefs_chest_callers.log`, `chest_script_syms.log`,
`decomp_open_chest_scripts.log`, `decomp_inventory_count_slot.log`,
`decomp_reward_expand_after_rtti.log`, `decomp_objectfamily_def.log`,
`decomp_family_transfer_helper.log`, `objectfamily_syms.log`.

Next best tasks:

1. FableForge M3: TNG mutation API with tests (in progress this session).
2. Enumerate `OBJECT_FAMILY_*` defs + global indices from game.bin for the reward picker;
   locate CContainerRewardHeroDef/CObjectFamilyDef payloads in compiled_game_entries.csv.
3. FableForge WAD repack/mod-package staging after TNG mutation tests exist.

## Current resume point (2026-07-18 ~23:30, authoritative) - M3 TNG mutation API shipped

FableForge commit `4c4dfb3` (`M3: TNG mutation API`), on top of `17b7b8e`:

- `forge::tng` document model now line-indexed (Thing/CtcBlock/Property carry raw-line indices).
- Mutations: `setThingProperty` (edit-in-place or insert before EndThing), `removeThingProperty`,
  `setCtcProperty`/`removeCtcProperty` (named CTC block, throws on missing block for set),
  `addThing` (serialized before `XXXSectionEnd;`, blank-line separated, CRLF-detected),
  `removeThing` (block + one adjacent blank separator).
- Invariant: mutations edit only the raw lines they touch, then reindex by re-parse; untouched
  bytes round-trip exactly. `ctest` passes (new `testTngMutation`); real
  `BarrowFields.tng` (191,614 bytes, 257 things) still round-trips byte-identically.

Next best tasks:

1. FableForge: WAD repack/mod-package staging (write modified TNGs back into a WAD or loose-file
   overlay; loose TNGs take priority in the engine per earlier FQT finding).
2. Enumerate `OBJECT_FAMILY_*` defs + global indices from game.bin for the reward picker;
   locate CContainerRewardHeroDef/CObjectFamilyDef payloads in compiled_game_entries.csv.
3. Wire chest semantics into a `forge chest list` command: per-level chest placements with
   DefinitionType, OpenerObject/OpenersRequired, and reward family columns (needs task 2).

## Current resume point (2026-07-19 ~00:30, authoritative) - reward data + deployment shipped

All three queue tasks done:

1. **Families enumerated (evidence in FINDINGS.md):** `tools/DecodeObjectFamilies.ps1` emits
   `ghidra_out/installed_game/object_families.{json,csv}` - 41 OBJECT_FAMILY defs (weighted
   per-mille member lists, defIdx 0 = no drop) and 72 CContainerRewardHeroDef entries (creature
   loot tables; chests use TNG `ContainerContents[n]` for fixed contents - 162 chest placements,
   115 with fixed contents). game.bin entry index = runtime global def index, proven by
   cross-reference. `tools/bin_dump` now has a payload hex-dump mode.
2. **`forge chest list <game-root> [level]`** (FableForge `054ebf1`): UID, definition, silver-key
   count, position, fixed contents vs def-level random fallback.
3. **WAD repack + mod staging** (FableForge `6e74b69`): `forge::wad::repack` preserves all
   uninterpreted bytes (retail FinalAlbion.wad no-change repack verified SHA256-identical);
   `forge stage/unstage <game-root>` = loose-file overlay with .forgebak backups + manifest.
   Test-suite gotcha fixed: Release builds had NDEBUG silencing every assert - test file now
   `#undef NDEBUG`.

FableForge is at `6e74b69`; `ctest` passes; `forge validate` CLEAN.

Next best tasks:

1. Bar #1 sweep: measure clean-decompile coverage on Fable.exe (script over all 49,082 fns:
   decompile, log failures/undefined/mis-split) -> `ghidra_out/coverage.tsv` per FULL_DECOMP.md.
2. FableForge: native game.bin/names.bin reader (BBB zlib chunks - vendor miniz) so the reward
   picker and def validation drop the .NET bin_dump dependency.
3. Configure `auto-re-agent` + VC7.1 harness for the first module lift (chest/reward subsystem is
   the best-evidenced candidate).

## Current resume point (2026-07-19 ~01:30, authoritative) - native defs reader + lift plan

Progress on the three queued tasks:

1. **Bar #1 sweep RUNNING** - `tools/ghidra_scripts/DecompCoverage.java` decompiling all 49,082
   retail functions to `ghidra_out/coverage.tsv`. At 45k/49k: only 2 decompile failures so far
   (99.995% clean rate). Summary to be appended below when it completes.
2. **Native CompiledDefs reader DONE** (FableForge `033868c`): `forge::bin` reads
   names.bin/game.bin (vendored public-domain miniz for the zlib chunks - the SDL3 copy needed
   de-SDL-ifying: MINIZ_STATIC_FUNCTIONS, MINIZ_SDL_MALLOC, and NO_INFLATE/NO_ZLIB trims reverted).
   CLI: `forge defs list|show|families`. Validated exactly against the C# dump: 14,761 entries,
   41 families, 72 reward defs.
3. **Lift harness PLANNED, blocked on toolchain** - `docs/LIFT_HARNESS.md`. **MSVC 2003/VC7.1 is
   not installed on this machine** (searched); user must supply the VS.NET 2003 installer or the
   free VC++ Toolkit 2003, or accept the modern-MSVC behavioral-only fallback. First lift module
   is chest/reward (13 functions, all with verified addresses + data oracles).

Next best tasks:

1. Summarize coverage.tsv when the sweep completes (failure classes -> fix list), update
   coverage_baseline.md.
2. Ask user for VC7.1 media / decide fallback, then execute LIFT_HARNESS.md setup.
3. FableForge: game.bin WRITE path (BinFile.Save equivalent) to enable loot-table editing.

## Current resume point (2026-07-19 ~02:00, authoritative) - BAR #1 MEASURED: 99.996% clean

Sweep complete (`ghidra_out/coverage.tsv`, summary in `ghidra_out/coverage_baseline.md`):

- **49,082 functions, 49,080 decompile cleanly.** 2 failures: `RunCutsceneMacro_Func`
  `0x00CBFB7D` (74 KB, 60s timeout; 600s retry was still running in background log
  `tasks/b3r7wxg8d.output` when the session ended - check it) and `Main` `0x00DECAD0`
  (Pcode decode error, needs instruction-level look).
- Unnamed tail: 8,899 functions but only 1.9 MB of 14.8 MB .text (avg 219 bytes) - bar #1 is
  closed on the decompile axis; remaining work is naming/typing quality.
- Retail exe DLL surface scanned for the modernization question: only system DLLs + OpenAL +
  msvcr71/msvcp71. No Bink/Miles/Havok/Granny imports - no closed middleware blocker for a port.
- New scripts: `tools/ghidra_scripts/DecompCoverage.java`, `RetryDecomp.java`.
- `docs/LIFT_HARNESS.md` written; VC7.1 still needed from user (or modern-MSVC behavioral
  fallback). User asked about modernized x64/C++23 port - answer: two-track (faithful VC7.1
  reconstruction as oracle first, modern port as fork); lift style should be portable-C++03
  (typedef'd fixed-width types, no pointer-size assumptions) so sources serve both tracks.

Next best tasks:

1. Check `RunCutsceneMacro_Func` 600s retry result; investigate `0x00DECAD0` Pcode error.
2. VC7.1 decision from user -> execute LIFT_HARNESS.md; write lift style guide (portable C++03).
3. FableForge: game.bin write path for loot-table editing.

## Current resume point (2026-07-19 ~03:00, authoritative) - bin writer + GUI concept

- **game.bin WRITE path shipped** (FableForge `347bc91`): `forge::bin::File::save` +
  `setEntryData`/`addEntry`; `forge defs roundtrip` proves semantic identity on all three retail
  bins (game 14,761 / script 611 / frontend 810). Loot-table editing is now end-to-end possible.
- **Coverage stragglers:** `RunCutsceneMacro_Func` FIXED via `setMaxPayloadMBytes(512)` (1.02 MB
  of pseudo-C); `Main@0xDECAD0` still fails (callee-metadata suspicion; FindBadPcode shows all
  instructions clean, FixDecompMeta reset did not help). **49,081/49,082 = 99.998% clean.**
- **GUI concept artifact** published for user review (SubuwuTuner theme: #1A1C21 chrome, purple
  #8C59D9 accent from `D:\Subuwu\code\src\ui\src\theme.cpp`; layout: world tree / chest browser /
  inspector / loot-family panel / status chips). URL in session log. M4 shell plan: reuse
  SubuwuTuner's ImGui+GLFW skeleton + theme.cpp (user's own Apache-2.0 code).

Next best tasks:

1. FableForge M4: ImGui shell (borrow SubuwuTuner theme.cpp/panel structure), read-only chest
   browser + family viewer first, then contents editing via TNG mutation + stage.
2. `Main@0xDECAD0`: dump callee prototypes to find the poisoned symbol.
3. VC7.1 decision from user -> LIFT_HARNESS.md.

## Current resume point (2026-07-19 ~04:00, authoritative) - GUI v2 + coverage closed to 99.998%

- **GUI concept REDESIGNED** (user rejected v1 as "corporate/too SubuwuTuner"): v2 is a
  Creation-Kit/xEdit-style modding tool - docked panels with title bars, an Object Window
  (World/Definitions trees), a **live top-down render viewport** drawing the REAL BarrowFields.lev
  terrain (height+theme decoded via tools/Decode... packed to 128x128 in scratchpad/pack_level.ps1),
  Cell View grid of real placed things, and an xEdit-style record editor (chest gate + contents,
  or decoded loot-family weighted bars). Dark scrollbars (user complaint fixed), square chrome.
  Artifact URL reused: claude.ai/code/artifact/e21c04f3. Kept user's dark+purple.
  **M4 build note:** borrow SubuwuTuner's ImGui+GLFW skeleton but NOT its app-chrome look; the
  viewport = render the LEV theme grid we already parse; record editor = forge::bin + TNG mutation.
- **Coverage stragglers resolved:** RunCutsceneMacro_Func fixed (payload cap); Main@0xDECAD0 is
  the one remaining un-decompilable fn (deep referenced-struct metadata fault; not a direct callee
  - see coverage_baseline.md). **49,081/49,082 = 99.998% clean.**

Next best tasks:
1. FableForge M4: real ImGui shell (theme first, then Object Window + render viewport + Cell View
   + record editor wired to forgecore). This concept is the spec.
2. VC7.1 decision from user -> LIFT_HARNESS.md.
3. Optional: chase Main@0xDECAD0 by dumping transitive struct field sizes.

## Current resume point (2026-07-19 ~05:00, authoritative) - GUI v3 + content-authoring plan

- **GUI concept v3** (approachable rethink; user rejected v2 as intimidating + emoji tofu): welcoming
  Home, plain-language task rail (World/Chests/Loot/Creatures/Validate), guided chest editor (key
  toggle+stepper, reward chips, progressive "Advanced"), loot editor with live weight sliders + donut,
  kept the real BarrowFields map viewport. **All icons inline SVG** (tofu fixed). Dark scrollbars.
  Artifact URL reused: claude.ai/code/artifact/e21c04f3.
- **docs/CONTENT_AUTHORING_PLAN.md written** answering scripts/quests/dialogue/cutscenes + Blender:
  - Quests/dialogue/cutscenes are authored as scripts -> pragmatic path is FSE Lua (843-fn Quest API,
    proven via FQT), native VM decomp is long-horizon (Stage D).
  - Dialogue/text = Babel text.big/dialogue.big; cutscene = native macro RunCutsceneMacro_Func +
    FSE Camera* API.
  - **★ Blender round-trip already exists in EgoCore code**: MeshCompiler/AnimCompiler +
    GltfMeshImporter/GltfAnimImporter + LipSyncCompiler/SpeechAnalyzer + TextureBuilder. So custom
    meshes/anims via Blender is orchestration, not new RE. Staged B1 view -> B2 addon (custom
    meshes) -> B3 level-layout-in-Blender over the safe TNG writer.
- Main@0xDECAD0 static badtype hunt found nothing (locals not populated because decompile fails);
  genuine dead-end, staying at 99.998% clean.

Next best tasks:
1. FableForge M4 ImGui shell (v3 concept is the spec) OR Stage-A Dialogue/Text editor (text.big) -
   both are strong next builds; text editor is cheaper and high-visibility.
2. Blender B1: "Open in Blender" via EgoCore --batch glTF (near-free).
3. VC7.1 decision -> lift harness (gates Stage D native quest/cutscene editing).

## Current resume point (2026-07-19, latest) - 3D editor direction + script VM plan

- User feedback on GUI:
  - v1 was too corporate / too SubuwuTuner.
  - v2 was closer to Creation Kit/xEdit but too intimidating.
  - v3 was more approachable, but still had garbled symbols.
  - v4 was created locally as
    `C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\8ef8ce14-af2e-46fd-ae46-80637e9e3007\scratchpad\fableforge-gui-v4.html`.
    It is ASCII-only after byte-safe inlining (`non-ASCII chars: 0`) and includes a WebGL-style 3D
    heightmap/world viewport concept. Previous session hit usage limit before publishing/reporting.
- UI direction from user:
  - keep dark + purple;
  - no garbled/emoji glyphs;
  - needs to be intuitive, non-intimidating, functional, powerful, beautiful, modern;
  - should support real 3D world manipulation, not only tables/top-down maps.
- 3D editor direction:
  - primary workspace should be a real 3D level viewport over LEV terrain + TNG placed entities;
  - entity selection should drive the inspector and script/quest links;
  - move/rotate/duplicate/delete placements in 3D, with property table available as advanced mode;
  - write placement transforms back through `forge::tng` mutation API;
  - Blender integration should be optional/pro workflow: export LEV terrain + TNG placements to Blender
    as mesh + empties, then import placement changes back to TNG.
- Script/entity/VM answer:
  - TNG entities attach script identity through `ScriptName` and `ScriptData`.
  - `script.bin` is readable with `forge defs`; current counts are 611 entries:
    595 `CCutsceneDef`, 14 `CRegionScriptDef`, 2 `CScriptDef`.
  - Decomping the native script VM/executors gives semantics; decoding `script.bin` gives shipped
    content; TNG indexing tells where content is attached in the world. FableForge needs all three.
  - New doc written: `docs/SCRIPT_VM_RE_PLAN.md`.
- Decomp coverage:
  - still 49,081 / 49,082 clean.
  - `RunCutsceneMacro_Func` decompiles with 512MB payload cap.
  - one holdout: `Main@0x00DECAD0`, likely Ghidra metadata/streaming quirk.
  - program-wide zero-length type scan found only `/mmsystem.h/TIMECALLBACK`; do not mutate the main
    DB based only on that weak lead.

Next best tasks:
1. Publish or rebuild the v4 GUI concept, then inspect it in-browser for remaining tofu/garbling and
   actual 3D viewport behavior.
2. Start `CCutsceneDef` payload mapping: dump payload hex, compare similarly named cutscenes, and use
   `RunCutsceneMacro_Func` reads as the executor oracle.
3. Begin real M4 GUI shell around the 3D viewport: world tree, 3D render window, cell/entity list,
   inspector, script links, and staged save/apply.

## Current resume point (2026-07-19, latest+1) - script refs CLI shipped

- FableForge commit `220ea3b` adds `forge script refs <game-root> [level-filter]`.
- It scans loose `data/Levels/FinalAlbion/*.tng` for top-level `ScriptName` / `ScriptData`, prints
  level, UID, thing type, script, definition, link classification, and position.
- Validation:
  - build succeeded;
  - `ctest --test-dir D:\Code\FableForge\build --output-on-failure` passed;
  - BarrowFields filter: 319 script refs across 12 matching TNGs;
  - full install: 21,764 script refs across 397 TNGs.
- Full-install link summary from the first pass:
  - `camera-marker`: 1,922 (`CAM_*`);
  - `marker`: 1,310 (`MK_*`);
  - `cutscene-name`: 51 (`CS_*`);
  - exact `script.bin`: 1;
  - unclassified/other: 18,480.
- Interpretation: exact name matching is intentionally conservative and weak for now. The next pass
  should build better prefix/name-resolution rules between TNG names and `CCutsceneDef` /
  `CRegionScriptDef` / native `NScript::*` handlers.

Next best tasks:
1. Publish/rebuild and inspect GUI v4; then start real M4 ImGui shell with a 3D viewport.
2. Improve `forge script refs` classification with real cross-link rules from `script.bin` payloads and
   native name lookups.
3. Start `CCutsceneDef` payload mapping using payload diffs + `RunCutsceneMacro_Func`.

## Current resume point (2026-07-19, latest+2) - cutscene commands + UI research

- FableForge commit `57aa6ed` adds `forge script cutscenes <game-root> [filter]`.
- It decodes the visible `CCutsceneDef` macro-command stream:
  - payload structure observed: 9-byte header, u32 command count at `+0x09`, then N null-terminated
    command strings from `+0x0D`;
  - validated on all 595 retail cutscenes.
- Full retail `script cutscenes` top command verbs:
  - `GamePause=2982`
  - `UseCamera=1713`
  - `DoScriptFrame=762`
  - `FadeOut=684`
  - `FadeIn=569`
  - `HERO.Teleport=521`
  - `WaitActiveDialog=365`
  - `DoCameraPreloading=338`
  - `HERO.PlayAnimation=314`
  - `Create=312`
  - `RemoveExtras=301`
  - `CreateEffect=244`
  - `PutUpYourSwords=221`
  - `HERO.EntitySetMaxWalkingSpeed=205`
  - `HERO.ClearCommands=203`
  - `NoLoadUseCamera=194`
- Arena filter sanity check: 23 cutscenes shown; `CS_ARENA_ROUND_1` has 69 commands and starts with
  `UseCamera P_ARENACUOC,-1,NULL,0,0`.
- Tests: `ctest --test-dir D:\Code\FableForge\build --output-on-failure` passed after both M4 commits.
- New research doc: `docs/FABLEFORGE_UI_UX_RESEARCH.md`.
  - It includes web research sources and design conclusions from Creation Kit/GECK, xEdit, Hammer,
    Blender, Unreal, REDkit, OpenIV, Aurora/NWN, Unity, and Starfield Creation Kit.
  - Core UI conclusion: FableForge should be viewport/outliner/inspector first, with task presets
    (World building, Quest authoring, Conflict review, Packaging, Beginner), synchronized selection,
    explicit mod safety, and raw record views as advanced truth rather than the first screen.

Next best tasks:
1. Implement `forge script cutscene <game-root> <name-or-index>` to print the full command list for
   one cutscene; use it for GUI timeline and VM executor labeling.
2. Improve `script refs` cross-linking using decoded `CCutsceneDef` command references (`CAM_*`,
   `MK_*`, `P_*`) rather than only TNG `ScriptName`.
3. Build/inspect GUI v4 or start real ImGui M4 shell with 3D LEV terrain + selectable TNG placements.

## Current resume point (2026-07-19, latest+3) - individual cutscene dump shipped

- FableForge commit `b282e6c` adds `forge script cutscene <game-root> <name-or-index>`.
- Example validation:
  - `forge script cutscene <root> CS_ARENA_ROUND_1`
  - output: entry 15, 2,310 bytes, 69 commands;
  - first commands include:
    - `UseCamera P_ARENACUOC,-1,NULL,0,0`
    - `PutUpYourSwords`
    - `Create CREATURE_BS_VILLAGER_MALE, P_CROWD1, Crowd1`
    - repeated `DoScriptFrame 1` pacing commands.
- Same commit improves `forge script refs`:
  - ignores `ScriptName NULL` placeholder rows;
  - tokenizes all decoded cutscene commands and labels TNG script names referenced by those commands
    as `cutscene-ref`.
- New retail script-ref counts:
  - BarrowFields filter: 121 meaningful refs across 12 TNGs;
  - full install: 8,063 meaningful refs across 397 TNGs;
  - full-install link summary:
    - `cutscene-ref`: 2,544;
    - `camera-marker`: 853;
    - `marker`: 222;
    - `cutscene-name`: 19;
    - exact `script.bin`: 1;
    - unclassified: 4,424.
- Validation:
  - build succeeded;
  - `ctest --test-dir D:\Code\FableForge\build --output-on-failure` passed.

Next best tasks:
1. Add a structured JSON/CSV output mode for `script refs` and `script cutscene` so the GUI can consume
   them without scraping text tables.
2. Map `CCutsceneDef` command verbs to native executor handlers in Ghidra, starting with
   `UseCamera`, `GamePause`, `DoScriptFrame`, `Create`, `FadeOut`, `FadeIn`, `WaitActiveDialog`.
3. Start the real M4 ImGui shell around a 3D LEV terrain viewport and TNG entity selection.

## Current resume point (2026-07-19, latest+4) - JSON script exports

- FableForge commit `5e8b9e9` adds GUI-consumable JSON output:
  - `forge script refs <game-root> [level-filter] --json`
  - `forge script cutscene <game-root> <name-or-index> --json`
- Validation:
  - build succeeded;
  - `ctest --test-dir D:\Code\FableForge\build --output-on-failure` passed;
  - `script refs <root> BarrowFields --json` parsed via PowerShell `ConvertFrom-Json`:
    `refs=121`, `tngs=12`, `cutscene-ref=32`;
  - `script cutscene <root> CS_ARENA_ROUND_1 --json` parsed via `ConvertFrom-Json`:
    `name=CS_ARENA_ROUND_1`, `bytes=2310`, `commands=69`, first verb `UseCamera`;
  - text mode still works for index lookup (`script cutscene <root> 15`).
- This gives the planned GUI/3D inspector a stable source for:
  - entity-attached scripts;
  - cutscene-linked markers/cameras;
  - full cutscene command timelines.

Next best tasks:
1. Ghidra: map cutscene macro verbs to native executor handlers. Start with string/xref searches for
   `UseCamera`, `GamePause`, `DoScriptFrame`, `Create`, `FadeOut`, `WaitActiveDialog`.
2. FableForge GUI M4: use JSON commands above as the first non-mock data source in the 3D inspector.
3. Extend JSON to `script cutscenes --json` if the GUI needs a full cutscene index in one call.

## Current resume point (2026-07-19, latest+5) - cutscene JSON index + native verb xrefs

- FableForge commit `0abdacb` adds `forge script cutscenes <game-root> [filter] --json`.
- Validation:
  - build succeeded;
  - `ctest --test-dir D:\Code\FableForge\build --output-on-failure` passed;
  - `script cutscenes <root> ARENA --json` parsed via `ConvertFrom-Json`:
    `shown=23`, `total=595`, first row `CS_ARENA_ROUND_1`, top command `GamePause=156`;
  - `script cutscenes <root> --json` parsed via `ConvertFrom-Json`:
    `shown=595`, `total=595`, rows=595, top command `GamePause=2982`;
  - text mode still works.
- New Ghidra script: `tools/ghidra_scripts/FindExactStrXref.java`.
- RE sidecar result:
  - log: `ghidra_out/cutscene_exact_verb_xrefs.log`;
  - exact strings avoid the previous broad `Create`/Win32 import noise;
  - all core macro verbs xref into `RunCutsceneMacro_Func` (`0x00CBFB7D`);
  - `UseCamera` and `DoCameraPreloading` also xref into `CutsceneCameraPreloader`
    (`0x00CBF29F`).
- `docs/FINDINGS.md` updated with exact per-verb xref addresses:
  - `UseCamera`, `DoCameraPreloading`, `FadeIn`, `FadeOut`, `Create`, `CreateEffect`,
    `NoLoadUseCamera`, `PutUpYourSwords`, `GamePause`, `DoScriptFrame`,
    `RemoveExtras`, `WaitActiveDialog`.

Next best tasks:
1. Ghidra: decompile/export `RunCutsceneMacro_Func` slices around the exact xref offsets and name
   helper calls inside each per-verb handler block.
2. FableForge: add `script command-stats --json` or include verb histograms in GUI bootstrap data.
3. GUI M4: use `script refs --json`, `script cutscenes --json`, and `script cutscene --json` as the
   first real data sources for the 3D inspector/timeline.

## Current resume point (2026-07-19, latest+6, authoritative) - cutscene VM cracked open

All three queue items above are DONE this session.

**★ Complete native cutscene verb API extracted: 184 verbs** (61 entity-scoped `.Verb` + 123
global), from the `RunCutsceneMacro_Func` dispatch chain itself. Full analysis in FINDINGS.md
(three new 2026-07-19 sections). Key facts:

- Interpreter architecture: linear case-sensitive `strncmp` PREFIX-match verb chain; args
  pre-split into `CCharString` locals (arg1 `local_44`, arg2 `local_40`, arg3 `local_3c`...);
  blocking waits pump the engine per tick at nominal **15 ticks/sec** (`GamePause secs` = secs*15
  ticks; `,clock` variant uses `GFGetTime`), break on `IsCutsceneSkipped()`.
- Cutscene manager global `DAT_0143E8F8`, vtable slots pinned: `+0x1C` tick pump, `+0x118` get
  hero CScriptThing, `+0x120` find CScriptThing by name, `+0x5C0` dialog-active query.
- Named in DB (labels_cutscene_helpers.tsv, 3/3 applied): `GFCharStringToFloat` `0x0099E690`,
  `GetCutsceneActorScriptThing` `0x00CBF9DE`, `FindRegisteredCutsceneActor` `0x00CD2B86`.
- 43 native verbs UNUSED by all 595 shipped cutscenes (CreateLight, CameraRig, .TurnInto,
  LiftRock, ExitGame...) = modder headroom; 8 shipped commands are silent no-ops (typos like
  `dGamePause`, `nop`, `.ClearActions`); 5 more only work via prefix-match accident.
- Evidence: `ghidra_out/cutscene_verb_slices.md` (12 verb slices + helpers),
  `ghidra_out/runcutscenemacro_full.c` (1.02 MB full pseudo-C),
  `ghidra_out/cutscene_native_verbs.txt` (184 verbs),
  `ghidra_out/cutscene_verb_census.tsv` (native vs content diff),
  `ghidra_out/decomp_cutscene_helpers.log`. New script: `DumpVerbSlices.java` (per-address
  pseudo-C slice dump with line/address mapping; note the bat `=`-splitting gotcha in CLAUDE.md).

**FableForge** (now at `bce67c7`):
- `82be9d7` `forge script command-stats [--json]` - full verb histogram with cutscene spread +
  example commands.
- `bce67c7` collapses `NAME.Verb` -> `.Verb` action table: 1,563 raw verbs = **156 distinct
  actions** in content. `ctest` passes.

**GUI v4 concept PUBLISHED:** https://claude.ai/code/artifact/a378f0d4-d431-4653-a679-fc93acd3274c
(the old e21c04f3 URL from prior sessions is not resolvable from a fresh session; this is the new
canonical URL). v4 = ASCII-only, dark+purple, WebGL-style 3D heightmap viewport concept. Awaiting
user feedback in-browser.

Next best tasks:
1. Enumerate the full strncmp chain ORDER from `runcutscenemacro_full.c` (prefix-collision
   routing, e.g. `Remove` vs `RemoveAll*`), and extract per-verb arg signatures (which of
   local_44/40/3c/38/34 each handler reads + GFCharStringToFloat/Int conversions) -> becomes
   FableForge cutscene validation/autocomplete table.
2. FableForge: `forge script validate` - flag cutscene commands that aren't one of the 184 verbs
   (catches the 8 dead + 5 prefix-accident forms in shipped + modded content); data source
   `ghidra_out/cutscene_native_verbs.txt` (vendor it into the repo).
3. GUI M4 real ImGui shell (v4 concept is the spec) wired to `script refs/cutscenes/cutscene
   --json` + `command-stats --json`.

## Current resume point (2026-07-19, latest+7, authoritative) - validator + GUI fix

Queue items 2 done; GUI concept advanced.

**FableForge `18f81e7`: `forge script validate <game-root> [filter] [--json]`.**
- Vendored the 184 native verbs into `libs/forgecore/include/forge/cutscene_verbs.hpp`
  (`forge::cutscene::kNativeVerbs` + `resolveVerb`/`verbToken`/`comparedTokenLength`,
  regenerate from `FableTLC/ghidra_out/cutscene_native_verbs.txt` - do not hand-edit).
- Replicates the engine's case-sensitive strncmp PREFIX dispatch; unit-tested
  (`testCutsceneVerbs` in tests/test_formats.cpp). `ctest` passes; `forge validate` still CLEAN.
- On retail: 10 dead commands (shipped typos incl. `Teacher.ClearActions` x27, `GamePAuse`,
  `Fadeout`), 20 loose prefix-accident commands. New FINDINGS.md section
  "Shipped-content cutscene bug census". These are real Lionhead bugs in the retail game.
- FableForge commits this session: `82be9d7` command-stats, `bce67c7` .Verb action collapse,
  `18f81e7` script validate. Head = `18f81e7`.

**GUI v4 chest-row fix (user-reported "text cramped horizontally"):** `.item .tx` was a plain
inline `<span>` so the chest name and reward subtitle rendered on the same line with no gap and
never truncated (ellipsis needs block/flex), letting the key-count pill overlap. Fixed to a
vertical flex column with block children that truncate. Republished (same artifact, ASCII-clean):
https://claude.ai/code/artifact/a378f0d4-d431-4653-a679-fc93acd3274c . Source file:
scratchpad `fableforge-gui-v4.html`. Still awaiting broader user feedback on the 3D viewport.

Next best tasks (unchanged priority):
1. Per-verb arg signatures + strncmp chain ORDER from `runcutscenemacro_full.c` -> richer
   validation (arg counts/types per verb, not just verb existence) + autocomplete.
2. GUI M4 real ImGui shell (v4 concept is the spec) wired to the `script *` --json commands +
   `command-stats --json` + `validate --json`.
3. FSE angle still open: 843-fn Quest API in `refs/fse_api_manifest.json` is the runtime-scripting
   authoring path (vs native cutscene macros); not yet wired into FableForge.

## Current resume point (2026-07-19, latest+8, authoritative) - arg sigs, fixup, FSE, vision

All of queue items 1-3 above are DONE, plus a shipped-typo fixer and a unified-scripting vision
answering the user's "expand FSE + FQT visual scripting + seamless" question.

**RE (task 1):** `tools/extract_cutscene_arg_sigs.py` parses `runcutscenemacro_full.c` and emits
`ghidra_out/cutscene_verb_args.tsv` = all 184 verbs with exact dispatch ORDER + heuristic arg
signature. Confirmed the chain is deliberately longest-prefix-first for colliding prefixes
(RemoveAllThings 75 < RemoveAll 76 < RemoveExtras 79 < Remove 168). Arg-hint caveats: float args
parsed by the __fastcall `GFCharStringToFloat` show as `str`; some entity verbs delegate parsing
and show 0 args. `tools/gen_verb_info_header.py` regenerates the vendored FableForge header.

**FableForge (tasks 1-3):**
- `34b4b59` `script verbs|fixup`: `forge::cutscene::{decodeCommands,encodeCommands}` codec
  (trailing-byte-preserving, unit tested); `script verbs` reference; `script fixup <root>
  [--write]` corrects the 4 unambiguous shipped typos (Teacher.ClearActions->.ClearCommands x27,
  GamePAuse, Fadeout, HERO.SetEntityMaxWalkingSpeed), backs up to *.forgebak, leaves the 6
  ambiguous dead verbs. **Verified write path on a COPY** (scratchpad/fixtest): 30 entries edited,
  dead 10->6, re-decodes to 595 cutscenes clean. Did NOT touch the real install.
- `d34feea` `forge::fse` + `fse list|show`: reads `refs/fse_api_manifest.json` (931 fns, BOM-
  tolerant), vendored to `docs/re_reference/`. The runtime-scripting surface now lives in the tool.
- `c14b5a1` `docs/UNIFIED_SCRIPTING.md`: vision for FSE + FQT node scripting + cutscene VM + data
  spine as one seamless tool. FQT visual scripting = 104 nodes (19 trigger/61 action/14 cond/8
  flow/2 custom), each a `CodeTemplate` emitting FSE Lua; port = data-model the nodes in forgecore,
  overlay on manifest-generated base, graph->Lua codegen (FQT `CodeGenerator` is the contract).
- FableForge head = `c14b5a1`. `ctest` green, `forge validate` CLEAN.

**RE evidence added:** FINDINGS.md sections "Shipped-content cutscene bug census" and the arg-sig
extraction; new files `ghidra_out/cutscene_verb_args.tsv`, `cutscene_verb_census.tsv`,
`tools/extract_cutscene_arg_sigs.py`, `tools/gen_verb_info_header.py`.

Next best tasks:
1. `forge::world` spine object (load a game root once -> typed lookups for defs/families/entities/
   levels/cutscenes/fse) so every editor reads one model. See UNIFIED_SCRIPTING.md section 5.
2. Port FQT's 104-node model + graph->Lua codegen into forgecore (headless, snapshot-tested),
   overlaid on manifest-generated base nodes.
3. Sense-(A) FSE expansion: generate Sol2 binding stubs from the Ghidra DB (address + demangled
   signature) for reversed engine fns not yet in FSE - separate FSE DLL build, live-game tested.
4. GUI M4 ImGui shell: cutscene timeline + quest/entity node canvas + 3D world viewport, all over
   the spine and the safe writers.

## Current resume point (2026-07-19, latest+9, authoritative) - FULL DEF SCHEMA + level-editor RE

★ Biggest data result yet: the complete NAMED + TYPED game.bin definition schema.

**RE (all evidence in FINDINGS.md 2026-07-19 sections):**
- **Def schema: 269 def types, 4,332 named fields (99%)** with types + game.bin serialization order.
  Method: batch-decompiled all 257 retail `Def::Transfer` (`DecompDefTransfers.java` ->
  `def_transfers.c`, 257/257 clean) for OFFSETS, then all 269 FableWin donor Transfers
  (`donor_def_transfers.c`, 269/269 clean) for NAMES + TYPES (donor persist helpers carry field-
  name string literals + type mangling: J=int32,K=uint32,M=float,_N=bool,VCCharString=string,
  W4Exxx=enum,TransferVectorOfSubComponents=vector). `tools/merge_def_schema.py` aligns them
  (serialization order = CALL order, NOT offset order) -> `ghidra_out/def_schema.json`. Validated
  exactly on CChestDef. Scripts: `tools/{parse_def_field_layouts,merge_def_schema}.py`,
  `tools/ghidra_scripts/DecompDefTransfers.java`.
- **Level-editor create/save recipe** (50 CEdit* fns decompiled, `leveleditor_decomp.c`, 50/50):
  new level = add a `CMapInfo` to the WLD world map at a 2D coord+bbox + LEV+TNG
  (`CreateAndSaveNewLevel` 0x0296D2D0); LEV write = `CMap::SaveToFile(file, CFileFormatLevel)`
  (`SaveSingleLevelToFiles` 0x0296E8B0); regions/navmesh = parallel vector<CRegion>/<CEditRegion>
  (`CreateNewRegion` 0x0296D920). Confirms new-level authoring is a known recipe.

**FableForge (head = `4844f9a`):**
- `d34feea` fse reader + `fse list|show` (931 fns).
- `cfc73f0` `forge::defschema` + `forge defs schema <schema.json> [def-type] [--json]`; vendored
  `docs/re_reference/def_schema.json`. (Fixed a Python truthy-`and` bug in the merge.)
- `c14b5a1` UNIFIED_SCRIPTING.md, `4844f9a` CAPABILITY_MAP.md - answer the user's vision questions
  (expand FSE via new natives not rebuild; full creation suite incl. meshes/anims via EgoCore's
  Blender pipeline, terrain/navmesh/new-levels via the level-editor RE). `ctest` green, validate CLEAN.

Next best tasks:
1. LEV writer (replicate CMap::SaveToFile/CFileFormatLevel) -> `forge lev write` -> terrain editing.
2. `forge level new` using the CreateAndSaveNewLevel recipe (CMapInfo + LEV + TNG + WLD register).
3. `forge::world` spine + payload DECODER: use def_schema.json to decode game.bin entry payloads
   into named field VALUES (needs per-type byte sizing of the persist format).
4. FQT node model -> forgecore; EgoCore mesh/anim Blender bridge; M4 GUI shell.

## Current resume point (2026-07-19, latest+10, authoritative) - MOD-PACK MERGE ENGINE proven

★ Built and proved a record-level mod merge/conflict system on THREE real 2GB+ overhaul mods
(Aeon Edition, Fable: The Lost Content, a third Modpack). This is the flagship FableForge feature.

**FableForge commits (head advanced ~15 commits this session):**
- `forge defs diff` / `forge wad diff` - record-level change sets.
- `forge defs merge <base> <out> <bin> <mod>...` - compose non-conflicting changes from N mods,
  resolve same-record conflicts by load order, write drop-in overlay. **Install-safe** (verified
  merged names.bin preserves existing offsets so vanilla script/frontend still resolve).
- `forge mods analyze <base> <mod>...` - read-only cross-mod conflict report over all def bins.
- `forge tng conflicts <base.tng> <mod.tng>...` - thing(UID)-level conflict analysis for levels.
- Proven: Aeon+LC 2-way = 3,136 applied / 53 conflicts (98.3% mergeable). 3-way (+Modpack) =
  3,967 applied / 238 conflicts / 17,722 entries, roundtrip clean, all 3 mods' unique content
  present. mods analyze: 4,076 changes / 242 conflicts across defs.
- Level-file conflict census (archive CRCs): defs 94% mergeable; TNG 182/401 whole-file conflicts
  (mostly thing-mergeable - modpack3 showed 0 thing-changes despite differing CRC); LEV 394/401
  (terrain, hardest). Docs: MOD_PACKS.md, LOAD_ORDER.md, CAPABILITY_MAP.md, FMP_FORMAT.md.

**RE this session (evidence in FINDINGS.md 2026-07-19 sections):**
- CGameScriptInterface = 891 native scripting methods (native counterpart to FSE's 931-fn API);
  FSE binds 790, **101 have no FSE binding** (candidate new bindings). Catalog:
  gamescriptinterface_catalog.tsv, native_methods_not_in_fse.txt. New tool DecompByName.java.
- game.bin field serialization FULLY cracked: per-field [4-byte name-hash tag][value] (proven
  via 3 CChestDef payloads + a 5-key chest). [UPDATED 2026-07-19: the hash is now solved — tag =
  **seed-0 reflected CRC-32(fieldName)**, LE; the "NOT standard" tests failed only because they used
  seed 0xFFFFFFFF/GetCRC. **#12 field-level merge is UNBLOCKED.** See FINDINGS.md "★ CANONICAL CRC
  REFERENCE" + "game.bin FIELD ENCODING FULLY CRACKED".]
- .fmp format RE'd from ControllerSupport.fmp sample: magic 0x42, footer section directory
  (GameBINEntries/ScriptBINEntries/FrontEndBIN/names/graphics/textures/effects/FinalAlbionWAD/STB),
  hash-keyed zlib entries = record-delta package. Spec: docs/FMP_FORMAT.md. Sample:
  scratchpad/fmp_sample/.
- Native controller stack exists (Xbox heritage): CInputManager, CJoystick::SetRumbleGloballyActive,
  EXboxControllerButton, CActionInputControl; CControlsDef is an editable game.bin def with the
  Controls binding table. => native remapping possible as a data edit (tasks #16/#17).

**Analysis artifacts:** scratchpad/{vanilla_baseline, aeon, lostcontent, modpack3, merged, merged3,
mod_analysis, fmp_sample, tngtest}. vanilla_baseline is the clean diff reference (Steam-revalidated).

**Merge coverage now DONE for defs + TNG:** `forge tng merge <base> <out> <mod>...` +
`forge::tng::replaceThing` (in-place, unit tested). Proven on banditcampboss.tng (3 mods):
280 base -> 325 things, 637 changes, 312 conflicts, merged TNG round-trips clean. `forge tng
conflicts` is the read-only analyzer.

**Task list:** #12 field-level decoder (UNBLOCKED — hash solved, tag = seed-0 CRC-32), #13 .fmp reader (format RE'd),
#14 text.big reader, #15 entity-script editing, #16/#17 controller RE, #9 freeroam. (#18 DONE.)

Next best tasks:
1. Top-level `forge mods merge` = game.bin (record) + TNG level files (thing-level) + stage
   installable, in one command. (Multi-bin script/frontend share names.bin - needs shared-names
   handling; game.bin alone is install-safe, verified.)
2. Crack the engine string-hash (decompile the CPersistContext binary Transfer/hash) -> unblocks
   field-level def merge (#12) AND .fmp hash->name (#13).
3. Build `forge fmp list` from the RE'd format (#13); ingest community mods as change sets.

## Resume point (2026-07-19, latest+11) - STRING-HASH [SUPERSEDED by latest+12 below]

> **⚠ SUPERSEDED — do not act on this block.** The seed here is WRONG: the game.bin
> field tag is **seed-0** reflected CRC-32, NOT `CCharString::GetCRC()` (seed
> `0xFFFFFFFF`, which is the runtime save-stream variant). See the "★ CANONICAL CRC
> REFERENCE" in FINDINGS.md and the later "game.bin FIELD ENCODING FULLY CRACKED"
> resume point in this file's tail. Kept for history only.

★ Identified the engine string-hash (a standing open question): the game.bin per-field 4-byte tag
is **CRC(fieldName)** via `CCharString::GetCRC()` = `CCRC::Calc(0xFFFFFFFF, chars, byteLen)`. Proven
by the donor `CPersistContext::CheckCRC` assert string "stream_crc==CCharString(name).GetCRC()".
Non-standard CRC (seed 0xFFFFFFFF; plain crc32 fails). Full evidence + all donor addresses in
FINDINGS.md "THE ENGINE STRING-HASH IDENTIFIED" (2026-07-19). Next: decompile `CCRC::Calc` (donor
0x0186d2d0) for the exact table/polynomial, OR build the name->CRC table empirically. Logs:
ghidra_out/decomp_persist_helpers.log, decomp_getcrc.log. New tool: DecompByName.java.

★ FLAGSHIP: FableForge mod-pack merge system SHIPPED + proven on 3 real 2GB+ overhaul mods
(Aeon Edition, Fable: The Lost Content, a third Modpack). Commands (all in `forge`, ~20 commits
this session, `ctest` green, `forge validate` CLEAN):
- `forge defs diff <a> <b> [bin] [--json]` - record-level def change set.
- `forge wad diff <a.wad> <b.wad> [--json] [--deep]` - WAD entry change set.
- `forge defs merge <base> <out> <bin> <mod>...` - compose N mods' record changes, load-order
  conflict resolution, install-safe drop-in overlay (verified: merged names.bin preserves offsets).
- `forge mods analyze <base> <mod>...` - read-only cross-mod conflict report over all def bins.
- `forge tng conflicts <base.tng> <mod.tng>...` - thing(UID)-level conflict analysis.
- `forge tng merge <base.tng> <out.tng> <mod.tng>...` - thing-level level merge (new
  forge::tng::replaceThing, unit tested).
Proven numbers: defs 3-way = 3,967 applied / 238 conflicts / 17,722 entries, roundtrip clean, all
3 mods' content present (94% auto-merge). TNG banditcampboss = 280->325 things, 637 changes, 312
conflicts, roundtrips. Level-file census (archive CRCs): defs 94% mergeable, TNG 182/401 whole-
file-conflicting (mostly thing-mergeable), LEV 394/401 (terrain hardest). Docs: MOD_PACKS.md,
LOAD_ORDER.md, CAPABILITY_MAP.md, FMP_FORMAT.md, UNIFIED_SCRIPTING.md.

Other RE this session (FINDINGS.md 2026-07-19): CGameScriptInterface = 891 native scripting methods
(101 not in FSE = candidate bindings; gamescriptinterface_catalog.tsv). .fmp format RE'd from
ControllerSupport.fmp (magic 0x42, footer section directory GameBINEntries/ScriptBINEntries/.../
FinalAlbionWAD/STB, hash-keyed zlib entries; FMP_FORMAT.md). Native controller stack exists
(CInputManager/CJoystick/EXboxControllerButton/CControlsDef editable def) - remapping as data edit.
Level-editor create/save recipe (CreateAndSaveNewLevel/SaveSingleLevelToFiles/CMap::SaveToFile).
Full named+typed def schema (269 types, 4,332 fields; docs/re_reference/def_schema.json).

Analysis artifacts (scratchpad, NOT git): vanilla_baseline (clean diff ref, Steam-revalidated),
aeon, lostcontent, modpack3, merged, merged3, mod_analysis/, fmp_sample/, tngtest/. Downloaded mods
in D:\Downloads (Aeon .zip, TLContent .rar, Modpack .zip, ControllerSupport .rar with .fmp+.dll).
7-Zip at C:\Programs\7-Zip\7z.exe.

Open tasks: #12 field-level merge (unblocked - use def_schema + [tag][value] split; per-type value
sizes: int32/uint32/float=4B, bool=1B, string/vector via Transfer<CCharString>/TransferVector
decompile at donor 0x0189c530/0x01894de0), #13 .fmp reader (format RE'd), #14 text.big reader, #15
entity-script editing (TNG ScriptName/ScriptData + 891-method catalog), #16/#17 controller RE, #9
freeroam. #10/#11/#18 DONE.

FableForge is a git repo (D:\Code\FableForge); commit after each feature. FableTLC (D:\Documents\
FableTLC) is NOT git - docs/scripts/logs updated on disk.

## Current resume point (2026-07-19, authoritative) - script-binding candidates spec'd
The 101 native CGameScriptInterface methods with no FSE binding are now a prioritized, cited
spec: `docs/SCRIPT_BINDINGS_CANDIDATES.md`. All 101/101 resolve to a retail address (from
`gamescriptinterface_catalog.tsv`) and are cross-checked against `refs/fse_api_manifest.json`.
Key finding: ~18 are VARIANT DUPLICATES already reachable in Lua under a typed/suffixed FSE name
(e.g. native `EntitySetAsOpinionSource` -> FSE `...ByInt/ByString`; `RadialBlurFadeTo` ->
`_NoPos/_WithPos`; `SetIsGossipForPlayer` -> `_ByName/_ByObject`). Truly-novel, high-value
Tier-A verbs to bind first: HeroGoFishing (0x895a90), HeroGoDigging/HeroStopDigging (0x8df80/
0x8dfa0), HeroPlayOracle/FireheartMinigame (0x895b60/0x895c90), SetHouseOwnedByPlayer (0x895ed0),
JamDoor (0x895e10), Open/CloseHouseDoors, AddLogBookEntry (0x8fe00). 7 candidates flagged
"needs decomp" for exact args (CreateEffect, GetAllThingsInLevel, conversation builder,
house/door bool args, AddLogBookEntry, UpdateOnlineScore_* XBL-noop check) - next step is
DecompByName.java on those addresses when the Ghidra lock frees. Xbox-Live UpdateOnlineScore_*
and IsXbox/Validate/Error/TraceMessage are Tier-D (no-op/internal). Input-poll methods
(IsPlayerHolding*, IsDPadButton*) deferred to the controller subsystem (#16/#17).

## Current resume point (2026-07-19, latest+12, authoritative) - CONTROLLER/INPUT RE (#16/#17)

Ran alongside the persist/field-merge/.fmp/FableForge agent WITHOUT touching that work (stayed
strictly in the input subsystem). Tasks #16/#17 (native controller/input RE) are now mapped and
cited (full block in FINDINGS.md "CONTROLLER / INPUT SUBSYSTEM RE (tasks #16/#17)"; subsystem
entry in SYSTEMS_ANALYSIS.md "Controller / Input System").

Result: **key remapping is a pure game.bin data edit; controller-motor rumble is dead code on
retail PC; screen-quake rumble is a data edit.**

- `CControlsDef` (retail `Transfer` `0x004330F0`): `Controls` vector @ retail +0x34 of 28-byte
  `CActionInputControl` records + 6 toggle bools (+0x48..+0x4D). Record layout: +0x00 EGameAction,
  +0x04 EControllerType (1=xbox,2=kbd,3=mouse), +0x08 EInputKey, +0x0C EXboxControllerButton,
  +0x10 EMouseButtonControl, +0x14/+0x18 dir vector. Tagged union on ControllerType. Proven via
  donor `CPersistTraits<CActionInputControl>::TransferOut` (0x017E149A) + `IsSameButton`
  (0x01848591) + 7-dword element stride. Donor->retail offset delta is -0x10.
- Rumble: retail `CJoystickDX` motor rumble vtable stubbed (AddRumble/ClearAllRumbles nop,
  Get/PeekRumbleWithID/GetJoystickDeviceNumber ret 0, 0x00AB7900-0x00AB7940); donor AddRumble
  also nop. `SetRumbleGloballyActive` = static writing one gate byte (donor DAT_04a67e31).
  `CInitRumble` (retail 0x0070C050) = 4 floats + bool. Live PC rumble = screen quake via
  `CreateRumble` (0x0089FDA0) + `CRumbleDef` (Transfer 0x004E676A: QuakeIntensities +0x28 /
  QuakeDurations +0x34, both map<enum,float>). ERumbleType = NULL/WILL/DAMAGED/HITTING/QUAKE.

Evidence: `ghidra_out/decomp_controller_donor.log` (684 fns), `decomp_controller_retail.log` (87),
`decomp_controller_retail2.log` (13 Rumble), `decomp_rumbledef_donor.log` (41). def_schema.json
CControlsDef/CRumbleDef confirmed.

REMAINING GAP (not resolved; 2 scan attempts failed, stopped per loop-prevention): the exact
integer->name tables for `EXboxControllerButton` / `EGameAction` / `EInputKey` / `EControllerType`.
Retail strips these strings and the controls headers are absent from the extracted DevHeaders
(`header_enums.csv` has ERumbleType only). To finish a turnkey remap UI: recover from
controls_def.hpp/inputkey.h headers, dump a real game.bin CControlsDef payload and correlate with
in-game bindings, or use the fabletlcmod.com wiki controls page.

Next best tasks (input subsystem):
1. Recover the input enum value tables (above) -> then FableForge can label binding button ids.
2. Locate the CControlsDef entry in compiled_game_entries.csv and decode a real Controls payload
   (uses the already-cracked [tag=CRC(fieldName)][value] format; tag = seed-0 reflected CRC-32 of
   "Controls" — NOT GetCRC; see FINDINGS.md "★ CANONICAL CRC REFERENCE").
3. Optional: prototype a native XInput AddRumble patch if motor rumble is ever wanted on PC.

## Current resume point (2026-07-19) - text.big localization format CRACKED (task #14 DONE)

The BIGB localization bank format (`text.big`, `dialogue.big`, and the shared `.big` container for
graphics/textures/frontend/effects/fonts/shaders) is fully RE'd, VALIDATED against the real retail
file, AND confirmed in the retail engine loader. Full spec: `docs/TEXTBIG_FORMAT.md`; cited summary
in `docs/FINDINGS.md` (2026-07-19 entry).

- **Validated:** parsed all 28,913 entries of `data\lang\English\text.big` into coherent English
  (identifiers, speakers, speech banks, tags, groups, narrator list). Parser:
  `scratchpad/parse_textbig.py`. Type histogram matches the file's own stats header exactly.
- **Three sources agree:** raw bytes; EgoCore MIT loader (`Banks\BankLoader.h`,
  `Text\TextParser.h`); retail `Fable.exe` (`NGameText::CDataBank`/`CBankFile`).
- **Loader addresses (retail):** `text.big` str @0x0122da24; `NLocalisation::GetLanguage`
  @0x00415070; `GetTextInfo` @0x009c7d00 (Type0=string, ID-indexed); `GetTextEntryFromGroup`
  @0x009c9280 (Type1=group, weighted random); `GetTextBySymbol` @0x009c95e0
  (id = FUN_009cc410(CCRC::Calc(0,name,len)) — symbol names CRC'd, **seed 0**: the SAME seed-0 CRC
  the game.bin field-tags use; only CCharString::GetCRC (save streams) seeds 0xFFFFFFFF). Logs: `ghidra_out/decomp_localisation.c`,
  `decomp_textbig_loader.c`, `decomp_textbig_strxref.log`, `decomp_textbig_bankfile.log`.

REMAINING GAP: byte-exact **.big WRITE round-trip not yet proven** (only reading validated). TOC
`CRC`(+0x14)/`Timestamp` fields are [hypothesis] (EgoCore struct names; not recomputed; unneeded for
reads). `FUN_009cc410` (CRC->entry-index map) and `CCRC::Calc` table/polynomial not decompiled in
detail (same open item as #12/#13).

Next best tasks (text/localization):
1. FableForge `forge::text` reader over `docs/TEXTBIG_FORMAT.md` (Stage-A dialogue/text editor);
   then prove a byte-exact no-op `.big` repack before adding a writer.
2. Decompile `CCRC::Calc` + `FUN_009cc410` to nail the symbol->ID CRC map (also unblocks #12/#13).
3. Wire `dialogue.big` (LIPSYNC_* sub-bank) + `.lut`/`.lug` speech-bank cross-ref for the media panel.

## Current resume point (2026-07-19, latest+12, authoritative) - game.bin FIELD ENCODING CRACKED + native decoder

★ Cracked and corpus-validated the game.bin per-field encoding (the long-open
"engine string-hash" question), correcting a wrong retraction from latest+11.

**RE result (full writeup: FINDINGS.md "game.bin FIELD ENCODING FULLY CRACKED"):**
- Field tag = **reflected CRC-32(fieldName), seed 0, NO final inversion**, name
  as-is, little-endian. NOT `CCharString::GetCRC()` (seed 0xFFFFFFFF) - that seed
  difference is exactly why every earlier crc32/fnv/djb2/GetCRC test failed.
  Proven: all six CChestDef field names reproduce their observed payload tags,
  including the two latest+11 called "unexplained groups".
- Correcting latest+11: `CPersistContext::CheckCRC` writes/checks the CRC in the
  BINARY modes (2/3); TEXT modes (0/1) return early. So `[tag][value]` IS the
  game.bin binary format - the opposite of the retracted note.
- Layout: `[variable untagged base-class prefix][field]*`; prefix 3 bytes for most
  types, 5 for some, larger for a few (ends at the first named field's tag).
  Field = `[4-byte CRC-LE tag][value]`. Value sizes: int32/uint32/float/enum/
  CDefIndex=4B, bool=1B, CCharString=null-terminated (empty=0x00), Vector_<T>=
  [u32 count][T x n]. Serialization order == def_schema (Transfer call) order.
- Validation: naive fixed decoder byte-exact on 85 types (leftover 0); resync
  decoder matches **145/145** schema'd types present in game.bin - every field
  tag, in order, zero misses. Scripts: scratchpad validate_defs.py / validate2.py.
- Reference impl (C port contract): `tools/decode_game_bin_field.py`
  (`field_tag()` + `decode()`, full spec in docstring).

**FableForge (commit `b2005b7`, on top of `f168785`):**
- New `forge::defdecode` (fieldTag + type-tolerant decode: each field value =
  bytes between its tag and the next field's tag, so every named field is
  recovered even across value types not sized explicitly).
- CLI: `forge defs decode <game-root> <schema.json> <name-or-index> [bin] [--json]`.
  E.g. `defs decode <root> docs/re_reference/def_schema.json 12266 game.bin` ->
  the 5-key chest with OpenersRequired=5, OpenerObject=4313. Minigame strings,
  reward vectors, and the 5-byte-prefix CAreaOfEffectAttackDef all decode clean.
- New unit test `testDefDecode` locks the hash constants + decoder. `ctest` green,
  `forge validate` CLEAN.
- NOTE: a prior session's incomplete `.fmp` reader stub (`libs/forgecore/{include/
  forge/fmp.hpp,src/fmp.cpp}`) is left UNTRACKED and UN-WIRED (it advertised a
  `forge fmp list` command with no handler and a throwing getEntryData). Build a
  proper `.fmp` reader on the now-cracked CRC family; see docs/FMP_FORMAT.md.

Next best tasks:
1. **Field-level def merge (#12), now fully unblocked:** use `forge::defdecode` to
   split same-def payloads into per-field byte ranges (tag = field identity) and
   merge field-by-field (same tag, differing value = changed field). Extend
   `forge defs merge` / `mods merge` from record-level to field-level.
2. Extend the decoder's value typing to the exotic types seen in the corpus
   (C3DVector, CRGBColour, CDefString, Map_*, vector-of-struct like
   CObjectFamilyEntry) so `defs decode` renders them, and characterize the 3-vs-5
   byte prefix rule (which base classes emit the extra 2 bytes).
3. Build the real `.fmp` reader (#13) on the cracked CRC (hash->entry-name), then
   ingest community .fmp mods as change sets; wire into `forge mods`.
4. `forge defs decode --all` corpus self-check (decode every entry, report
   leftover/mismatch) as a native regression guard for the schema + decoder.

## Current resume point (2026-07-19, latest+13, authoritative) - FIELD-LEVEL DEF MERGE shipped

Extended `forge defs merge` from record-level to **field-level** using the game.bin
decoder (latest+12). FableForge commit `42c6dd2` (on `b2005b7`).

**What it does:** when two mods change the same def record, record-level merge
picked one whole record and dropped the other's edits. Field-level decodes both
against def_schema and composes PER FIELD - a field only one mod changed (or both
changed identically) auto-merges; only same-field/differing edits are true
conflicts (load order, or `--picks`).
- CLI: `forge defs merge <base> <out> <bin> <mod>... --fields <schema.json> [--picks f]`
- `forge::defdecode` gained `encode()` (exact round-trip of a clean decode),
  `mergeFields()` (3-way per-part compose incl. the untagged prefix), and
  `resolveType()` (bin `Entry.definition` -> schema DefType: literal class name,
  else `C`+CamelCase(CATEGORY)+`Def`, CONFIRMED by decoding the sample clean so a
  wrong guess never merges). `forge defs decode` uses `resolveType` too.
- Unit tests (`testFieldMerge`, `testDefDecode`): round-trip, disjoint-field
  auto-merge (both edits kept), same-field conflict (load order), pick override,
  vanilla pick. `ctest` green, `forge validate` CLEAN.

**Proven on real mods** (Aeon x Lost Content over vanilla install, game.bin):
53 record conflicts -> **39 field-merged** (15 fields auto-merged that record-level
would have lost, 30 residual field conflicts) + 14 safe whole-record fallbacks.
Verified: `INVENTORY_CATEGORY_*` NumberOfSlots auto-merged to the agreed value;
`WASPS_05` Creatures conflict took the load-order winner. Mod roots re-extractable
from `D:\Downloads\{Aeon .zip, TLContent .rar, Modpack .zip}` via 7-Zip (only
`data/CompiledDefs/{game,names}.bin` needed).

**Coverage limits (next work):**
1. ~100 category-named def types resolve, but some (OBJECT=2858 entries, UI=2537)
   have NO schema class at all (e.g. CObjectDef is absent from def_schema.json) -
   those still fall back to whole-record. Extend def_schema.json coverage
   (decompile the missing base-class Transfers: CObjectDef, CUIDef, CThingDef...)
   to widen field-merge. Also a few entries (e.g. CREATURE_HERO) don't decode
   clean and safely fall back - investigate per-type.
2. Value typing in `defdecode::formatValue` still renders exotic types
   (C3DVector/CRGBColour/Map_*/vector-of-struct) as hex; add sizing for display.
3. Wire `--fields` into the top-level `forge mods merge` multi-bin flow, and into
   the GUI per-conflict picker (field-level rows).
4. Build the real `.fmp` reader (#13) on the cracked CRC; the un-wired
   `libs/forgecore/{include/forge/fmp.hpp,src/fmp.cpp}` stub is still untracked.

## Current resume point (2026-07-19, latest+14, authoritative) - mod ingestion: bsdiff + .fmp

Two community mod-format ingestion paths shipped in FableForge (running total this
day: field-level def merge, then these). Another agent is doing Ghidra decomp
(savegame/persist primitives) - stay off Ghidra; that agent's `crc_table_full.txt`
independently CONFIRMED the game.bin field hash = CRC seed 0 (also used by text.big).

**FableForge commits (head `d7e25e6`+docs):**
- `4fea785` **bsdiff `.patch`**: `forge::bunzip` (self-contained decompress-only
  bzip2, validated vs mingw `bzip2` on 6 inputs) + `forge::bspatch` (BSDIFF40).
  `forge patch info|apply`. Proven: applying the Unofficial Fable Patch's
  `game.bin.patch` to the vanilla install yields a valid bin (14761/14761 parse);
  `forge defs diff` recovers its 7 changed defs (incl. MAIN_HERO_STATS).
- `d7e25e6` **.fmp reader**: `forge::big` (Lionhead BIG/BIGB container) +
  `forge fmp list [--json]`. **The .fmp is a BIGFile** - solved from decompiled
  ChocolateBox source (`FableMod.ContentManagement/ModPackage.cs` +
  `SilverChest.Formats.Big/BigReader.cs`), validated zero-slack on two samples
  (ControllerSupport.fmp, HalsSword.fmp). Each entry = name + def-type (subHeader)
  + raw payload, mapping onto game.bin `(name, definition)` -> feeds the merge
  engine. FMP_FORMAT.md updated (it wrongly said "hash, never plaintext").

**Key resource (answers "does the decompiled source help"): YES.**
`D:\Code\Fable\ChocolateBox\ChocolateBox\DecompiledDLLs\` has decompiled FableMod.*
(BIG/BIN/TNG/LEV/WLD/STB/Script/ContentManagement) and `src/SilverChest.Formats.*`
clean readers - authoritative format references. `.fmp` samples:
`ExampleFMPFilesForDev/HalsSword.fmp` + `D:\Downloads\ControllerSupportFMP-...rar`.

**Mods on disk for merge/ingest tests** (`D:\Downloads`, 7z at C:\Programs\7-Zip):
Aeon, TLContent, Modpack (whole-file overhauls, extracted to session scratchpad),
Unofficial Fable Patch (+hotfix; bsdiff `.patch`), ArenaRevisited (`FinalAlbion.qst`
quest mod), FableTECv1 + Graphics (not yet examined), ControllerSupport (.fmp+.dll).

Next best tasks:
1. `forge fmp extract`/`apply` - inflate/emit entry payloads; feed `.fmp`
   GameBINEntries into `forge defs merge` (they're already (name, def) records).
2. `forge patch create` (bsdiff diff) for emitting patches; and wire `.patch`/`.fmp`
   ingest into a top-level `forge mods merge`.
3. Widen field-merge coverage via `DEF_SCHEMA_COVERAGE.md` (needs decomp agent to
   add CObjectDef/CUiDef/CBuildingDef/CThingDef + CCombatAbilityBlock* Transfers).

## Current resume point (2026-07-19, latest+15, authoritative) - UNIFIED MOD INGEST/MERGE

Built the task-listed ingestion pipeline: every community mod format now flows
through ONE field-level game.bin merge. FableForge head advanced ~5 commits.
(Decomp still owned by the other agent - stayed off Ghidra.)

**Shipped (all `ctest` green, `forge validate` CLEAN, real-mod validated):**
- `forge fmp apply <base> <mod.fmp> <out-root>` - .fmp GameBINEntries -> drop-in
  game-root (replace by name, else add). HalsSword.fmp -> 5 added; ControllerSupport
  -> 4 replaced + 1 added. (Sibling script/frontend copied; *LinkMetaData / names.bin
  fixups + script/frontend BIN apply are follow-ups - shared names.bin.)
- `forge fmp extract <mod.fmp> <dir> [bank]` - dump entry payloads by bank
  (HalsSword -> 17: defs/mesh/textures/text).
- `forge mods merge <base> <out> --with <dir|.fmp|.patch>... [--fields schema]
  [--stage]` - normalizes each source (dir=as-is, .fmp=fmp apply, .patch=bspatch)
  then field-level merges all in load order. **Proven** on HalsSword.fmp +
  Unofficial-Patch game.bin.patch + Modpack overlay: 836 changes / 591 new records,
  cross-format field conflict (DEFAULT_GLOBAL_DEFS.AgeIncreasePerLevelUp: patch vs
  modpack -> load-order winner), output 15,352 entries, round-trips clean, contains
  all three sources' content.

Commits: `fmp apply`, `fmp extract`, `mods merge`, plus earlier `bsdiff .patch`
(`forge::bunzip`/`bspatch`) and `.fmp` BIG reader (`forge::big`). Format truth from
decompiled ChocolateBox (`DecompiledDLLs/FableMod.*`, `src/SilverChest.Formats.*`).

Next best tasks:
1. Fold TNG thing-level merge (already have `forge tng merge`) into `mods merge`
   so level files merge in the same pass as defs.
2. `.fmp` link fixups: apply GameBINLinkMetaData + merge the `names` bank so a
   single `.fmp` produces a directly-playable install (not just a merge source).
3. `.fmp`/`.patch` EXPORT (`forge fmp export`, `forge patch create` via bsdiff diff)
   so FableForge output loads in Fable Explorer / distributes as patches.
4. Widen field-merge coverage - needs the decomp agent to add CObjectDef/CUiDef/
   CBuildingDef/CThingDef + CCombatAbilityBlock* Transfers (`DEF_SCHEMA_COVERAGE.md`).

## Current resume point (2026-07-19, latest+16, authoritative) - .fmp EXPORT + TNG in mods merge

Extended the mod pipeline both ways (produce, not just consume) and made
`mods merge` handle level files. (Still off Ghidra - other agent owns decomp.)

**FableForge shipped (ctest green, forge validate CLEAN, real-mod validated):**
- **BIG writer** (`forge::big::serialize`): both real .fmp samples re-serialize
  **byte-identical**; reader now captures every entry field. `addBank`/scratch
  build supported. Hidden `fmp _rewrite` for validation.
- **`forge fmp export <base-root> <modded-root> <out.fmp>`**: game.bin diff → .fmp
  (16-bank skeleton, GameBINEntries records). Round-trip PROVEN: export from
  hals_root → fmp apply onto vanilla → defs diff = 0. (Link/names gen for
  Fable-Explorer parity = follow-up.)
- **`forge fmp apply/extract`** (from previous session): .fmp → drop-in root /
  per-bank payload dump.
- **TNG thing-merge folded into `forge mods merge`**: one pass now merges game.bin
  (field-level) AND loose level TNGs (data/Levels/**/*.tng from dir sources;
  per level: 1 editor→copy, ≥2→thing-merge by UID). Proven on Aeon + Lost Content:
  BanditCampBoss.tng thing-merged (base 308 → 325 things, 32 conflicts, roundtrips
  clean) in the same run as the game.bin field merge. `mergeTngFiles()` helper
  shared with `forge tng merge`.

Note: base install's BanditCampBoss.tng == Modpack's byte-for-byte (Modpack ships
some vanilla level files unchanged) — so Aeon+Modpack gave a single-editor copy;
Aeon+LostContent gave the real 2-way thing-merge. Test TNGs extracted into
scratchpad/mods/{aeon,modpack,lostcontent}/data/Levels/... via 7-Zip.

Next best tasks:
1. `.fmp`/`.patch` level data into the TNG merge: extract TNGs from an .fmp's
   FinalAlbionWAD bank (and a wad.patch) so those formats also thing-merge levels.
2. `.fmp` link fixups (GameBINLinkMetaData + `names` bank) so a single exported/
   applied .fmp is a directly-playable install and loads in Fable Explorer.
3. LEV terrain merge (hardest; grid-level) into `mods merge`.
4. Widen field-merge coverage - needs decomp agent to add CObjectDef/CUiDef/
   CBuildingDef/CThingDef + CCombatAbilityBlock* Transfers (DEF_SCHEMA_COVERAGE.md).

## Current resume point (2026-07-19, latest+17, authoritative) - FIELD-MERGE COVERAGE 100%

Folded the decomp agent's schema-gap handoff into FableForge: field-level def merge
now covers **249/249 game.bin def types / all 14,761 entries** (was 196/249).

**What I did (FableForge, no Ghidra — decomp agent owns that lane):**
- Merged `docs/re_reference/def_schema_missing.json` (58 add + 1 replace) into the
  vendored `def_schema.json` (269 → 327 types). Only overlap: `CCombatAbilityBlock
  DefBase` (0 → 5 fields, intended replace). No `resolveType` code change — the new
  entries are keyed by literal definition string (Candidate-1).
- Key insight from the decomp agent: category types are the **Thing hierarchy**
  (`OBJECT`→`CThingObjectDef`, `BUILDING`→`CThingBuildingDef`, `THING`→
  `CThingBaseDef`), + the new **UI family** (`CUIDef` 109 fields etc.). The large
  untagged record prefix = inherited `CThingBaseDef` base fields.
- Validated: OBJECT/UI/BUILDING/THING now `forge defs decode` clean (leftover 0).
  Aeon+LostContent merge: whole-record fallbacks 14 → 10 (rest are genuine add-add),
  field-merged 39 → 43, auto-merged fields 15 → 18 (now incl. INVENTORY_*, REWARD_*,
  OBJECT_*). ctest green, forge validate CLEAN.
- CRC confirmed seed-0 everywhere (my `fieldTag` already correct); see the decomp
  agent's "★ CANONICAL CRC REFERENCE" consolidation in FINDINGS.md.
- Docs: FableTLC DEF_SCHEMA_COVERAGE.md rewritten (CLOSED/100%); FableForge
  CAPABILITY_MAP.md updated.

Commits (FableForge): schema fold-in + coverage-doc + the earlier .fmp export/BIG
writer + TNG-in-mods-merge from this session.

Big picture — the decomp agent (separate terminal) cracked a LOT in parallel this
session (save format + editor, full .big family read/write, .lut audio, text.big,
controller/input, script "VM" = text command lines, 101 script bindings). Those are
new FableForge feature surfaces to consume next.

Next best tasks (FableForge, consuming the decomp outputs):
1. `forge::bin`/`defschema` native game.bin/names.bin already exist — nothing needed
   for the schema; consider a `forge defs decode --all` corpus regression that
   asserts 249/249 clean as a guard against schema drift.
2. Save editor: consume the cracked FableSave! format + HEADER/hero-stat maps
   (SAVEGAME_FORMAT.md, SAVE_WRITER.md, hero-entity map) -> `forge save` read/edit.
3. `.big` asset read/write (retexture/mesh-swap at container level is GO per the
   decomp agent; inner Lionhead-LZ codec still being cracked) -> `forge big`.
4. `.fmp` GameBINLinkMetaData/names fixups + WAD-bank TNGs into mods merge (still open
   from latest+16).

## Correction (2026-07-19, to latest+17): field-merge coverage is 86%, not 100%

The latest+17 "100%" was NAME-resolution only. `forge defs decode --all` (new
corpus guard) shows true CLEAN-DECODE = **86.19% (12,723/14,761 entries)**.
def_schema_missing.json closed OBJECT/BUILDING/THING/UI, but ~28 derived
Thing-hierarchy types still resolve to a wrong/base CamelCase class and fall back to
whole-record merge: `CREATURE` (517, matches only CThingBaseDef's base fields),
`CPhysicsDef` (315), `CTargetingDef` (254), `CWeaponDef` (190), `THING_GROUP`,
`CShopDef`, `CBuyableHouseDef`, `CAMERA_MODE`, ... (full list + counts:
docs/DEF_SCHEMA_COVERAGE.md). These need their FULL derived-class field set added to
def_schema.json keyed by the literal definition string (same fix the decomp agent
applied to OBJECT->CThingObjectDef). Field-merge still WORKS for the 14% via
whole-record fallback; it's just not field-level for them.

Guard: `forge defs decode <root> docs/re_reference/def_schema.json --all game.bin`
(exits non-zero while any entry is unresolved/dirty).

## Current resume point (2026-07-19, latest+18, authoritative) - native save reader + hero-stat flag

Consumed the decomp agent's save-format crack into FableForge: `forge save read`.

**Shipped (FableForge, no Ghidra):**
- `forge::save` + `forge save read <FableSave-file> [--json]` — parses the
  FableSave! container (header + zlib chunks via miniz) and decodes the validated
  **23-field HEADER** ([seed-0 CRC tag][value]; string/wstring/int32/float/bool/
  range3), reusing `forge::defdecode::fieldTag`. Read-only. Validated on the user's
  real saves: WorldName=FinalAlbion, CurrentRegionName=TXT_REGION_GREATWOOD_*,
  SaveGameMarkerPos, save name, all gameplay flags — **all 23 tags verified**.
  Synthetic unit test added; ctest green, validate CLEAN.

**★ FLAG for the decomp agent — hero-stat claim does NOT reproduce:**
The decomp agent's "hero-entity stat map" reported Money=1625 (tag b03ccbfd),
Morality=483 (79a2d479) editable in Cornelio's save. On disk that does NOT hold:
- I verified the tags ARE correct seed-0 CRCs (Money→b03ccbfd, Morality→79a2d479).
- But a flat search of the inflated ENTITIES chunk of BOTH `Cornelio/Manual - Save1`
  and `Cornelio/AutoSave` finds NEITHER tag.
- The entity chunk inflates fine (has ENTITIES/SAVED_ENTITIES/PLAYER/faction
  strings) but is **mixed binary + TEXT-mode** persist — e.g. `Name
  "FACTION_RIVAL_HERO";` (text syntax, no CRC tag). So hero stats are nested inside
  the entity/component graph and/or text-encoded, NOT flat `[seed-0 tag][value]`.
So the save WRITER / hero-stat editor is NOT ready — the decomp agent should
re-verify the hero-stat map against real Cornelio saves and characterize the
entity-graph (mixed binary/text) structure. `forge save read` is the container
foundation an editor builds on once that's mapped.

Test saves copied read-only to scratchpad/saves/{manual1,auto}.sav (real saves at
`My Games\Fable\Saves\Cornelio\` were NOT modified).

Next best tasks:
1. (decomp) re-verify hero-stat map + map the entity-graph (mixed binary/text)
   structure -> then FableForge `forge save edit`.
2. `forge save read` for `.qs`/`.qs.hs` companion saves; Profile.bin (FablePro text).
3. Consume the .big family: `forge big list/extract` for retail graphics/text.big
   (container read is GO; inner Lionhead-LZ codec still being cracked by decomp).
4. Close the field-merge 14% via the decomp agent's derived-class schemas
   (DEF_SCHEMA_COVERAGE.md gap list).

## Resume point (2026-07-19, latest+19) — HERO-STAT MAP RE-VERIFIED (corrects latest+18)

★ The latest+18 flag ("hero-stat claim doesn't reproduce; Money/Morality tags not found in
the ENTITIES chunk") was a **flat-search artifact, one zlib layer short** — the hero-stat map
in docs/SAVE_HERO_STATS.md is CORRECT. Re-reproduced directly on the user's real save
(Cornelio/Manual - Save1):

- A flat tag search of the ONCE-inflated chunk1 (90,544 B) finds NONE of the tags — this is
  what `forge save read` / the flat search saw.
- The hero stats live in a **NESTED zlib cell**: chunk1's inflated `SAVED_ENTITIES` holds 25
  independently zlib-compressed cells (78 xx). Inflating all 25 → 383,284 B; **cell #10 contains
  `CTCHeroStats`** and all the tags. Read in the ~0x140-byte window right after `CTCHeroStats\0`:
    - `Money` (b03ccbfd) i32 = **1625**   `Morality` (79a2d479) i32 = **483**
    - `Age` (663c8f40) f32 = **26.4**      `RenownLevel` (0c4ed775) i32 = **2**
  All exactly match SAVE_HERO_STATS.md. PlayerCreature/CREATURE_HERO/CTCHeroStats counts = 1/1/1.

**Action for the FableForge side:** `forge save read` currently inflates chunk1 once and flat-
searches → it will never see hero stats. To expose gold/morality it must (1) find
`SAVED_ENTITIES` in the inflated chunk1, (2) inflate each nested `78 xx` cell, (3) pick the cell
containing `CTCHeroStats`, (4) read tags in the bounded window after it (seed-0 CRC, same as
game.bin field tags). Then correct CAPABILITY_MAP's "hero stats not flat-decodable" note — they
ARE decodable, just one zlib layer deeper. The save-EDIT path (round-trip re-sign) was already
PoC-validated; the only true unknown remains whether retail enforces the trailer signature on
load (a live-fire test, not a format gap).
Verification: pure-Python repro on a read-only save copy; no Ghidra, no real-save writes.

## Current resume point (2026-07-19, latest+19, authoritative) - retail .big asset banks

`forge big list/extract` — browse/extract every retail `.big` asset bank, reusing
`forge::big` (the BIGB reader built for .fmp already handles the retail container).
- `forge big list <file.big> [name-filter] [--json]` (capped preview or name-filter
  so 28k-entry banks don't flood); `forge big extract <file.big> <dir> [bank]`.
- Validated on all 8 retail banks; counts match the decomp agent's
  BIG_FAMILY_FORMATS.md: text.big 28,913 (1 bank), dialogue.big 20,505 (4 banks;
  LIPSYNC 12,134), fonts.big 26 (3), textures.big 6,324 (2, 509MB read in 1.4s),
  graphics/effects/shaders/frontend all read. ctest green.
- Payloads extract RAW; the inner Lionhead-LZ codec (texture pixels / mesh vertex
  buffers) is still being cracked by the decomp agent, so texture/mesh AUTHORING
  isn't ready — but container browse/extract + whole-entry swap are GO.

FableForge now consumes, in one tool, every format the decomp agent cracked this
day at the container level: game.bin (field-merge 86%), TNG, LEV, WLD, STB, WAD,
.fmp (read/write/merge), .patch (bsdiff), save (HEADER read), .big (all banks).

Next best tasks:
1. `forge text` — decode text.big string payloads (UTF-16 + speech/speaker/tag
   metadata; symbol→ID = seed-0 CRC) for a localization/text editor. Read is GO
   (TEXTBIG_FORMAT.md); write gated on byte-exact .big round-trip (decomp agent
   proved GO — CRC is a format-class fingerprint, preserved verbatim).
2. `forge big` payload decoders for the cracked-structure formats (LIPSYNC keyframes,
   texture DXT descriptors) — read/inspect; pixel authoring waits on Lionhead-LZ.
3. Save editor once the decomp agent maps the entity-graph hero stats (latest+18 flag).
4. Close field-merge 14% via derived-class schemas (DEF_SCHEMA_COVERAGE.md).

## Current resume point (2026-07-19, latest+20, authoritative) - hero stats decoded (my latest+18 flag was WRONG)

Correcting myself: my latest+18 flag ("hero-stat map doesn't reproduce") was a
flat-search artifact — one zlib layer short, exactly as the decomp agent's
latest+19 diagnosed. The hero-stat map (docs/SAVE_HERO_STATS.md) is CORRECT.

I reproduced it independently in Python then shipped it natively in FableForge:
`forge save read` now inflates chunk1, finds SAVED_ENTITIES, streaming-inflates
the 25 nested zlib cells (miniz mz_inflate, unknown output size), locates the
`CTCHeroStats` cell (#10), and reads the confirmed tags in the bounded window
after it (seed-0 CRC via forge::defdecode::fieldTag): **Money=1625, Morality=483,
Age=26.4, Fatness=0, RenownLevel=2** — matching SAVE_HERO_STATS.md exactly on both
Cornelio saves. ctest green, validate CLEAN, real saves untouched (read-only copies).

So `forge save read` now reports gold/morality/age/renown — the most-requested
save-editing surface, now readable in the native tool. Commit in FableForge.

Next best tasks:
1. `forge save read` for the experience-pool tags (SAVE_HERO_STATS.md lists them as
   positionally-confirmed, names [hypothesis]) once the decomp agent names them.
2. `forge save edit` (write): reuse forge::defdecode value encoders + re-deflate +
   seed-0 trailer re-sign (decomp agent's PoC proved the round-trip). Gate: whether
   retail enforces the trailer signature on load (live-fire test).
3. Wire hero stats + HEADER into the eventual GUI save panel.

## Current resume point (2026-07-19, latest+21, authoritative) - text.big string decode

`forge text show/list` — decode text.big localization strings (`forge::textbig`
over `forge::big`), per the decomp agent's TEXTBIG_FORMAT.md §4.
- `forge text show <text.big> <name-or-id> [--json]` — one entry: UTF-16 content,
  SpeechBank, Speaker, Identifier, conversation tags (ANIM:/CAM:/mood); or, for a
  group entry, the member IDs.
- `forge text list <text.big> [name-filter] [--json]` — content previews.
- Validated on retail text.big: TEXT_CS_035_END_30 -> "I'd better make a move now
  and check on the damage." (speaker FARMER, ScriptDialogue.lug);
  TEXT_AI_GUARD_DEMAND_REBRIBE_SECOND -> group [16879,16872,16761,16858]. Unit test
  covers type-0 + type-1. ctest green.

PENDING FOLD-IN (from the decomp agent's running #13 agent): they are decompiling
the ~28 derived Thing-hierarchy classes (CREATURE 517/CPhysicsDef 315/CTargetingDef
254/CWeaponDef 190...) to close the field-merge clean-decode gap (currently 86%,
12,723/14,761 — DEF_SCHEMA_COVERAGE.md). When they emit an additive schema JSON,
fold it into docs/re_reference/def_schema.json (same as def_schema_missing.json)
and re-run `forge defs decode --all` to confirm the % rises toward 100.

Next best tasks:
1. Fold the decomp agent's derived-class schema when ready -> field-merge toward 100%.
2. `forge save edit` (write path): value re-encode + re-deflate + seed-0 trailer
   re-sign (decomp PoC proven); gate = does retail enforce the trailer on load.
3. `forge text`/`forge big` write paths once the .big round-trip writer is ported
   (decomp agent proved GO: CRC is a format-class fingerprint, preserved verbatim).
4. Consume the Blender addon / animation / entity-graph cracks as they land.

## Current resume point (2026-07-19, latest+22, authoritative) - field-merge 99.29%

Folded the decomp agent's `def_schema_missing2.json` (the derived-Thing-hierarchy
close-out I was waiting on) into FableForge's `def_schema.json`: 16 field-set
REPLACEMENTS (CPhysicsDef/CTargetingDef/CWeaponDef/... were incomplete) + 11 new
(CREATURE 52 fields, THING_GROUP, VILLAGE, HERO_ABILITY, PLAYER_INVENTORY, ...).
Update semantics (overwrite + add); 327 -> 338 types.

`forge defs decode --all` = **14,656/14,761 = 99.29% clean, 0 dirty** — matches the
decomp agent's DEF_SCHEMA_COVERAGE.md exactly. Only `THING_GROUP` (85) +
`ENGINE_THEME_GROUP` (20) still don't decode clean (their missing2 field-sets remain
incomplete — the last residual RE gap). Real merge: Aeon+LostContent field-merged
43->46 records, auto-merged fields 18->30, whole-record fallbacks 10->7 (rest are
genuine add-add). ctest green, validate CLEAN.

Field-level def merge is now effectively complete (99.3%). Full arc this day:
86% (missing) -> 100% name-resolution (overstated) -> corrected to 86% clean ->
99.29% clean (missing2).

Next best tasks:
1. THING_GROUP / ENGINE_THEME_GROUP: last 2 types' complete field-sets (decomp).
2. Write paths: `forge save edit` (re-sign PoC proven), `.big`/text writers (round-
   trip proven GO), once prioritized.
3. Keep consuming decomp cracks (Blender addon, animation, save entity-graph).

## Current resume point (2026-07-19, latest+23, authoritative) - SAVED_ENTITIES entity graph doc'd + validated

docs/SAVE_ENTITY_GRAPH.md WRITTEN (was never written; task resumed and closed) and
tools/parse_save_entities.py validated end-to-end, read-only, on all 4 real saves
(Cornelio/Manual-Save1, Cornelio/AutoSave.qs, Test/AutoSave, Default/AutoSave):
- 25 cells / 383,284 B inflated (Manual-Save1); hero cell located by CTCHeroStats content
  (idx 10 / 12 / 0 / 0 - moves per save, never locate by position).
- Full entity enumeration: 348 records (Manual-Save1) via UID/DEF anchor, cross-checked
  EXACTLY by 348 END\0 markers (AutoSave.qs has a benign 1-record delta, 348 vs 347).
  267 classed top-levels (Object 153/AICreature 48/Building 32/Marker 19/Village 15),
  108 distinct DEF-names; 147 distinct CTC* component classes (census in doc).
- Hero stats through the graph walker match SAVE_HERO_STATS.md exactly on all saves
  (1625/483/26.4/0.0/2 played; 0/0/18.0/0.0/0 fresh).
- Inventory decode count==declared on all saves (62/40/3 played, 5/0/0 fresh); names all
  valid OBJECT_* defs.
- QUESTS (LZO1X text 17,457 B + 119 B trailer), FACTIONS 29/29 byte-exact, REGIONS 141 -
  FACTIONS/REGIONS counts invariant across saves (static tables).
Open: cell first-u32 semantics, QUESTS-trailer + FACTIONS recTag names, base-field
tag->name map, parent<->child containment relation, the .qs 1-record END delta.
NOTE: a concurrent session was extending parse_save_entities.py during this pass (added
entity_records END cross-check + flipquest write PoC) - doc reconciled to its output; do
not treat doc entity counts as stale if the parser gains more commands.

## Current resume point (2026-07-19) - GUI v6-world PUBLISHED (was never published before)

- The **v6-world** mockup (source: session `e42c9bae` scratchpad `fableforge-gui-v6-world.html`,
  now copied to the current session scratchpad) is the realized 3D world-editor spec from
  `FABLEFORGE_UI_UX_RESEARCH.md`: left icon rail (World/Mods/Records/Scripts/Verify/Package) +
  Object Window (region->level tree w/ counts), center **3D level viewport** (shaded oblique
  terrain + placed entities colored by type + selection ring/XYZ move gizmo), right **Inspector**
  (Transform, Chest block: silver keys/OpenerObject/contents, Scripts links), breadcrumb +
  Validate/Stage&Play, footer "edits write through forge::tng - byte-exact". Dark+purple #9b6dff,
  0 non-ASCII.
- **PUBLISHED:** https://claude.ai/code/artifact/0e56e416-bce4-4eca-9e64-18cdac2b09bf
  (v4 remains separately at artifact a378f0d4; v5 + v6 had never been published - prior sessions
  hit limits first). **This v6 is the M4 visual spec** - it maps 1:1 onto shipped forge verbs.

## Current resume point (2026-07-19) - MESH GEOMETRY DECODE DONE + imports real geometry in Blender

- **Mesh geometry decode SHIPPED (native, no EgoCore exe).** Ported EgoCore (MIT)
  `C3DMeshContent::Parse` (MeshParser.h) + `GltfExporter.h` vertex-offset/triangulation into
  `fable_core` (`_parse_mesh_primitives` / `_prim_geometry`), wired into `build_model` (one Blender
  submesh per compiled PRIMITIVE). On-disk graphics.big mesh vertex+index buffers are **Fable
  chunked-LZO** (`[u16 clen][LZO]` filling size-3, +3 trailing plain bytes; `clen=0`=stored),
  inflated with `lionhead_lz` (LZO1X). VALIDATED: LZO-inflated vertices' bbox == entry stored bbox
  to 0.000 (MESH_SEAGULL_01, MESH_OBJECT_STATUE_BEAR); real compression ratios (seagull vbuf
  5080->2940 58%, bear 35796->28416 79%). io_scene_fable now imports REAL geometry in Blender 5.0
  (headless test green: seagull 2 mesh objs + armature + textures, bear 2983v/6596f). Supersedes
  find_subm_headers for LOD0 (fixes the bear-yields-0-submeshes gap). Corrected docs:
  BIG_MESH_GEOMETRY.md, MESH_BANK_INFLATE.md, BLENDER_ADDON.md, fable_core docstring.
- **CROSS-AGENT CONFLICT RESOLVED (empirical > static):** the mesh-bank-inflate Ghidra agent
  concluded the retail engine reads mesh entries RAW (no LZO on the load path; lzo1x_decompress
  @0xc06b90 has one save-side caller). REFUTED empirically - the on-disk blocks are genuinely
  LZO-compressed (ratios above), so the engine MUST inflate them; its (new) XrefTree.java missed the
  real caller (likely a chunked-LZO wrapper reached by indirect/vtable dispatch). The exact engine
  inflate SITE stays OPEN (settle via live x32dbg bp on the bank read during a mesh load); the
  on-disk LZO FORMAT is certain. New tool: tools/ghidra_scripts/XrefTree.java.
- **NEXT = export bridge, Route B (native, dependency-free) per user.** Blender mesh -> native
  serialize (STORED chunks clen=0, NO LZO compressor needed) -> graphics.big write (staged/reversible).
  Ordered: native serializer -> Phase-1 round-trip oracle (decode(encode(x))==decode(x)) -> Blender
  Export-to-Fable operator. LZO *compressor* is optional (size only). Plan: docs/MESH_EXPORT_BRIDGE.md.

## Current resume point (2026-07-19) - MESH EXPORT PIPELINE validated end-to-end (Route B)

- **Custom-model-INTO-game data path is proven** (native, no EgoCore exe). New files:
  `tools/blender_addon/io_scene_fable/mesh_rw.py` (serializer + `replace_block` + vertex
  `encode_positions`/`pack_pos_packed3`) and `tools/big_write.py` (BIGB container writer;
  faithful raw-record rebuild, only size/offset patched).
- **Validated:** vertex encoder identity `encode(decode(vb))==vb` byte-exact **84/84** prims
  (packed positions quantise losslessly for unedited verts; edit precision is per-mesh
  format-limited). Mesh serializer round-trip **60/60**. BIGB writer: text.big no-edit
  round-trip content-identical (28,913 entries); **full pipeline** - edit MESH_SEAGULL_01
  prim0 +2.0 X -> encode -> replace_block -> serialize -> rebuild graphics.big (243MB, 0.5s)
  -> re-parse: prim0 moved +2.035 (quant), prim1 + all other entries UNCHANGED. PASS.
- **REMAINING for #5:** the Blender export OPERATOR (bpy wiring: selected mesh -> per-vertex
  positions in original order -> encode -> big_write.rebuild -> STAGED graphics.big), staging/
  reversibility, and the in-game load test (user-only). Topology constraint for v1: edit vertex
  positions, keep the original vertex order/count (add/delete/retopo needs an index remap).
- **Also this session:** LZO decoder bug fixed (`lionhead_lz.py` was skipping the first byte on
  streams starting <=0x11 -> desync; now 60/60). LEV terrain writer byte-exact 399/399 (#8).
  Frontier RE done (#12): new-level recipe (CMapInfo::SaveBinary byte-map) + region graph format
  + navmesh container format; demon doors confirmed as region-graph nodes. Navmesh subdivision
  algorithm + per-node layout (#13) running in background.
- Docs: MESH_EXPORT_BRIDGE.md, LEV_WRITER.md, NEW_LEVEL_CREATION.md, NAVMESH_RE.md; corrected
  BIG_MESH_GEOMETRY.md / MESH_BANK_INFLATE.md (on-disk geometry IS LZO; engine inflate site open).

## latest+N: GUI layout fixes + LZO compressor integrated (2026-07-19 late)
- **forge-gui (branch gui-m4-start, commit 892e431):** fixed the two issues from the
  user's screenshots — Stage&Play button overflowing the right edge (right-align now
  measures real button widths) and records-list text overlap (def name hard-clipped at
  the "N fld" column, hover tooltip shows full name). Rebuilt clean.
- **#7 LZO1X compressor DONE + integrated:** tools/lionhead_lz_compress.py (minilzo
  port, M4-offset byte-mask fix) validated: mesh vbufs 67/67, texture mips 60/60,
  compress->decompress identity. mesh_rw.serialize_segments(compress=True) validated
  25/25 meshes round-trip; sizes 1.07x original (vs 1.24x stored). Custom textures
  are now definitively unblocked (no stored-block gamble needed).
- lionhead_lz.py: silenced the spurious "-3" NOTE (normal Fable trailing-3 framing);
  real mismatches still warn.
- Navmesh RE relaunched in background (attempt 3, told to save incrementally to
  docs/NAVMESH_RE.md). Prior 2 attempts died on session/spend limits with no output.

## latest+N: NAVMESH CRACKED (2026-07-19 night) — last hard RE gap for custom dungeons
- Navmesh RE (attempt 3) SUCCEEDED and was spot-verified by main session:
  subdivision algorithm recovered from CNavQuadTree/Node::Initialise; regen oracle
  reproduces shipped tree topology EXACTLY on 398/398 retail LEVs (spot-checked
  BarrowFields: 2224+110 nodes, both layers, EXACT MATCH). Per-node fields corrected
  (+0xc detail level, +0xd vertical layer, +0x1c preferability). Full detail:
  docs/NAVMESH_RE.md; oracles: tools/parse_navmesh.py correlate/regen.
- Bonus: LEV cell byte +20 classified = preferred-path flag (0 exceptions corpus-wide);
  noted in docs/LEV_WRITER.md. Also: 249/398 retail LEVs ship with EMPTY nav
  (interiors) -> navless custom interior levels are valid.
- Remaining nav gap (data-mapping, not format RE): thing-collision hulls ->
  blocking-line lists (CWorldMap::GetMapNavigationAreaInit). Terrain-only generator
  implementable today; over-approximates walkability only under placed objects.
- Ghidra lock freed -> queued save-signature-enforcement agent LAUNCHED.
- Still running in background: bind-pose port (addon), anim keyframes + demon door
  face (FableTLC), FQT quest-node port (FableForge, branch gui-m4-start).

## latest+N: save-sig ENFORCED (recipe known) + anim format CLOSED + demon door face solved (2026-07-20)
- SAVE SIGNATURE: ENFORCED, decompile-confirmed (CUserProfileManager::VerifySignature
  @0x00409730, gated in CWorld::LoadGameStateInternal @0x004A21F0; no retail bypass
  flag). Recipe fully known: trailer u32 = CCRC::Calc(0, file[0:trailer_off]), exact
  0x4B000 size, zero pad. tools/save_edit.py already satisfies it -> forge save edit
  SAFE TO SHIP; last QA gate = one in-game load of a re-signed save (user).
  docs/SAVE_SIG_ENFORCEMENT.md.
- ANIM FORMAT CLOSED: XSEQ keyframes fully decoded via EgoCore AnimParser port
  (tools/parse_anim_xseq.py): 3,435/3,435 retail anims parse byte-exact. Prior doc
  premises corrected (one raw LZO1X stream, plain fourcc chunks, per-bone XSEQ tracks).
  docs/BIG_ANIM_FORMAT.md S9.
- DEMON DOOR FACE SOLVED (docs/DEMON_DOOR_FACE.md): 16-bone Demon rig w/ real face
  bones; CONVERSATION_BASE + BREAKOFF gestures; six single-frame phoneme viseme poses
  (Type 9, face-bone AMSK mask) blended by lipsync weight curves (dialogue.big, 43 fps
  = 22050/512, confirmed by EgoCore LipSyncParser). Face is a CREATURE def; door leaf
  separate rig/OBJECT. Lipsync + anim WRITER references exist (EgoCore compilers).
- Main session verified: CREATURE_DEMON_DOOR_FACE_01 decodes CLEAN with FableForge's
  current schema (52 fields, leftover 0) -> the anim agent's "CREATURE layout" gap was
  the STALE ghidra_out schema copy, already solved. Demon door anims listed live.
- Ghidra lock FREE. Bind-pose port done earlier (goblin 67 bones verified visually).
  Still running: quest-node port, ForgeFSE fork, .qst RE.

## latest+N: ForgeFSE fork COMPLETE (2026-07-20 morning)
- D:\Code\ForgeFSE = user-owned FSE fork, 5 commits (pristine upstream 5ca4a66 ->
  README w/ no-license note -> v0.1 banner -> GetGameLanguage binding -> BUILD.md).
- Builds UNMODIFIED with VS2022 MSVC v143 (upstream .vcxproj already targeted
  v143/Win32; no CMake port, no mingw). One-command MSBuild rebuild in BUILD.md.
  Artifact verified PE32/i386, /MT static CRT, Lua 5.4 static, no exports (behavior
  runs from DllMain; hook @ ASLR-adjusted 0xCDB355; loads via external injector).
- Modify->build loop PROVEN: banner string + new binding confirmed in rebuilt DLL.
- Stretch shipped: GetGameLanguage() Lua binding wrapping NLocalisation::GetLanguage
  @0x00415070 (evidence: decomp_textbig_loader.c + BSim 0.897) — the TEMPLATE for
  generating more natives from the Ghidra DB.
- USER live-test: copy Release\FableScriptExtender.dll next to Fable.exe with an
  FSE\ folder (quests.lua), inject into running Fable.exe; log should open with the
  ForgeFSE v0.1 banner; GetGameLanguage() returns 9 on English.
- Still running: quest-node port (FableForge), .qst RE, native quest VM RE (Ghidra).

## latest+N: FQT quest-node port SHIPPED headless + .qst cracked (2026-07-20)
- forge::questnodes (FableForge, 4 commits on gui-m4-start, ctest green): 104 FQT
  curated nodes ported verbatim (19/61/14/8/2) + 931 manifest-generated raw nodes
  = 1,035 total. Graph->Lua codegen BYTE-IDENTICAL vs FQT's own 7 snapshot fixtures.
  Compiler has a manifest warning pass (every emitted Quest:/Me: call checked vs the
  931; one documented exception FQT itself ships). CLI: forge quest nodes / compile.
  Example docs/examples/quest_graph_example.json (demon-door talk+apple->open flow)
  compiles with 0 warnings — verified live by main session.
- .qst CRACKED (verified live): plain-ASCII script, AddQuest("Name",TRUE|FALSE);
  parser tools/parse_qst.py round-trips byte-identical (187+112 entries);
  ArenaRevisited = literal 1-byte diff; FSE's shipped example appends a custom quest
  (DemonDoorLUA) -> custom-quest registration is one appended line. docs/QST_FORMAT.md.
- CUSTOM QUEST LOOP NOW COMPLETE ON PAPER: author graph JSON -> forge quest compile
  -> Lua -> ForgeFSE loads it -> registered via one .qst line. User live-test pending.
- Still running: native quest VM RE (Ghidra), texture writer, new-mesh composition.

## latest+N: NATIVE QUEST VM answered — HYBRID, FSE structurally required (2026-07-20)
- docs/QUEST_VM_RE.md: quest LOGIC is native-code-bound (CONFIRMED decompile). NO quest
  VM exists. Registration routine FUN_00cd52d0 @0x00CD52D0 (ends exactly at FSE hook
  0xCDB355) writes 161 CScriptInfo{name,id,master,pAllocFunc,...} with per-quest native
  allocator code pointers as .text immediates. Binding = indirect call through that
  pointer -> per-quest vtable (Main/Init/OnPersist = compiled code). Table:
  ghidra_out/quest_registry_table.tsv.
- Reconciles with .qst: AddQuest registers the NAME (data); the compiled table supplies
  the LOGIC. Name-without-script is an anticipated engine state (QuestNotInScripts).
- CUSTOM-QUEST BOUNDARY: data-drivable w/o injector = name registration (.qst), initial
  activation (WLD), quest cards/gold/renown/objective text (game.bin), cutscene/region
  text scripts (script.bin), save persistence, re-plotting existing quests. Requires
  native code = any NEW quest control flow (and new TNG entity behaviors, same arch via
  AddEntityScriptBinding @0x00CB8230).
- IMPLICATION: FSE / ForgeFSE is STRUCTURALLY REQUIRED for new quest logic — not a
  tooling gap. Its design (append CScriptInfo w/ a Lua-hosting CScriptBase clone at the
  0xCDB355 seam) is the minimal correct solution. Validates the ForgeFSE + quest-node
  strategy. No native-data quest path exists to chase.

## latest+N: custom-asset WRITE trio complete + FSE fork mismatch diagnosed (2026-07-20)
- TEXTURE WRITER done (tools/texture_build.py): DXT1/DXT3/ARGB encode + mips + chunked-LZO,
  validated 227/230 retail + end-to-end inject (39.5 dB, 6323/6323 others byte-identical).
  Corrected BIG_TEXTURE_FORMAT §4.1/§6: only mip0 is LZO, mips1..n stored raw; MipSize0==0
  => whole payload raw. docs/TEXTURE_WRITER.md.
- NEW-MESH COMPOSITION done (mesh_rw.compose_mesh + big_write add_entry): compose full
  static mesh payloads from arbitrary geometry, add NEW graphics.big entries. EgoCore
  MeshCompiler was the answer key. Validated recompose(exact)/new-entry/Blender-import.
  Retail conventions found: ghost LOD, DegenerateTriangles sentinel material, type-1 layouts
  0x04/12 + 0x14/20 only. v1 = static only (skeleton blocks dropped), LOD0. docs/MESH_COMPOSE.md.
  => custom textures + custom meshes + terrain all have WRITE paths now.
- FSE LIVE TEST diagnosis: user's installed FableScriptExtender.dll (5,197,824 B, Feb 13)
  is their OWN FQT-modified build from D:\Code\FSE_Source (dllmain 33KB vs pristine 20KB).
  ForgeFSE was forked from the PRISTINE snapshot (4,948,480 B) -> different codebase ->
  loads+registers fine (banner+3 quests, no crash) but in-world quest hosting no-ops (their
  scripts depend on their FQT mods). User setup RESTORED (working DLL + FSE_Master.lua).
  FIX PENDING USER GO: re-base ForgeFSE onto D:\Code\FSE_Source (their real source; not yet
  git-tracked), re-apply banner+GetGameLanguage, rebuild to 5.2MB drop-in, re-test.
- FSE RE-BASE COMPLETE + DLL REBUILT (2026-07-20): ForgeFSE re-based onto the user's REAL
  source (D:\Code\FSE_Source, the codebase of their installed 5,197,824 B DLL). Git history:
  306e7c1 pristine import of real source -> 3d6f111 ForgeFSE v0.1 banner -> 4207878
  GetGameLanguage binding. Rebuilt Release|Win32 via MSBuild (VS2022 v143, /MT):
  D:\Code\ForgeFSE\Release\FableScriptExtender.dll = 5,198,336 B (installed build +512 B),
  PE32/i386 verified (machine 0x14C, magic 0x10B), banner + GetGameLanguage strings present.
  NOT deployed - user's working DLL stays in the game dir until they re-test.
  READY FOR USER LIVE-TEST: swap Release DLL into game dir, launch via FSE_Launcher.exe,
  load a save, check FSE\FableScriptExtender.log for "ForgeFSE v0.1" banner. NOTE: the
  FSE_Master.lua test lines were reverted during restore, so "GetGameLanguage = 9" will NOT
  appear unless the test call is re-added to a script; the banner alone confirms the fork
  DLL is running the real codebase.

## latest+N: SKINNED-MESH IMPORT SHIPPED — vertex groups + Armature modifier (2026-07-20)
- Blender addon now imports PER-VERTEX SKIN WEIGHTS: imported creatures deform
  correctly when the armature is posed. Decode `fable_core._prim_skin` (joint byte
  = paletteIndex*3 into the animated block's Groups[] LOCAL-bone palette; weights
  u8/255 renormalized — docs/BIG_MESH_GEOMETRY.md §7.1, ported from EgoCore
  GltfExporter.h); Blender side `import_op._apply_skin` (one vertex group per used
  bone, named by bone name; ARMATURE modifier bound to the imported armature; mesh
  parented to it). No global_id remap on the weight path (Groups[] is local).
- EXPORT-SIDE inverse also in: `mesh_rw.encode_skin` rewrites joint/weight bytes +
  block palettes; decode->encode->decode verified an EXACT fixpoint on all 6 skinned
  retail prims tested (goblin 4 + seagull 2, worst |dw| = 0.0).
- VALIDATED headless (Blender 5.0, retail graphics.big): NEW tests/blender_skin_test.py
  — goblin (67 bones: prims 312/1780/71/257 v, 8/54/5/6 groups) + seagull (27 bones):
  every group is a real bone name, every skinned vertex has >=1 group, weights sum
  ~1.0, and Blender weights EQUAL the _prim_skin decode (|dw|<1e-4). Proof .blends in
  tests/proof/. Decode is anatomically sane (face bones on head prim, arm bones on wings).
- NO REGRESSION: test_core.py, compose_test.py (recompose/synth/new-entry/no-edit
  oracles), blender_bones_test.py, blender_compose_test.py, blender_headless_test.py
  all PASS unchanged.
- Context: a prior agent died 9:15 AM mid-task; its edits (fable_core GeometryData.skin
  + _prim_skin + build_model wiring, import_op._apply_skin, mesh_rw.encode_skin,
  BIG_MESH_GEOMETRY.md §7.1) were reviewed and KEPT AS-IS — all correct; the missing
  piece was the promised tests/blender_skin_test.py (created) + docs. BLENDER_ADDON.md
  updated (TL;DR row, §1.2 skin binding, §4.2 skin test, §4.3 limitation lifted).
- Still open (skin scope): compose_mesh is still static-only v1 (composing NEW skinned
  meshes = emit bone blocks + animated blocks; encode_skin already provides the vertex
  side). Edit-existing-skinned-mesh weight export via the export operator is unwired
  UI-wise but the codec (encode_skin + replace_block on ('vbuf',pi)) is proven.

## latest+N: def-schema field decode CLOSED at 100.00% (2026-07-20)
- THING_GROUP (85) + ENGINE_THEME_GROUP (20) — the last 2 undecoded def types —
  are SOLVED: both Transfer methods are EMPTY (donor decompiles 0191da25 /
  0191e286 in ghidra_out/donor_def_transfers.c = bare ret; retail stubs
  0x0045954D / 0x00459764). They serialize ZERO fields; every one of the 105
  entries is exactly the 5-byte untagged base-class prefix (01 00 01 00 00;
  NULLDEFs all-zero) — the same prefix ENGINE_THEME entries carry before their
  first field tag. No live Ghidra needed (evidence already in donor_def_transfers.c).
- Mechanism: with no field tag to anchor the prefix empirically, def_schema.json
  gained an optional per-type "prefix_len" (set to 5 on THING_GROUP /
  ENGINE_THEME_GROUP + CThingGroupDef / CEngineThemeGroupDef keys); the FableForge
  decoder uses it as the prefix length, and a truncated/oversized payload is dirty
  (encode/decode still round-trips byte-exact).
- VALIDATED: forge defs decode --all game.bin = 14,761/14,761 clean (100.00%),
  0 no-schema, 0 dirty (was 14,656 = 99.29%). ctest green, forge validate CLEAN.
- FableForge commit 11aabb9 (branch gui-m4-start). DEF_SCHEMA_COVERAGE.md marked
  CLOSED; ghidra_out/def_schema.json mirror re-synced from FableForge (338 types)
  so no future agent trips on the stale copy again.
- Field-level def merge now covers ALL game.bin entries; no def-type RE gaps left.

## latest+N: .lug SFX/ambience audio banks CRACKED + extractor shipped (2026-07-20)
- docs/LUG_AUDIO_FORMAT.md + tools/parse_lug.py: data\Sound\*.lug =
  LiOnHeAd(LHFileSegmentBankInfo) block-stream container. Blocks = char[32] structName +
  u32 size + payload: title / LHAudioWaveData (packed complete RIFF/WAVEs) /
  LHAudioBankSampleTable (652-B records: dev path[260], id, waveId, riffSize, riffOffset,
  fmt fields, group[256] @+320, 3D params @+576) / LHAudioBankCriteiaInfo (sic) =
  event-tag ("SI_HERO;SE_FOOTSTEP;MATERIAL_GRASS") -> sampleId map, 1497 tags.
- VALIDATED all 50 retail banks: zero-slack walk + independent RIFF walk == table (sets;
  288 aliases, 9 empty slots). 3833 entries / 3545 unique clips / ~125.5 min audio.
  Codec = Xbox IMA ADPCM 0x0069 (3773) + plain PCM16 (51); 22050/44100 Hz, mono/stereo.
  Playability: all 7 Frontend PCM clips read via stdlib wave; ffmpeg decode sweep on
  Ingame.lug extraction = 2354/2354 OK (adpcm_ima_xbox). Music = loose .ogg (no container);
  dialogue = .lut (see LUT_AUDIOBANK_FORMAT.md). EgoCore has NO audio code (checked).
- Open: sample-table unk u16 (205 in Ingame), record fields +276/+280 and +576.. param
  semantics -> Ghidra follow-up (xref "LHAudioBankSampleTable" in Fable.exe; lock was
  taken this run). .met sidecar exact map (redundant). Repacker not attempted (easier
  than .lut: no hash table, but wave-block/table offset re-patch needed).

## latest+N: text.big WRITE path SHIPPED (2026-07-20)
- NEW tools/text_build.py: edit/add game-text entries + rebuild a valid text.big
  on top of big_write.rebuild. CLI: list / get / narrators / set / add /
  roundtrip / selftest. Payload codec is an EgoCore CTextParser port (type 0 =
  UTF-16LE content \0\0 + 3 len-prefixed strings (SpeechBank/Speaker/Identifier)
  + tags; type 1 = u32 count + member IDs) — re-encodes ALL 28,912 retail
  payloads byte-exact.
- NEW facts: type-0 TOC Info u32 = Speaker's index into the NarratorList
  (26,807/26,807 verified); the NarratorList "payload" actually lives in its
  TOC record's Info blob (u32 count + 379 cstrs) — TEXTBIG_FORMAT.md §4
  signature-scan claim was a misread (the sig is the record's own Name) and is
  now corrected; narrator TOC Offset == sub-bank TOC offset invariant kept by a
  post-rebuild fixup; CRC/Timestamp/deps are all 0 in retail; Identifier==Name
  on every entry; new IDs = max+1 (runtime indexes an array by ID).
- big_write.rebuild gained OPTIONAL toc_align= (default 4 = old behaviour;
  text.big packs TOC unaligned -> pass 1). Consumers unaffected:
  compose_test.py re-run vs graphics.big copy = ALL ASSERTIONS PASSED.
- VALIDATED vs COPIES of retail text.big (6,652,561 B, 28,913 entries), one
  command: `python tools/text_build.py selftest <retail text.big> <workdir>` ->
  (a) no-edit rebuild BYTE-IDENTICAL; (b) edit one string -> decodes back, all
  other entries + TOC fields byte-identical (speaker change also patches Info,
  bank-wide speaker<->Info audit 0 mismatches); (c) add new entry id=28914 ->
  decodes back, stats header + footer entry_count bumped, retail entries
  byte-identical; parse_bigb on outputs = CLEAN.
- Docs: NEW docs/TEXT_BIG_WRITE.md (recipe + evidence + open questions);
  TEXTBIG_FORMAT.md §4/§7 corrected/closed.
- Open: in-game smoke test (edit a visible line, e.g. Flourish onscreen help,
  confirm in Fable.exe); adding NEW narrator names unsupported (needs type-2
  Info rewrite, length-changing TOC record edit); non-English sub-banks
  untested (same grammar).

## latest+N: ANIM + LIPSYNC WRITE PATH SHIPPED (2026-07-20)
- Custom animations & custom speech mouth-sync are now WRITABLE. New tools:
  tools/anim_build.py (3DAF composer) + tools/lipsync_build.py (LIPSYNC composer);
  recipe doc docs/ANIM_WRITER.md. EgoCore AnimCompiler/LipSyncCompiler were the
  answer key (read-only, no exe launched); our writers are LOSSLESS field-level
  models, not EgoCore's re-deriving compiler.
- ROUND-TRIP PROOF (copies in work/anim_write/, originals untouched):
  graphics.big: decompressed-3DAF-image recompose byte-exact 3,435/3,435 (0 raw
  fallbacks, 213,728 tracks); recompress(lionhead_lz_compress)->decompress==image
  3,435/3,435 (compressed bytes never equal retail -- valid stream, different LZO
  choices, +5.3% size). dialogue.big: LIPSYNC encode(decode) byte-exact
  20,214/20,214 non-empty (6.30M weight keys), Info-duration coherent 100%.
- MODIFIED-CONTENT PROOF (big_write.rebuild edits+adds, both VERIFY PASS):
  ANIM_HERO_THROW posFactor x1.25 edit + NEW entry ANIM_HERO_THROW_SLOW (fps
  20->10, ANRT+Info dur 1.15->2.30 s, fresh 24-B info blob); lipsync Dialogue_1
  weights x0.5 + NEW LIPSYNC_FORGE_TEST_OH (synthetic 2.0 s OH curve, 86 frames
  @43fps, SPEAKER_FEMALE1 deps). All untouched entries byte-identical
  (3,434/3,434 and 20,504/20,504).
- New grammar facts: XSEQ's four u16-counted sections are optional-from-the-end
  (has_* flags); palette width u8/u16 keyed on pool count; ANRT/AOBJ freeform
  header bytes + raw tails preserved; retail image ends with 8-B footer
  04 00 00 00 00 00 00 00 (kept as trailer); TOC Info = 24-B C3DAnimationInfo.
- OPEN: (1) in-game visual soak not yet run (parser-clean only); (2) big_write
  capability request -- info_edits={(sub,name): blob} to retime an EXISTING entry
  in place (records copied verbatim today; add-as-new is the workaround; NOT
  implemented because the text.big agent holds big_write.py); (3) Blender action
  -> Track exporter is the natural next step, codec complete.

## latest+N: EFFECTS/PARTICLE SYSTEM CRACKED — effects.big reader shipped (2026-07-20)
- docs/EFFECTS_FORMAT.md + tools/parse_effects.py (--list/--extract/--validate):
  data\Misc\pc\effects.big = standard BIGB, sub-bank PARTICLE_MAIN_PC (v111),
  1,165 entries, UNCOMPRESSED payloads = serialized CParticleEmitter streams
  (magic 0x64, cstr name, flag/float block, systems -> components; component
  terminator 0x7B, system terminator 0x26). 10 CPSC* component classes; layouts
  + u32 quantization scales ported from EgoCore Particles/ParticleParser.h +
  ParticleCompiler.h (checked EgoCore FIRST — it is the byte-exact answer key).
  VALIDATED: 1165 exact / 0 slack / 0 failed, strict terminators, full-bank
  JSON extraction OK. Entry id == EParticleEmitter enum in
  data\Defs\RetailHeaders\pc\particles.h.
- Reference chain mapped: game.bin def int32 fields hold the effect id
  (~40 def types; verified CChestDef.OpenParticleEffect=391=CHEST_OPEN_01);
  TNG placed emitters reference BY NAME (StartCTCDParticleEmitter /
  ParticleTypeName "SUNBEAMS"); CParticleAttacherDef records = {u32 particleId,
  u32 names.bin offset(+0x14 hdr) of mesh helper e.g. EMITTER_LOWERARM, f32,
  u8}; emitter SpriteBankIndex -> textures.big GBANK_MAIN_PC id (textures.h).
  OBJECT.Components (CThingComponentSet) = count x 9-byte {names.bin offset of
  CTC class, u32 param, u8} + shared ~100B {hash,bool} tail (unsolved).
- WORKED EXAMPLE (torch flame): OBJECT_BANDIT_RAG_TORCH_LIT -> Graphic id 4594
  -> MESH_OBJECT_BANDIT_RAG_TORCH_LIT -> helper dummy CREATEPARTICLE
  "BRAZIERFIREFINAL" (CTCMeshAutomaticEntityCreator) -> effects.big entry 926
  (systems Fire/Smoke/Fire2) -> sprites 4404 SPRITE_SMOKEPUFF_LONG_02 + 4469
  SPRITE_FLAME_BUNCH_02. Green torches = recolor entry 926 + big_write rebuild
  (effects.big already round-trips; EgoCore ParticleCompiler is the encoder).
- GOTCHA: EgoCore x64/Release/EgoCore.exe (Jul 11) predates its own
  --extract-particles flag (main.cpp Jul 17) — exits 127; parse_effects.py
  replaces the read side, rebuild EgoCore only if the GUI/writer is needed.
- Ghidra follow-ups (lock taken this run): loader quantization-constant xrefs
  ("CPSC*" strings), CThingComponentSet tail hash table, CParticleAttachmentInfo
  f32/u8 semantics, CTCMeshAutomaticEntityCreator CREATEPARTICLE arg parsing.

## latest+N: .lug WRITE path shipped + Xbox ADPCM encoder (custom SFX + custom dialogue audio) (2026-07-20)
- tools/lug_build.py: (a) IDENTITY-ALL gold oracle = read->rebuild BYTE-EXACT on ALL 50 retail
  .lug banks (incl. Ingame 43MB/288 aliases, empty-slot banks); (b) REPLACE clip (any size;
  aliases follow the shared blob by design -- Ingame id145 -> {145,146} changed, 1490 criteria
  verbatim); (c) ADD new clip + 652-B record (template-cloned params, waveId=max+1) + criteria
  tag (village.lug id80 + "SI_HERO;SE_FORGE_TEST" -> [80] resolves). Every output re-passes
  parse_lug.validate() + per-clip byte-diff + ffmpeg/wave decode + IDENTITY fixpoint.
- CODEC ANSWER: ffmpeg CANNOT encode adpcm_ima_xbox (decode only). tools/xbadpcm.py = pure-Python
  Xbox ADPCM encoder/decoder; decode pinned BYTE-EXACT vs ffmpeg on retail clips (mono+stereo).
  Semantics: 36*ch-byte blocks, 64 samples/block = header predictor sample + 63 nibbles (64th
  nibble's sample DISCARDED), precise IMA expand ((2d+1)*step)>>3, blocks independent
  (header = exact source sample), avgBps = rate*ba//64 floored (retail-exact 12403). Encode
  quality 22-36 dB SNR via ffmpeg-decode round-trip. PCM16 also accepted anywhere in .lug
  (51 retail PCM clips prove engine path); .lut is XADPCM-only in retail.
- .lut: WRITE side already existed (lut_write.py, identity+replace validated 2026-07-19). NEW:
  encode gap closed -- proven chain PCM -> xbadpcm.encode_wav -> lut_write REPLACE
  --allow-resize (Dialogue2.lut copy) -> parse_lut --extract byte-exact -> ffmpeg decode OK.
  Fixed LUT_WRITER.md par.4 (it recommended a nonexistent ffmpeg encoder). .lut ADD-new-index
  still not attempted: new dialogue needs matching text.big SpeechBank + dialogue.big LIPSYNC
  entries anyway (cross-file join), + MaxEntries cap semantics unknown -- documented risk.
- Docs: LUG_AUDIO_FORMAT.md par.8 (write path + codec spec), LUT_WRITER.md correction.
  All work on copies (tmp/lug_work/); originals untouched. Open: engine runtime acceptance
  (no game launch allowed this run); .met sidecar NOT regenerated (build-time metadata,
  presumed unread [hypothesis]); sample-table unk/+276/+280/+576 semantics still Ghidra TODO.

## latest+N: APPEARANCE/EQUIPMENT SYSTEM CRACKED — slot->mesh->body-region model (2026-07-20)
- docs/APPEARANCE_SYSTEM.md + tools/appearance_survey.py: full hero clothing model,
  decompile-confirmed. KEY FORMAT FINDING: top-type game.bin entries (OBJECT/THING/...)
  start with a CDef LISTING: `01 00 01, u16 n, n*{u32 crc0(defClassName), i32 dataEntryIdx,
  i32 ownerEntryIdx}` — component-def data is separate unnamed entries, inherited from
  template objects (OBJECT_BASE 285 / OBJECT_HOLDABLE_TEMPLATE 325 / _CLOTHING_ 326).
  crc0 = the seed-0 field-tag CRC, also used for class keys ("CInventoryItemDef"=83AD7C03).
- Chain: OBJECT item -> CAppearanceModifierDef {SuitPart slot (1 hat,2 shirt,3 gloves,
  4 trousers,5 boots), CoversBodyAreaFlags (bitmask, table in doc), HideMaterials (hero
  body material names to hide), Graphics[] 24-B recs {statA,statB,MODELID,scale,appear,
  disappear} -> graphics.big MBANK_ALLMESHES ids, ArmourDefIndex, HeroSuit}. Suits:
  HERO_SUIT_* THING -> CHeroSuitDef.SuitParts map<slot,OBJECT>. VALIDATED 148/148 vanilla
  items resolve every mesh id (type-5 skinned; boot L = 10-bone hero-rig subset).
- Engine (headless decomp, ghidra_out/appearance_decomp.c): CTCHeroAttachableAppearance-
  Modifiers keeps 3 lists by AppearanceType (0 hair,1 horns,2 clothing); equip =
  RemoveAppearanceModifiersCoveringBodyArea(covers,type) @0x7066A0 (retail AMD offsets:
  covers +0x40, type +0x50) then AddAppearanceModifier @0x706370; suit equip
  SetThingAsWearingClothingSuit @0x5B3D7A; draw @0x707440 wraps each worn mesh in
  CEnginePrimitiveMorphedAnimatingMesh SHARING the hero's anim interpolators — worn
  meshes are SKINNED to hero-rig bone names, NO attach bones/sockets exist.
- HalsSword.fmp anatomy documented (append entries at next free indices, mesh id
  maxId+1=8113, OBJECT.Graphic patched; stale ownerEntry proves engine keys on dataEntry;
  fmp LinkMetaData = .NET BinaryFormatter offset->name maps, installer-only).
- Custom-boots recipe in doc §7 — fully data-driven, no code needed. Open: Graphics
  statA/statB exact selector semantics; 21-slot {u32,u8} CThingComponentSet tail hash;
  composing NEW skinned meshes (compose_mesh still static-only; workaround = clone a
  retail boot payload + replace_block geometry).

## latest+N: navmesh thing-blocking mapped + terrain-only GENERATOR shipped (2026-07-20)
- docs/NAVMESH_RE.md gained two sections. (1) THING-COLLISION -> BLOCKING (empirical, Ghidra
  lock was taken): the delta mask (LEV-terrain-walkable but nav-blocked) = placed
  Object/Building collision footprints (rotate with RHSetForward; buildings = whole-room
  shells, e.g. BUILDING_KRAKEN_CHAMBER) + unreachable-island removal (terrain components
  without a NAVIGATION_SEED / REGION_ENTRANCE_POINT / REGION_EXIT_POINT are blocked
  wholesale; markers do NOT anchor; sole corpus exception NorthernWastes2 = layer-1 bridge
  reachability) + ~1.5% map-border seam artifacts. Switchable leaves: switchKey ==
  thing UID & 0xFFFFFFFF, 407/407 corpus keys match (CTCDoor doors, thorn-vine degradables,
  buyable BHF furniture). Negative result: the LEV obsOffset block is 33801 bytes, all-zero
  in all 397 retail LEVs - blocking lines are NOT baked into the LEV.
- (2) tools/parse_navmesh.py grew `generate <in.lev> [out.lev] [--tng] [--compare]`:
  terrain-only navmesh builder (TopologyWeights from cell bytes +15/+20, island rule,
  proven subdivision, writer conventions recovered: post-order index assignment with
  blocked-root index burning, pre-order records, blockedArg=1 on root-slot records,
  edge-adjacency neighbours, region flood fill with numRegions=N+1 when a layer exists).
- VALIDATED: 148/148 nav-bearing retail LEVs generate + pass structural self-validation and
  lev_rw round-trip; on zero-thing-delta levels output is byte-exact vs retail MODULO
  neighbour-list order (17/17 single-layer; neighbour ORDER = build-traversal insertion
  order, unrecoverable offline). Generated walkability never under-covers shipped except
  NorthernWastes2; extra walkable = 7.4% of map area = thing carving (documented). Navless
  demo: DragonCliff_Vista_01 gains a valid 288-node mesh. regen/correlate/roundtrip modes
  unregressed.
- Ghidra follow-ups flagged: GetMapNavigationAreaInit (ego_r 0x004df950 / retail 0x0050a650)
  line gathering; CalculateChildrenNeighbours 0x031a79d0 (neighbour order); physics-mesh
  footprint routine. Def-side leads already decoded: CDoorDef.Open/ClosedCollisionMesh,
  CDegradableDef.InitiallyBlocksNavigation, CKickableDef.IgnoreNavigationTest.

## latest+N: NEW-LEVEL ENGINE REGISTRATION CRACKED — retail load path + BWD writer + staged ForgeTest region (2026-07-20)
- docs/NEW_LEVEL_ASSEMBLY.md completes the new-level recipe on the RETAIL engine side
  (companion to NEW_LEVEL_CREATION.md). Key decompile-confirmed facts (Fable.exe, headless
  runs -> ghidra_out/newlevel_retail_decomp{,2,3}.c, newlevel_fablewin_run5_decomp.c):
  * CWorld::Load 0x4a1840 -> CWorldMap::LoadFromFile 0x507c30. Retail loads the world from
    FinalAlbion.bwd (compiled CMapInfo+CRegion stream) because userst.ini sets
    UseCompiledWorldFiles TRUE (DAT_013b8618); the .wld TEXT is parsed only when FALSE.
    AllowDataGeneration FALSE means the game never regenerates .bwd/.gtg. UseLevelWAD TRUE
    (DAT_01375456) makes LoadLevel 0x502620 read BOTH .lev and .tng from FinalAlbion.wad.
  * .wld text grammar in retail: NewMap <n> = the 1-based map SLOT (not always 1 as
    NEW_LEVEL_CREATION.md implied); on EndMap the box extent comes from OPENING the LEV
    (GetLevelDimensions); overlapping placement DROPS the map; MapUID 0 -> allocator.
    START_INITIAL_QUESTS is NOT parsed by retail (initial quests = <world>.qst AddQuest lines).
  * BWD format fully cracked: tools/wld_bwd.py parses+rewrites FinalAlbion.bwd BYTE-IDENTICAL
    (64,223/64,223). Layout: u32 mapCount, (n-1) CMapInfo recs (slot0 never stored), u32
    regionCount, (n-1) CRegion recs (CMapInfo::LoadBinary 0x4fb4f0; CRegion::LoadBinary donor
    0x1f70940).
  * mapwho = RUNTIME spatial hash only (CThingMapManager::Init 0x638540 / AddGameMap 0x638310);
    NO file data — the old "mapwho blocks in .lev" item conflated it with the LEV obstacle/brush
    region, which CMap::LoadFromFile 0x821a80 reads (25-B header IS CFileFormatEditMapBrushHeader;
    0x10822 map-header block = 22B + theme table + cellVersion/themeCount + SECOND 256x132
    brush-def table; cells; then w*h {u32 len, payload} obstacle records when cellVersion>=2).
  * Empty TNG ("Version 2;") and navless LEV are shipping-valid (Darkwood_Filler_08 precedent).
    No script.bin/.qst/.gtg changes needed for a new map (evidence table in the doc).
- Tools: tools/wld_bwd.py (bwd read/write + roundtrip oracle), tools/wad_add.py (append NEW
  entries to FinalAlbion.wad; 2048-aligned payloads, footer rebuild, crc field is 0 in retail).
- EXPERIMENT (static, install untouched): work/newlevel_experiment/assemble_forgetest.py ->
  stage/ = ForgeTest region cloned from Darkwood_Filler_08 (32x32, 0 things, navless) at free
  hole (2784,2560), MapUID 4193259, map slot 399 + region slot 142 wired in BOTH .wld text and
  regenerated .bwd, LEV+TNG appended to a WAD copy. Gates passed: forge wld info 399/142 +
  byte-identical roundtrip; bwd reparse OK; WAD new entries extract back byte-identical and
  existing entries unchanged. AWAITS LIVE GAME TEST (not deployed; no game launch this run).
- OPEN: STB per-level entry requirement (unknown), region-graph absence behavior, from-scratch
  brush-region bytes (clone workaround), thing-UID remap for cloning TNGs with things.

## latest+N: ForgeFSE LIVE TEST PASSED — .qst activation was the root cause (2026-07-20)
- LIVE-CONFIRMED in running game (user loaded existing save): ForgeFSE v0.1 banner;
  Quest:GetGameLanguage() == 9 (the new binding, decompile-derived, works live —
  the generate-natives-from-Ghidra-DB template is PROVEN); FSE_Master Init+Main ran;
  both user quests activated; entity scripts executed (19KB of live log).
- ROOT CAUSE of all prior "frozen at registration" runs: FinalAlbion.qst had NO
  AddQuest lines for FSE quests. FSE only REGISTERS Lua hosts; the ENGINE activates
  from .qst/save state. FSE log flushes per line (open-append-close, checked in
  source) so the frozen log was truthful: registered, never activated.
  Fix: appended AddQuest("FSE_Master",TRUE)+2 dormant to FinalAlbion.qst
  (backup: FSE\backups\FinalAlbion.qst.pre-fse.bak). Prior "codebase mismatch"
  diagnosis was WRONG; user's in-world reports were accurate throughout.
- NEW HARD FACT: engine applies NEW .qst AddQuest entries when LOADING an existing
  save (not just new game) — custom quests deploy to existing saves.
- RECURRING FAILURE MODE: Steam integrity verify restores retail FinalAlbion.qst and
  silently deactivates all FSE quests (likely what broke the user's Feb setup).
  forge qst list is the 5-second check; re-append to fix.
- Deployed state: ForgeFSE real-source DLL (5,198,336 B) live in game dir; test line
  still in FSE_Master.lua Init (harmless; remove or keep).

## latest+N: CUSTOM DIALOGUE pipeline shipped + CROSS-FILE JOIN solved (2026-07-20)
- THE JOIN (empirical, Ghidra lock taken): text.big stores only the bank NAME
  (SpeechBank="ScriptDialogue.lug"); the clip index <N> lives in data\Defs\<bank>snds.bin:
  u32 count + count x {u32 crc, u32 soundID} sorted by crc, crc = crc0("SND_"+entryName)
  (same seed-0/EDB88320 hash as def field tags). soundID == .lut clip Index == dialogue.big
  LIPSYNC id (Dialogue_<N>/ScriptDialogue_<N>). Proof: 12121/12129 + 5263/6325 + 2704/2738
  text entries resolve (misses = audio-less GUI/log lines); text-length vs lipsync-duration
  r=0.77-0.86 under this join vs r~0.00 under the old ordinal hypothesis (REFUTED); EgoCore
  TextBackend ResolveAudioID uses the .h form (SND_<name> = N enums, BankCreator); Fable.exe
  carries the "SND_" prefix string; names.bin references dialoguesnds.h et al.
- KEY .lut FACT: tail MaxEntries (500/1000) < retail Count (12134/5310) in every shipped
  bank => NOT a count cap; ADD-new-index unblocked. lut_write.rebuild grew add=(N, record)
  (append clip at max+1, tail regenerated, fixpoint-validated).
- tools/dialogue_pipeline.py: WAV+text+speaker -> staged text.big + <bank>.lut + dialogue.big
  (+ snds.bin on ADD), all against copies, full static validation + deploy checklist.
  resolve/stage/makewav subcommands; REPLACE = honest v1, ADD staged but engine-[unproven].
  Also closes ANIM_WRITER's in-place TOC Info gap (patches LIPSYNC f32 duration in place).
- WORKED EXAMPLE (staged, ALL CHECKS PASSED): Barrow Fields demon door
  TEXT_QST_088_EAT_PIES_INTRO_40 -> crc C699DB84 -> scriptdialoguesnds.bin N=5080 ->
  ScriptDialogue.lut clip 5080 replaced w/ 3.0s synthetic clip (5309 others byte-identical),
  ScriptDialogue_5080 lipsync re-authored (130 fr, Info dur patched), new text+speaker set;
  ffmpeg decodes staged clip. ADD example: TEXT_DDOOR_FORGE_NEW_LINE_10 -> ScriptDialogue2
  N=3061, 4 artifacts staged clean. Dirs: work/dialogue_pipeline/demon_door_{replace,add}/.
- Docs: docs/DIALOGUE_PIPELINE.md (join model + evidence + recipe + live-test plan);
  LUT_WRITER.md par.7 updated. Open: live REPLACE smoke test then ADD acceptance test
  (no game launch this run); synth_curve is placeholder (audio-energy->viseme tool next);
  Ghidra follow-ups: snds.bin loader + "SND_" xref, MaxEntries semantics, GetTextBySymbol
  path, LIPSYNC SPEAKER dep (vestigial?).

## latest+N: SKINNED mesh composition shipped -- custom-equipment write path complete (2026-07-20)
- mesh_rw.compose_mesh grew type-5 SKINNED support (docs/MESH_COMPOSE.md v2, new par.1b):
  compose_mesh(..., skeleton=clone_skeleton(donor_pay, 5)). What was needed beyond the
  vertex codec (encode_skin, already proven): (1) skeleton emission -- the four bone
  blocks are CLONED RAW from a donor (byte-exact, no bind-matrix rederivation);
  (2) CAnimatedBlock records replacing static blocks on skinned prims (sbc=0,
  BonesPerVertex=3, PalettedFlag=1 -- every retail skinned prim checked); (3) the
  Groups[] palette partition: single block when the bone set fits, else EgoCore
  ImportType5's greedy face walk (cap 16; retail reaches 18; grammar max 85) with
  per-block vertex regrouping; (4) AnimatedFlag=1 + real BoneCount/BoneNameSize;
  (5) NO DegenerateTriangles sentinel material (retail skinned meshes carry none);
  (6) ghost LOD keeps the skeleton (EgoCore full clone; retail ships a reduced one --
  both parse). New retail facts: weight bytes sum EXACTLY 255 on all 82,073 skinned
  verts surveyed, max 3 influences; skinned strides 20 (0x04 packed, creatures) /
  28 (0x14 float, hero boots). Skin input = per-vertex [(bone_name_or_slot, weight)].
- decode_to_compose_args now returns (name, prims, mats, skeleton) and decodes skinned
  prims (skin + skin_blocks preserving the source ablock partition). clone_skeleton()
  is the donor extractor. Static path/API otherwise unchanged.
- VALIDATION (all PASS, tests/compose_test.py + blender_compose_test.py):
  (a) RECOMPOSE oracle on goblin (67 bones, 7 ablocks, strips), seagull, 2 hero boots:
  verts/faces/UVs exact, palettes identical, per-vertex skin decode identical, skeleton
  blocks byte-exact, ghost parses (grammar-exact; strips->lists, deeper LODs/cloth/
  helpers dropped -- documented). (b) NEW skinned mesh (donor boot geometry x1.15,
  re-skinned by bone NAME) injected into a graphics.big COPY as type-5 id 8115 via
  big_write adds=, headless Blender re-import: armature = donor's 10 bones, ARMATURE
  modifier + parenting, weights match authored <=1.5/255. Multi-block path exercised by
  a 41-slot synthetic ribbon (3 blocks). Full suite green: compose_test, test_core,
  blender_headless/bones/skin/compose tests, roundtrip_report on bear+goblin+seagull+boot.
- Remaining limits: skeleton must be cloned (no from-scratch rigs); LOD0+ghost only;
  no cloth/bump layouts; engine load test still not run (next: wire a composed boot id
  into a CAppearanceModifierDef per APPEARANCE_SYSTEM.md par.7 and equip in-game).
  APPEARANCE_SYSTEM.md par.7 gap note closed.

## latest+N: ONE-LAUNCH SMOKE TEST STAGED — all of today's write tools in work/smoke_test/ (2026-07-20)
- work/smoke_test/stage/ mirrors game-relative paths; deploy.ps1 backs up originals (SHA256
  ledger backups/original_hashes.txt) then copies staged files in; revert.ps1 restores + deletes
  the one NEW file (ForgeTest.tng). Both idempotent + refuse to run while Fable.exe is up.
  NOT deployed this run (game was running). Install untouched.
- TEXT  data\lang\English\text.big: TEXT_GUI_MENU_CONTINUE_GAME -> "FORGE: Continue Game"
  (Esc menu top item = visible in seconds) + TEXT_QST_028_ONSCREENHELP_FLOURISH_BASIC
  FORGE-prefixed. parse_bigb CLEAN, decode-back OK.
- TEXTURE data\graphics\pc\textures.big: GRAPHIC_HERO_FACE (face+HANDS sheet — always on
  screen) tinted bright green (65% blend), DXT1 512x512 8 mips via texture_build replace.
  Decode-back verified green; container structure matches retail parse shape.
- AUDIO data\Sound\Ingame.lug: all 10 SI_HERO;SE_FOOTSTEP;MATERIAL_GRASS clips (ids 820..838
  even; child aliases follow) replaced with 0.3s 880/1320Hz alternating beeps, PCM->xbadpcm
  ADPCM. NOTE grass ids are shared by MATERIAL_EARTH/SCRUB/STRAW criteria (same clip list) so
  those surfaces beep too. Every REPLACE re-parse OK; extracted id820 decodes to ~880Hz tone.
- ANIM data\graphics\graphics.big: reused work/anim_write/graphics_mod.big (graphics_copy.big
  MD5 == retail, verified) = ANIM_HERO_THROW amplitude x1.25 edit + ANIM_HERO_THROW_SLOW new
  entry. anim_build --verify-mod PASS (3434/3434 untouched byte-identical).
- REGION: newlevel_experiment stage (FinalAlbion.bwd/.wld/.wad + ForgeTest.tng loose) copied in;
  re-validated (bwd roundtrip byte-identical, wld info 399 maps/142 regions + roundtrip, WAD
  ForgeTest.lev/.tng extract clean, lev info parses). ENTRY MECHANISM: FSE
  Quest:EntityTeleportToPosition(hero,{x=2800,y=2576,z=74},0) — manifest has NO load-region
  API; blocking quest-teleport is the retail primitive; z from lev_rw terrain (72.89 @ centre).
  Staged FSE\Master\FSE_Master.lua adds ForgeTestEnter thread: banner at load, teleport at
  +60s (keeps minute-1 clean for other checks), logs pre/post pos + GetCurrentMapName +
  IsRegionLoaded('ForgeTest') to FableScriptExtender.log = live verdict for NEW_LEVEL_ASSEMBLY
  open item 5 (does the engine stream a prox=0 map on teleport?). Standalone snippet:
  work/smoke_test/enter_forgetest.lua. Lua syntax gated via luaparser.
- Expected in-game: green face/hands immediately; Esc menu "FORGE: Continue Game"; beeping
  grass footsteps; exaggerated throw; 60s banner->teleport. All checks < 2 min on any save.

## latest+N: green-tint target CORRECTED -- hero skin chain solved, stage2 staged (2026-07-20)
- Stage-1 tinted GRAPHIC_HERO_FACE: NO in-game effect (user-confirmed, adult hero). Root
  cause found empirically (no Ghidra -- lock taken): GRAPHIC_HERO_FACE (MAIN id 45) is
  referenced by ZERO of the 3295 compiled-mesh material lists. The playable adult hero is
  MESH_HERO (graphics.big id 4299, type 5); its 12 materials ARE the live sheets:
  HERO_SKIN_FACE 1250 (face), HERO_SKIN_ARMS 1248 (arms+HANDS), TORSO 1252 / BACK 1249 /
  LEGS 1251 / MOUTH 1253 + HERO_BUMP_SKIN_* 1233-1237 + eyeshadow 1045. Teen/young heroes
  use MESH_TEEN_HERO (HERO_TEEN_FACE 1254) / MESH_YOUNGHERO_02 (YOUNGHERO_SKIN_*).
- Morph side decoded: CHeroMorphDef (game.bin 10535 & 11246, identical TextureMorphs)
  = 156 x 24B records {stat, stat, layer, texBase, texOverlay, 2}: engine COMPOSITES
  COMP_HERO_* overlays onto the base HERO_SKIN_* sheets by stat (4=hair-age, 5=skin-age,
  6=evil(+antitan layer0), 7=good/tan+blonde, 8/9=muscle/stomach, 10=will, 3/11=stubble/
  body hair) -- mesh texture ids never change, so tinting the BASE sheets always lands.
  Full table + blob layout: docs/HERO_MORPH.md (new); pointer added APPEARANCE_SYSTEM.md 7b.
- STAGE2 (NOT deployed): work/smoke_test/stage2/data/graphics/pc/textures.big built by
  stage2/build_stage2.py from the ledgered pristine backup -- 23 sheets tinted green 65%
  (FACE/ARMS/MOUTH bases + all old/evil/tan/antitan/will/stubble/muscle/hair overlays so
  ANY save state stays green), one big_write.rebuild, formats/mips/transparency preserved.
  Validation ALL PASS: census identical, 23/23 decode back green-dominant (PSNR 33.9-47.1
  dB), GRAPHIC_HERO_FACE restored byte-identical to retail (stage-1 mistake undone).
  Deploy = copy over data\graphics\pc\textures.big with game closed (stage-1 deploy.ps1
  pattern); expected: green face+hands on any adult save.
- Ghidra follow-ups flagged (HERO_MORPH.md par.4): who consumes GRAPHIC_HERO_FACE/45 +
  _MOUSTACHE/46; which def owns morphdef 10535 vs 11246; layer-field semantics +
  replace-vs-blend in the compositor (CHeroMorph Transfer / FUN_00708600-area).

## latest+N: BLENDER ANIM BRIDGE shipped -- actions <-> ANIM (3DAF), round-trip proven (2026-07-20)
- NEW tools/blender_addon/io_scene_fable/anim_ops.py (registered in __init__):
  IMPORT: ANIM_* entry -> Blender action on the imported Fable rig
  (build_action_from_anim; quaternion+location fcurves per track, LINEAR,
  anim frame i at Blender frame i+1, fable_* custom props on the action);
  EXPORT: action -> donor-clone (ANIM_WRITER par.4 recipe automated):
  anim_build.parse_image(donor) + replace each matching track's quat/pos
  pools+palettes with fcurve-sampled data (keyframe reduction by f32-exact
  dedup; positions requantized i16 via Track.set_pos_keys auto factor; quat
  sign-aligned along time), compose_payload + big_write.rebuild edit/add to a
  STAGED .big. Handles legacy Action.fcurves AND slotted actions (Blender 5.0).
- COORDINATE CONVENTION (probed, documented ANIM_WRITER.md par.7): anim tracks
  store the FULL parent-relative local transform in mesh model space (importer
  maps Fable->Blender verbatim, no axis swap): pos = local translation, quat
  (x,y,z,w) = CONJUGATE of the mathutils local rotation. Stationary bones'
  anim keys == mesh bind-local transforms exactly (up to i16 quantum / q sign).
  Pose math: basis_q = conj(rest_q) @ conj(q_anim); basis_p = conj(rest_q) @
  (p - rest_t); export is the exact inverse.
- ROUND-TRIP PROOF (tests/blender_anim_test.py, headless Blender 5.0):
  goblin ATTACK_01 (61 data tracks, 1891 frames) max quat 1-|dot| 1.85e-07,
  max |dp| 2.04e-04 units; sparrow FLY 1.59e-07 / 9.66e-05 (asserted < 5e-6 /
  2e-3). Recomposed payloads re-parse clean (0 pal_oob/tail/quat_bad) and
  survive big_write injection + reload. FULL SUITE GREEN: compose_test,
  test_core, blender_headless/bones/skin/compose/anim tests.
- Limits: donor-clone only (no from-scratch track invention; empty stub tracks
  Scene Root/movement_dummy, missing-bone tracks, helpers, MVEC kept verbatim);
  same-name retime blocked by the known big_write in-place Info gap (add-as-new
  works); in-game visual soak still untested. Docs: BLENDER_ADDON.md par.1.5 +
  par.4, ANIM_WRITER.md par.5 item closed + new par.7.

## latest+N: ForgeTest save-load CTD ROOT-CAUSED (missing STB entry) + stage2 fix built (2026-07-20)
- Stage-1 live test verdict: 4-file ForgeTest set -> save-select OK, CTD on save selection.
  WER Application log nailed it in minutes: 2x 0xC0000005 @ Fable.exe+0x62428A (13:09/13:11)
  = VA 0xA2428A = def-by-global-index getter (BSim-misnamed "GetAbilityLevel"), wild index.
  (13:12 FSE.dll+0x200599 crash = follow-on run, not root.)
- Chain (Ghidra, headless readOnly): CWorld::Load "Set Static Map for Engine" ->
  CEngine::SetStaticMapFileForUse 0xB23DC0 -> CEngineWorldMap::OpenStaticMaps 0xB42750 ->
  FUN_00B420F0 builds map<levelName,chunkOff> from __STATIC_MAP_COMMON_HEADER__ of
  FinalAlbion_RT.stb, then per EVERY registered map CEngineMap::OpenRetailStaticMap 0xB41E50:
  FUN_0042B467 (map lower-bound) returns HEAD node on miss; *(node+0x14) used as seek WITHOUT
  found-check -> garbage CStaticMapInfoBlock (ReadMapInfoBlock 0xB3EFA0) ->
  CLocalDetailCacheMap::OpenStaticMap 0xBDF010 -> palette Load 0xBDEDD0 -> wild def index ->
  AV. So NEW_LEVEL_ASSEMBLY checklist #12 ("likely benign") was WRONG: STB entry is MANDATORY.
  Suspects 2-4 (region graph / BWD new-record path / WAD TOC) exonerated for this crash.
- STB chunk format cracked (ReadMapInfoBlock + differential diff of same-size retail chunks):
  92B MapInfoBlock {ver, bankEntryId, w,h, MapX,MapY ints, float box, ...} + landscape hdr +
  detail hdr; exactly 5 position-dependent abs-offset fields per chunk (+0x20,+0x24,+0x58,
  chunk+0x5C dword, chunk+0x71 dword) - invariant verified 398/398; downstream readers
  sequential (no other seeks). Full layout in NEW_LEVEL_ASSEMBLY.md par.9.
- FIX BUILT: work/newlevel_experiment/assemble_forgetest_stage2.py -> stage2/ = stage1 4 files
  unchanged + rebuilt FinalAlbion_RT.stb: new bank entry 426 ForgeTest.lev (payload = clone of
  Darkwood_Filler_08.lev), common header 398->399 names (+43B; BanditCamp_Filler_05 chunk
  relocated to tail, 5 offsets rebased), ForgeTest chunk = DW08 clone w/ id 426, coords
  (2784,2560)-(2816,2592), 5 offsets rebased. Static gates ALL PASS (423 payloads
  byte-identical @ same offsets, donor payload identical, 399 chunks re-parse w/ invariants,
  every WLD LevelName resolves, 397 chunks byte-identical); independent forge.exe stb list
  reads 425 entries / 399 static maps. NOT deployed; install untouched; stage-1 forensics
  kept at work/smoke_test/stage/data/Levels/.
- STILL NEEDS LIVE TEST: deploy stage2 5-file set (bwd/wld/wad/loose tng/stb), load save,
  confirm world constructs, then FSE ForgeTestEnter teleport verdict (open item 5). Watch:
  detail-palette def indices are donor's (should resolve, same def set); sea-map path
  untouched; if a NEW crash appears, diff WER offset first.

## latest+N: FRONTEND/GUI data cracked -- menus + HUD are compiled-defs, all editable (2026-07-20)
- LOCATION: the "frontend binary" is data\CompiledDefs\frontend.bin = the SAME
  CompiledDefs container as game.bin (names.bin-joined, tagged fields). 810 entries,
  810/810 decode CLEAN under def_schema.json via forge defs decode: 785 UI (CUIDef,
  109 fields) = the whole menu system as a real widget SCENE GRAPH (Children =
  Vector_int32 of global entry indices, 222 roots), + FRONT_END / UI_MISC_THINGS /
  UI_ICONS / CONTROL_SCHEME / ENGINE / ENGINE_VIDEO_OPTIONS / CONFIG_OPTIONS_DEFAULTS.
  HUD lives in game.bin PLAYER_GUI_PC / PLAYER_GUI_DEFAULT (CPlayerGuiDef, 463 fields
  in schema -- not 432).
- ASSET CHAIN (all id joins verified against RetailHeaders):
  widget States[].GraphicIndex -> front_end_bank.h -> frontend.big (BIGB
  GBANK_FRONT_END_PC, 394 texture entries, parse_bigb+parse_texture CLEAN,
  texture_build.py applies); PLAYER_GUI graphic ints -> textures.h -> textures.big
  GBANK_MAIN_PC (HUD_HEALTH_BAR_RING = 5553 etc.); effect ints -> particles.h
  (GUI_HEART_BEAT_NORMAL = 816); widget TextValue = literal wide string OR TEXT_GUI_*
  symbol resolved via text.big (already writable); Font -> fonts.h -> fonts.big
  (BIGB, 3 sub-banks; glyph payload NOT cracked, 25.9MB GlyphData). Loading screens
  = loose DDS in data\Misc\LoadProgress.
- NEW TOOL tools/parse_frontend.py: pure-python CompiledDefs reader/WRITER (save path
  mirrors forge bin.cpp: 32KiB inflated chunks, u16 offset table, sentinel pair +
  trailing u32) + UI views: list / tree (widget hierarchy) / survey (810 clean) /
  entry / patch (field-level edit, tag-delimited like defdecode.cpp).
- WORKED EXAMPLES (staged in work/frontend_re/, install untouched, engine-untested):
  (a) healthbar_move: PLAYER_GUI_PC.HealthBarPos (-9,-9) -> (300,-9); forge decodes
  staged root clean 14761/14761, corpus diff = exactly 1 payload. (b) yesno_text:
  UI_YESNO_TEXT.TextValue -> "FORGE: sure?" (945->943B variable-length edit), 810/810
  clean, 1-entry diff. Main-menu background = frontend.big id 1 FRONTEND_BACKDROP_01
  (DXT1 640x480) -- plain texture_build replace.
- EDITABILITY: HUD layout/colors, menu structure/positions/colors/text, GUI textures,
  GUI text, load screens = editable TODAY. Not editable: font glyphs (format
  uncracked), ActionOn* semantics (compiled handlers). Docs: docs/FRONTEND_FORMAT.md.
- Ghidra follow-ups (lock was taken): NUISystem enums (EType/EActionType) + action
  handler map; GraphicIndex bank-binding rule; fonts.big payload struct; engine
  acceptance of ADDED bin entries; Map_JW4ETableSprites inner layout.

## latest+N: DEPLOYED STATE of the game install (2026-07-20 ~13:30)
- Currently live in the install (all backed up in work\smoke_test\backups\ + FinalAlbion_RT.stb added there):
  text.big (FORGE strings — LIVE-PROVEN in save menu), textures.big STAGE2 (correct
  HERO_SKIN_* + 22 overlay sheets green), graphics.big (throw x1.25 + THROW_SLOW),
  Ingame.lug (grass-footstep beeps), FSE_Master.lua (binding test + ForgeTestEnter
  60s teleport thread), ForgeFSE DLL, .qst FSE activation lines, and the STAGE2
  ForgeTest region 5-file set (bwd/wld/wad/tng + rebuilt STB with entry 426/chunk 399).
- Next user-at-home run: load save -> expect green hero, beeping grass, big throw,
  and at 60s the ForgeTest teleport (streaming verdict in FSE log). If world load
  crashes again: pull WER fault offset FIRST, then revert.ps1 + restore
  backups\data\Levels\FinalAlbion_RT.stb.

## latest+N: .fmp SUPPORT CLOSED OUT in FableForge (2026-07-20)
- .fmp (Fable Mod Package, community format from Fable Explorer/ChocolateBox) is
  fully shipped in FableForge (branch gui-m4-start): it IS a Lionhead BIG archive
  (contentType 510), reader/writer = forge::big (libs/forgecore/include/forge/big.hpp),
  ported from decompiled SilverChest BigReader.cs. Entries carry plaintext
  DevSymbolNames + ASCIIZ def-type subheaders (no hash resolution needed); BIN-bank
  payloads are raw compiled-def bytes.
- CLI: forge fmp list [--json] / extract / apply <base-root> <x.fmp> <out-root>
  (non-destructive, builds a drop-in root) / export (game.bin diff -> .fmp) /
  _rewrite (round-trip check); .fmp sources feed forge mods merge.
- Validated today on the only real sample on disk,
  D:\Code\Fable\ChocolateBox\ChocolateBox\ExampleFMPFilesForDev\HalsSword.fmp
  (88,408 B, 17 entries / 6 banks: 5 GameBIN defs for F2_LEGENDARY_LONGSWORD_02 +
  LinkMetaData, 1 graphics mesh, 3 maintextures, 2 text): fmp _rewrite is
  byte-identical (fc /b clean); fmp apply onto the retail root added 5 game.bin
  entries (14761-14765) and every one decodes against the 100% def schema with
  leftover 0. ctest green (synthetic BIG/.fmp fixture in tests/test_formats.cpp).
- Housekeeping: a crashed session's stale pre-SOLVED prototype (forge::fmp custom
  footer parser assuming length-prefixed section names + zlib record streams) was
  deleted; docs/FMP_FORMAT.md build-order marked SHIPPED (commit 666d505).
  ControllerSupport.fmp referenced by the docs is no longer on disk; HalsSword.fmp
  is the surviving reference sample.
- Not done (known gaps, documented): fmp apply skips *LinkMetaData banks (.NET
  BinaryFormatter NameValueCollections used by ChocolateBox for index re-targeting;
  engine never sees them) and script/frontend BIN banks warn-only; fmp export emits
  GameBINEntries only (no link/names generation for Fable-Explorer parity yet).

## latest+N: SAVE EDITOR PRODUCTIZED — entity-graph writes (gold/stats/inventory) + cell-descriptor fix (2026-07-20)
- SAVED_ENTITIES cell RECORD framing decoded (new): each nonempty cell = [u64 0][u64 recLen=
  29+clen][u64 cellId][u64 clen][u32 ulen][clen zlib][9x00]; empty slots all-zero; trailing
  UID->cellId index table (no offsets => no patch needed). Invariant verified 81/81 cells
  across 6 saves (incl. NEW profile "1", fresh 2026-07-20). CORRECTION: earlier gold/quest
  PoCs patched only sectionLen+chunk1_ulen — descriptor recLen/clen/ulen MUST be patched too.
- Inventory record grammar exact: [u32 nameLen][name][u32 qty][u32 unkA][u8 ext](+ext payload).
  unkA nonzero on used tools (fishing rod 65, spade 233) [usage counter?]; money-bag ext =
  15x00 + u32 1 + 00. Fresh-hero component tail = 3 [tag][u32 0] fields (names unrecovered).
- tools/save_edit.py extended (stdlib-only): report | set-gold | set-stat (Money/Morality/
  RenownLevel i32, Age/Fatness f32) | set-qty | add-item (def name validated vs names.bin
  13,593 defs; dupe guard -> set-qty; --force). All writes: hero cell by CTCHeroStats,
  re-deflate, patch descriptor+sectionLen+chunk1_ulen, rebuild + re-sign (seed-0 trailer,
  300KB, zero pad). verify_edit() asserts surgical scope: chunk0 + all non-hero cells +
  chunk1-outside-SAVED_ENTITIES byte-identical; prints hero-cell diff ranges.
- Validated on COPIES (scratchpad; real saves untouched): gold 1625->999999 (3-byte diff),
  Morality 483->-750 (4B), SILVER_KEY qty 7->30 (1B), add HEALTH_POTION x50 + EBONY_KATANA
  (+33/+32B record +count +dataLen only). Edited saves re-parse clean via parse_save_entities
  (348 records, END check 348). getcrc0() helper added; header docstring seed corrected.
- Docs: SAVE_ENTITY_GRAPH.md par.9.5 (records+grammar+editor), SAVE_FORMAT.md (new index),
  SAVE_HERO_STATS.md par.5 correction.
- OPEN / live QA (user away, NO in-game test done): load an edited copy in-game; confirm
  list-front insert order OK, weapon add via general list sufficient?, qty stack caps,
  in-place morality sign flip side effects (CTCHeroMorph mirror?). Ghidra follow-ups (lock
  taken): CTCInventory::Transfer decompile (unkA/ext + tail tag names), SAVED_ENTITIES
  trailing index-table semantics, sectionhead u32 N (206/340) meaning.

## latest+N: HERO OVERLAY AUTHORING TOOL -- custom tattoos via TextureMorphs (2026-07-20)
- NEW tools/hero_overlay.py: PNG glyph + region + trigger stat -> staged (a) NEW textures.big
  GBANK_MAIN_PC entry (DXT3/TransparencyType=1 = the retail blending-overlay format; alpha =
  compositing mask; dims/mips match the region's base sheet -- 512x512/8 for face/arms/torso/
  back/legs, 256/7 mouth; TOC crc 0x7C8D23C4 bank constant) via texture_build.build_entry +
  big_write.rebuild(adds=), and (b) modified game.bin: appends {stat,stat,layer,baseId,newId,2}
  records to TextureMorphs of BOTH CHeroMorphDef entries 10535+11246 (identical retail content,
  owner unresolved -> edit both). game.bin writer = Python mirror of forge::bin::File::save
  (main table/names refs verbatim -> names.bin untouched; chunks re-cut <=32KiB zlib); retail
  roundtrip semantically identical 14761/14761.
- ALWAYS-ON verdict: TextureMorphs has NO appear/disappear floats (ParticleMorphs does) =>
  overlay opacity tracks the stat rank continuously; no stat is provably always-max. --stat
  always = same overlay under stat 6 (evil) AND 7 (good), covering the alignment axis; truly
  guaranteed = --bake-base (also composites glyph into the base HERO_SKIN_* sheet, stage-2-
  proven). Neutral-hero opacity of the 6+7 pair = open question (compositor RE / live A/B).
- WORKED EXAMPLE staged (NOT deployed): work/hero_overlay/build_example.py -> stage/ with
  textures.big (COMP_HERO_SKIN_FACE_FORGE_CROSS id 6291, forehead cross, PSNR 63.1dB) +
  game.bin (156->158 records: {6,6,4,1250,6291,2},{7,7,4,1250,6291,2}) + names.bin copy.
  ALL gates green: untouched entries byte-identical in both containers (6324 tex / 14759 defs),
  edited defs re-parse with preserved tails, forge defs decode leftover 0 on both. Manifest w/
  SHA256s in stage/. Base = ledgered pristine textures.big (stage does NOT carry stage2 green).
- Docs: HERO_MORPH.md new par.3b (overlay format spec + authoring recipe + always-on analysis);
  open questions extended (blend-vs-replace evidence, alignment rank model, re-chunked game.bin
  engine acceptance untested, layer ordering for authored overlays). Ghidra lock was taken all
  session -- compositor questions remain flagged for the next Ghidra run.

## latest+N: PROFILE.BIN CRACKED -- edited saves now installable into the in-game LOAD list (2026-07-20)
- ROOT CAUSE of "hand-copied .sav doesn't show in-game": the game enumerates MANUAL saves
  from the profile registry Profile.bin, not by scanning the dir. Profile.bin is a "FablePro"
  container: 16B header + PLAINTEXT persist-script (Name value;\r\n, latin1, no zlib/no pad) +
  4B trailer. Trailer = seed-0 reflected CRC-32 over file[0:total_data_len] (total_data_len =
  u32 @0x0C = filelen-4) -- SAME primitive as the .sav trailer, and hard-enforced on load by
  CUserProfileManager::VerifySignature @0x00409730 (handles both FableSav + FablePro). Verified
  MATCH on all 4 real profiles (Cornelio/1/Default/Test).
- Registry = 50 fixed text slots SaveGameNames1..50 "<name>";. Slot shown in LOAD list IFF
  string non-empty; string is BOTH the label AND the exact on-disk filename (no GUID/slot-id/
  embedded save-id). AutoSave/AutoSave.qs are reserved special slots, NOT registered here (only
  manual saves). Cross-check: profile "1" has AutoSave but 0 occupied slots. Per-slot LOAD-screen
  metadata (hero/chapter/playtime/thumb) is read lazily from the .sav HEADER at enumerate time,
  NOT stored in Profile.bin -> a valid edited .sav displays correctly with no extra work.
- CHECKSUM VERDICT: the seed-0 CRC trailer is the ONLY thing blocking a naive edit -- must be
  recomputed after any text change or the whole profile fails to load. Nothing hashes the .sav
  files, so overwriting an existing listed slot's file needs NO Profile.bin edit.
- TWO transfer procedures: (a) overwrite an existing occupied slot's file in place (simplest,
  Profile.bin untouched); (b) add SaveGameNamesN + re-sign Profile.bin (keeps existing saves).
- NEW tools/save_install.py (stdlib-only): inspect <profileDir> (lists occupied slots + flags
  UNREGISTERED files) | install <edited.sav> <profileDir> <slotName> [--out-dir DIR] [--apply].
  slotName = "new" | existing-name (overwrite) | new-name. Default STAGES corrected Profile.bin
  + renamed .sav + INSTALL.txt (real dir untouched); --apply writes real dir (backs up
  Profile.bin.bak). Every build recomputes+self-verifies the trailer and re-parses before write.
- STAGED FIX (not deployed) for the gold save in work/save_install/: Profile.bin (18102B, adds
  SaveGameNames2 "Manual - Save2", trailer re-signed A34E698D, slot 1 preserved, delta=+14 =
  the 14-char filename) + Manual - Save2 (307200B) + INSTALL.txt. Re-decode confirms occupied
  {1:"Manual - Save1", 2:"Manual - Save2"}, verify=True, no padding.
- Docs: docs/SAVE_PROFILE_INDEX.md (full format + both transfer procedures + checksum notes).
- OPEN / needs LIVE test (user away, real Profile.bin NOT touched, game NOT launched): load an
  edited copy in-game to confirm the added slot appears + loads, and that entering the LOAD menu
  doesn't rewrite Profile.bin and drop the externally-added slot before you can load it (it
  rewrites on SAVE, not on browse -- expected fine, but only a live run proves it).

## Current resume point (2026-07-20, latest) — CUSTOM-CONTENT END-TO-END + DEBUG BUILD unlock

**PROVEN LIVE this session — custom region, entered + walked from an ADULT save:**
- ForgeFSE `Quest:GoToMapSlot(mapSlot,x,y,z)` = the real cross-region hero move. Final impl (commit `926609a` on `rebase-on-fse_source`, chain e52eff6->5fb03f7->70ccc72->89d75cf->926609a): `GetRegionNumberMapIsIn(slot)` @0x4FC190 -> if >0 `CWorldMap::LoadRegion(regionIdx, ELoadType0, force=true)` @0x500540 (synchronous, no adjacency check) -> `SetPlayerPos` @0x5063E0 -> `CWorldMap::ActivateNavMap(slot)` @0x50AF10 (nav go-live) -> `EntityTeleportToPosition` (hero move). DLL is 5,209,600 B (static /MT); also added diag bindings IsHeroControlledByPlayer/IsEntityUnderScriptedControl/IsRegionLoadedAndPreloaded.
- KEY FACTS: the engine reads **FinalAlbion.bwd** (loose), NOT .wld/WAD, for region membership. ForgeTest map slot 399 was the lone-tail region (142) -> `GetRegionNumberMapIsIn` returned 0 -> fixed by adding 399 to **Filler_Darkwood_03 (region 95) containsMaps+seesMaps** in the .bwd. Nav needs `NAVIGATION_SEED`+`REGION_ENTRANCE_POINT` in the TNG (walkable-island rule) AND the map registered as a proper region member so `LoadRegion` runs the full go-live (that fixed BOTH distant-teleport and the frozen-hero-walk).

**FableForge commits (branch gui-m4-start):** 06e3d56 GoToMap/GoToMapSlot quest nodes; 518a411 `forge quest card` (clone+patch OBJECT_QUEST_CARD_* pair, offset-81 link); 9b06ed9 GUI Verify+Package panels; 0a837a6 GUI Cutscenes panel; bc0cecc `forge defs set-field` (typed single-field editor for game.bin/frontend.bin).

**TERRAIN (void root cause + fix STAGED):** ForgeTest renders void because retail draws the landscape MESH from the per-map chunk in **FinalAlbion_RT.stb** (CLocalDetailCacheMap::OpenStaticMap @0xBDF010), NOT the LEV grid — and ForgeTest's chunk was a byte-clone of empty filler Darkwood_Filler_08. FIX (staged work/forgetest_terrain/A_matched + folded into work/smoke_test/stage): STB chunk (entry 426) + WAD ForgeTest.lev + TNG swapped to donor **Darkwood9_Leadout_01** (real 32x32 Darkwood outdoor map). Teleport lua retargeted z 74->41 (donor ground). deploy.ps1 $Replace now includes FinalAlbion_RT.stb (retail backup present).

**OPEN — CARD:** appending a NEW def to game.bin (OBJECT_QUEST_CARD_FORGE_TEST entries 14761/14762) FAILS engine acceptance: name resolves (names.bin->14762) but `GiveHeroQuestCardDirectly` @0x8968c0 silently fails to create/activate the thing (no card in any Quests tab, and it corrupted the quest-screen text render). FIX PATH: OVERWRITE an EXISTING OBJECT_QUEST_CARD_* in place (no append, def count unchanged) via forge defs set-field, give that name. Being decided by workflow figure-out-fixes.

**OPEN — AUDIO:** swapped Ingame.lug clips (cat-meow, byte-valid, fmt==retail, step-index<0x58, in place) are SILENT while non-swapped play. Sounds are .lug/.met/.ogg in data\Sound (NOT .big). Suspected runtime gate FUN_00c4de00 (alBufferData caller) reading a cached length/duration/offset from a table we didn't patch. Being RE'd by workflow figure-out-fixes.

**SAVE childhood:** save_edit.py-rebuilt saves boot the CHILDHOOD intro; zlib-level fix DISPROVEN live. Proven cause = the REBUILD itself (Save3 = byte-exact copy of adult Save1 as external slot -> loads ADULT; so not the slot, not zlib -> the save_edit reframe/rebuild breaks world-restore). Unfixed.

**DEBUG BUILD UNLOCK (game-changer):** Fable Anniversary 2013-02-25 (D:\Downloads\Fable_Anniversary-2013-02-25.7z, 3.5GB) already extracted + PDB-applied + BSim-ported in the hub (symbols/, debug_build/, ghidra_out/fablewin_editor_symbols.tsv, egor_pdb_names.tsv). **FableWin.exe = the level EDITOR with the STB-LANDSCAPE BAKER retail strips** — symbolized: CEditControlCentre::SaveLevel @0x020377e0, CLandscapeBackgroundTreeNode::SavePatchesToTemporyStream @0x02deb570, CEngineMap::WriteMapInfoBlock @0x02d67ea0, CEngineLocalDetailLayer/ObjectDef::Transfer, GConsoleAddForcedStaticMapUpdateMap. So custom STB landscapes (author terrain from scratch) = CRACKABLE, not blocked. Ego_d.pdb (269MB) full symbols re-illuminate the whole engine.

**RUNNING WORKFLOWS (resume via Workflow({scriptPath, resumeFromRunId}) — completed agents return cached):**
- figure-out-fixes (card+audio adversarial): runId `wf_4b931779-671`
- import-spiral-mountain (Banjo-Tooie glTF -> mesh placed in ForgeTest): runId `wf_96035f20-36e`
- crack-stb-landscape-baker (FableWin baker -> author terrain): runId `wf_8b43d003-55a`
Scripts under the session workflows/scripts dir; run IDs above.

**DEPLOYED (messy smoke-test pile) in the install:** custom game.bin (card, +.retail-bak), region-fixed .bwd, instrumented DLL 5209600, cat-meow Ingame.lug, FORGE text/textures/graphics. `revert.ps1` restores retail (Steam verify also reverts). Profile.bin has Save1(adult)/Save2(gold, childhood-bug)/Save3(adult copy). NEXT: consolidate to ONE clean test = revert.ps1 -> deploy only verified fixes (terrain A_matched + overwrite-card + audio-fix + region .bwd + DLL + teleport lua). deploy.ps1 in work/smoke_test/ manages the file set.

## Current resume point (2026-07-21, ULTRACODE session) — naming surge + VC7.1 SOLVED + gen headers

**NAMING (evidence-based, workflow-driven).** Built an autonomous naming pipeline:
`DumpNamingBatch.java` exports the top-N most-referenced `FUN_*` (decomp + named callers +
callees + string refs) -> split into batches -> a name->adversarial-check Workflow proposes +
verifies names -> `LabelApply.java` applies high+med confidence with `[forge-autoname]` audit
comments (revert TSVs kept). Two tranches applied: **+913 (tranche1) +1,380 (tranche2) = 2,293
new evidence-based names, 0 failed.** Coverage: FUN_* **8,899 -> 7,065**; named **~35.5k ->
42,488 (~86%)**. Tranche 3 (next 2,000) running (`wf_935a4f1c-69b`). Corpus/labels/reverts in
`ghidra_out/naming_*`. Because applied names are no longer FUN_*, re-running the exporter auto-
advances to the next tranche.

**VC7.1 / MSVC 2003 = SOLVED (the bar-#2 blocker).** Portable toolchain at **`D:\Tools\vc71`**
(`cl.exe` **13.10.3077** — Fable's exact compiler; compiles+links+runs verified). Obtained from
the VS.NET 2003 Pro ISO (archive.org **X09-46364**; Disc1 .7z -> Alcohol .mdf **2448-byte
sectors** -> ISO via `scratchpad/mdf2iso.py` slicing [16:2064]/sector -> 7z-extract Vc7 +
`msvcp71.dll` KEY: c1xx.dll needs it). `vc71env.bat` activates. GAP: no Win32 PlatformSDK
(windows.h) on Disc1 — CRT/STL only; add a 2003 Platform SDK for engine TUs. See memory
[[vc71-toolchain-installed]]. LIFT_HARNESS bar-#2 oracle prerequisite now met.

**GENERATED HEADERS (FableForge `generated/`).** `ExportEngineApi.java` -> `engine_api.tsv`
(41,108 named fns + ABI) -> `gen_fable_engine_header.py` -> **`fable_engine.h`** (all engine VAs,
`FABLE_FN` cast macro) + **`fable_cgsi_api.h`** (891 CGameScriptInterface script-API methods, all
with signatures). These are the injector/ForgeFSE binding substrate.

**REVIEWS + PLANS.** Full correctness reviews of ForgeFSE (bugs: B1 5 entity cmds skip control
acquisition; B2 quest-host+VM leak every level; B4 UAF; + unbound high-value verbs — conversation
builder unusable, house/minigame) and FQT (qst-writer wipes 112 AddTestQuest lines; BIG reader
layout wrong; region-name drift). Integration design -> `D:\Code\FableForge\docs\FORGE_INTEGRATION_PLAN.md`
(FableForge already has native successors; generate ForgeFSE bindings from DB + emit gamedata
catalog for FQT). Prioritized 149-gap backlog -> `docs/BACKLOG.md` (two north stars: decomp +
injection-first). Fable2 TU1 for a Discord user = `D:\Documents\Fable2RE\default_tu1.xex` (21MB;
the .zip is an empty stub).

**Next:** apply tranche 3; continue naming tranches; type/cc recovery pass (raise typed metric);
PDB struct-layout -> fable_types.h; source-tree reconstruction from debug PDB (decomp scaffold);
CGSI vtable slot-order dump (for ForgeFSE slot bindings); stand up LIFT_HARNESS chest/reward
trial on the new VC7.1.

## Resume point (2026-07-21, ULTRACODE cont'd) — terrain codec + lift harness PROVEN + ForgeFSE built

**BAR #2 LIFT HARNESS PROVEN (first matching function).** `lift/` stood up: VC7.1 (D:\Tools\vc71)
compiles a lifted TU and `build_and_diff.ps1` diffs .text vs retail. First target
`CTCPhysicsBase::GetAirResistance @0x00662030` lifted -> **byte-identical** (`8b81e0000000d94078c3`
== retail). Pipeline proven: Ghidra pseudo-C -> clean C++ -> VC7.1 -> matching bytes. Tooling:
DumpFuncBytesAndC.java (decomp+len+bytes oracle), lift/README.md. Scaling deps all present
(fable_types.h layouts, decomp_module_scaffold.tsv 3,433 class-modules, 99.998% clean decomp).

**TERRAIN CODEC SHIPPED (native bake crux).** forge::rangecodec (libs/forgecore/src/rangecodec.cpp)
ports CRangeCompressor::Decompress @0x00f39ed0: encodeRaw emits the flags==0 stored block
[0x00][raw] the engine decodes byte-exact (the VB/IB authoring path, no arithmetic-coder parity),
decode() is the full range/bit-unpack (read donor blocks). Built + round-trip test green
(testRangeCodec). Commit 090ff27. This + emitChunk's FrameEdit path = the native terrain bake;
docs/TERRAIN_NATIVE_BAKE.md has the 8-step plan (STEP 0 CRangeCompressor RAW now RESOLVED).

**FORGEFSE BINDINGS BUILT.** 10 CGSI verbs exposed to Lua (conversation 364/365, house/door
426-432, fishing/digging 247-249) — commit a479259 on rebase-on-fse_source. Built clean with VS18
MSBuild Release|x86 -> FableScriptExtender.dll (4.48MB; note /MD vs the live-proven 5.2MB /MT —
rebuild /MT for in-game). FSE needs NO game SDK (hooks by address); MSVC + Windows SDK + vendored
lua/sol2 suffice.

**NAMING FINISH running** (wf_ea091211-481, 86 batches = all 3,413 remaining FUN_*). Coverage
before this: 93.1% named. **Type recovery (#10):** 71,511 donor sigs dumped + 917/921 CGSI verbs
demangled via VC7.1 undname.

**Decomp distribution model** (user Q): ship source + RE facts only; user supplies their own
Fable.exe; NEVER redistribute the retail binary, assets, or the debug PDBs. Matching-decomp +
build-from-user-copy is the legally-defensible pattern.

## latest+N: lift harness scales to a MODULE + naming finish underway (2026-07-21)
- LIFT MILESTONE: whole-class lift proven. CMouseDX accessor surface (10 methods:
  PeekPos/GetPos/SetInputOn/SetRecentringOn/IsRecentringOn/SetDrawCursor/
  IsDrawingCursor/IsLMBDown/IsMMBDown/IsRMBDown) all recompile BYTE-IDENTICAL to
  retail under VC7.1. Harness: lift/scripts/build_module_diff.sh (compile via PowerShell to
  avoid MSYS /flag mangling; objdump -d per-symbol byte diff vs lift/cmouse_dx.expected.tsv).
  Lesson: VC7.1 emits true __thiscall only for real member fns -> lift methods-with-args
  as members of a padded class (unsigned char m_[0x4850] at off 0, non-virtual -> no vptr
  shift -> raw offsets == retail ecx+disp). Single-fn __fastcall(self,edx) shortcut only
  works for 0-arg getters. lift/README.md updated.
- NAMING FINISH: 624 remaining FUN_* exported (naming_corpus6.txt), split into 8 batches
  (ghidra_out/naming_batches6/), 8 parallel naming subagents running -> out_NN.tsv. Apply
  high+medium via LabelApply when they land; target ~99.5%+ named.
- NEXT LIFT after this: pick the next small self-contained class (e.g. CMemoryAllocatorFixedSize
  accessors, or a math/vector class) and repeat build_module_diff.sh; then wire the
  auto-re-agent reverser/checker loop to drive it with the byte-diff as the parity gate.

## latest+N: NAMING 100% COMPLETE + lift Tier-0 module #3 (2026-07-21)
- NAMING FINISHED: tranche-6 applied 624 names (20 high / 448 medium / 156 low, all
  addr-suffixed for uniqueness, [conf] tag in each audit comment). DumpStats:
  default_named=0, named=49553/49553 -> 100% NAMED. (3.9% project start -> 80% session
  start -> 100%.) Source: labels_forge_autoname_t6_all.tsv (git-tracked, revertible).
  Engine header + engine_api.tsv regenerated at 49553 symbols.
- LIFT module #3: CMemoryAllocatorFixedSize (lift/cmemalloc.cpp) — 5/7 BYTE-IDENTICAL
  incl. free-list-walk loops (GetNoAllocatedAreas matches the 8d642400 loop-align nop;
  GetTotalMemoryControlledByAllocator; GetLargestFreeAreaSize after subtraction-test
  massage). 2/7 (GetNoFreeAreas, GetHeaderOverhead) are functional-grade: identical loop
  structure, only the hoisted-member register differs (retail reuses ecx/this, VC7.1
  picked esi). GetFragmentation OMITTED = link-time-only (fld [abs global 0x122dedc]).
  Running tally across 3 modules: 16/18 matching-grade, 2/18 functional-grade.
- Lift lessons captured: (1) load member VALUES not &addresses to match retail; (2) hoist
  loop-invariant members; (3) test-by-subtraction not cmp when retail does; (4) functions
  referencing absolute globals can't byte-match standalone (need full-image link).

## latest+N: terrain per-chunk WRITE LAYER shipped (STEP 1/2/5) (2026-07-21)
- forge::stbinfo (STEP 1): read/writeInfoBlock for 0x5C CStaticMapInfoBlock, engine on-disk
  order from stb_infoblock_baker.py; golden-vector validated (testInfoBlock).
- alignUp + EmitOptions.frameAlign (STEP 2): page-align re-laid frames (0x1000); align=0 keeps
  byte-identity; align=0x1000 -> all 26 donor frames page-aligned (testEmitAlign). Aligned emit
  not byte-identical to the retargeted (non-aligned) fixture — retail-chunk byte-repro unvalidated.
- generateQuadDir (STEP 5): inverse of parseQuadDir; reproduces donor's 4 quad entries byte-exact
  (testQuadDirGen). Commit 0733482.
- REMAINING (container-splice layer): STEP 6 InfoBlock rebase is BLOCKED ON RE (must map which
  InfoBlock body-pointer targets which frame before rebasing landscape/localDetailMapPtr); STEP 7
  forge::stb::save() needs a container WRITER (only reader exists) + confirmed entry CRC rule.
  See docs/TERRAIN_NATIVE_BAKE.md "STATUS 2026-07-21 (2nd update)".

## latest+N: terrain STEP 6 closed — InfoBlock positions are logical, not physical (2026-07-21)
- Editor write-order proof: LandscapeMapPtr/LocalDetailMapPtr/HeaderEndPtr are GetPosition snapshots
  around GenerateStaticMapEntry calls in the logical control stream. Retail Darkwood_Filler_08
  corroborates it (landscape→detail delta `0x15`, impossible as a physical frame-region span).
- CONSEQUENCE: physical LZO span changes/page re-layout do NOT rebase InfoBlock fields. The minimal
  same-topology height writer preserves decoded body lengths, so logical positions stay invariant;
  only quad frame offsets/spans move and are already rewired.
- forge::stbbake::emitChunk now accepts same-decoded-length recompression/edits and rejects broader
  decoded-length changes. Fixed walkFramedBlocks to accept legal LZO1X-1 expansion on incompressible
  bodies. Full forge_tests green; real donor recompress round-trips retain 18/18 and 26/26 frames.
- REMAINING terrain splice task: STEP 7 forge::stb::save/container writer + CRC confirmation.

## latest+N: terrain STEP 7 SHIPPED — native STB container append (2026-07-21)
- Added forge::stb::appendStaticMap + `forge stb append` and `forge stb record`. Writer appends an
  aligned payload, rebuilds the common index, rebases the full common record, clones/extends the TOC,
  and preserves all existing payload offsets.
- Corrected the old sketch: common index offsets target the full InfoBlock+control-subheader record,
  not only 0x5C and not the physical chunk. Required rebases: record +20/+24/+58/+5C/+71; bank id +04.
- CRC settled empirically: 424/424 retail STB entry CRC fields are zero; new entry CRC=0.
- Full tests green. Retail G9: pristine 597,979,518-byte file -> 425 entries / 399 maps, ForgeTest id
  426 aligned at 2048, original prefix byte-identical except header dwords, 750-byte common record
  diffs confined to bank id+five pointers, appended payload SHA-256 f9f96ba9...55ed.
- Staged, NOT deployed: work/forgetest_bake/FinalAlbion_RT_step7.stb. Terrain offline writer path
  STEP 0-7 is complete; remaining proof is live engine acceptance of this staged archive.

## Resume point (2026-07-22 00:10 MDT) - Mario animated hero still stretches; auto-RE healthy

### User-observed state (authoritative)

- Latest live test was `work/mario_hero/stage_bindaxis4`, deployed to the retail install and
  launched through `FSE_Launcher.exe`.
- Result: **looked better, but every Mario body segment is still stretched/pulled apart during
  gameplay animation.** Treat this stage as a failed experiment, not a finished rigging fix.
- Earlier tests established that the inventory/menu rest preview can look assembled while gameplay
  animation separates the limbs. This is an animation/bind-space problem, not merely placement.
- **Textures are still visibly wrong in the live `stage_bindaxis4` test.** The atlas-row change
  was not a completed texture fix; debug UV orientation/row selection and the staged GBANK entry
  independently from the rig before calling the material path solved.

### Exact Mario artifacts and installed test build

- Source: `D:\Downloads\Nintendo 64 - Super Mario 64 - Playable Characters - Mario.zip`
- Extracted source: `work/mario_hero/src/` (`mario.fbx`, `mario.dae`, textures).
- Repro scripts: `export_mario_retarget_source.py`, `build_mario_atlas.py`,
  `build_mario_animated.py`, and `stage_texture.py`, all under `work/mario_hero/`.
- Latest stage: `work/mario_hero/stage_bindaxis4/`.
- Installed hashes at launch:
  - graphics.big: `AC6B2863D2DA9B3539EFDF7AA0974B050E00E94765FEF4CE3946ED4F912763B6`
  - textures.big: `024A650738583A44EC6EFDA9DF3A2B9490593E9A8F848669648D91B9197027AE`
- Clean script inputs/backups remain beside the install files as
  `graphics.big.bak-20260721` and `textures.big.bak-20260721`.

### Why stage_bindaxis4 is not the solution

- It maps Mario's supplied 27-bone FBX rig to 15 deform regions on the cloned retail 77-bone
  `MESH_HERO` skeleton. All 2256 emitted vertices use rigid one-bone weights.
- It moves arm/leg pieces from Mario bind axes onto hero bind axes and scales the four long limb
  chains. Head/torso retain the uniform silhouette fit.
- The primary visible failure is a **Mario-vs-hero scale/proportion mismatch**: gameplay animation
  restores hero-sized parent-relative joint translations, pulling Mario's smaller disconnected
  segments outward until they meet the hero joint positions. This is why the assembled rest/menu
  pose does not predict the animated result.
- Format checks pass (type 5, 77 bones, 752 faces, 15-slot palette, normalized weights, sane
  `Z 3.1..167.7` bbox). Those checks prove file validity only, not animation compatibility.
- Engine fact from `io_scene_fable/anim_ops.py`: Fable animation position keys are parent-relative
  local translations in bind units; stationary keys equal mesh bind-local transforms. Changing
  mesh rest/inverse-bind data alone is incomplete because hero animation translations can restore
  hero proportions. The live test falsified continued per-piece axis tweaking.

### Recommended next Mario direction

1. Decode one representative hero walk clip and compare its local position tracks against both the
   retail hero bind locals and Mario FBX bind locals (`anim_ops.py` / `tools/anim_build.py`).
2. Test one explicit strategy on walk only: retarget translation tracks to Mario proportions, or
   conform Mario to the retail hero rig with blended joint weights plus overlap/bridge geometry.
3. The disconnected N64 segments plus rigid one-influence weights are intrinsically gap-prone.
   The cheapest next probe is two-bone blending around shoulders/elbows/hips/knees and overlapping
   seam rings before considering a whole animation-bank rewrite.
4. Restore clean BIG backups before a baseline; deploy only while `Fable.exe` is closed.
5. Diagnose the texture path as a separate baseline test (static/rest pose, known UV checker or
   labeled atlas) so rig deformation cannot obscure whether UV V-flip, atlas rows, or GBANK
   staging is wrong.

### Auto-RE loop / terrain / quest state

- Auto-RE Windows fixes are in `D:\Documents\Fable2RE\auto-re-agent\src\re_agent\llm\codex_cli.py`
  and `core/session.py` (stdin prompts, direct Node codex.js, UTF-8 replacement, atomic replace).
- Completed: `0x008ED590` PASS/2 rounds; `0x00BF7400` LoadHeader PASS/2;
  `0x00BF6720` UpdateTesselation PASS/1; `0x00BF8810` UpdateStaticLOD PASS/4.
- Latest output is `lift/re-agent-staticlod.stdout.log`; no re-agent process remained after PASS.
- The 2026-07-22 00:04 FSE log decisively proves the in-box ForgeTest load: `GoToMapSlot(399)`
  force-loaded host region 95, ground lookup returned `39.8169`, the hero landed at
  `(2800,2576,40.5)`, `GetCurrentMapName()` repeatedly returned `ForgeTest`, and player control
  recovered after four seconds. The earlier donor-coordinate test entered actual Darkwood, but
  it is superseded by this in-box result. `IsRegionLoaded("ForgeTest") == false` is not a map-load
  failure: ForgeTest is hosted by region 95 (`Filler_Darkwood_03`), so that name queries the wrong
  region identity. Duplicate membership in regions 95 and 142 should be cleaned up, but it is not
  the current blocker. The remaining live unknown is specifically whether ForgeTest's STB
  landscape resources bind and submit for rendering.
- Terrain authoring itself was not abandoned: native STB codec/chunk/container work reached STEP
  0-7 and produced the offline-validated, **not yet live-proven** archive
  `work/forgetest_bake/FinalAlbion_RT_step7.stb`. Resume terrain by instrumenting
  `OpenRetailStaticMap`/bank/theme/landscape submission, establish a known-good retail donor
  telemetry signature, and compare the installed `A_matched` ForgeTest run against it. Only then
  live-test STEP-7 in a clean terrain-only deployment. Do not fold it into another Mario smoke test.
- Quest card exists but still lacks objective/gold/renown values; activation succeeds. Resume from
  `docs/QUEST_CARD_EMPTY_FIX.md` and inspect the runtime setter/card-THING lookup.

### Terrain runtime probe update (2026-07-22 00:28 MDT)

- Clean terrain-only FSE run completed with installed `A_matched` STB SHA-256
  `8AF88BC8AD3106E403CD690B28B20A48AB235ED14B3A3FFBFF5B7E74DC9512DD`.
- Unconditional `OpenRetailStaticMap` telemetry proves the engine opened
  `Data\Levels\FinalAlbion\ForgeTest.lev` through **bank entry 426**, with state 1 and runtime
  bounds `(2784,2560,37.8359)..(2816,2592,83)`. Manifest lookup, own-name binding, entry selection,
  and map bounds are therefore GOOD and must not remain white-out root-cause suspects.
- `GoToMapSlot(399)` again loaded host region 95; ground returned 39.8169; current map was
  ForgeTest; control recovered; the hero moved across `(2800,2576)` to `(2812,2579)` while Z
  followed 40->38. Map identity, navigation, and collision are live across the box.
- Still unproven: whether `CEngineLandscapeMap::OpenStaticMap` loads non-empty patch/tree state and
  whether patches reach renderer submission. The investigation now starts at landscape header,
  quadtree/patch load, themes, culling, and draw submission—not WLD lookup or STB entry 426.
- Auto-RE `CLocalDetailCacheMap::OpenStaticMap @0x00BDF010` completed PASS in 1 round. It stores the
  file-block manager, reads a 12-byte chunk header, seeks its source chunk, then loads the local
  detail quadtree header and object-type palette. Next loop target running:
  `CEngineLandscapeMap::OpenStaticMap @0x00BDD0E0`.

### Native authored heightfield update (2026-07-22 00:40 MDT)

- Root cause of the loaded-but-invisible donor test is now byte-proven: the retarget script moved
  only outer quadtree/AABB metadata. Actual decoded patch vertices remained at donor coordinates
  `X=2816..2848, Y=2368..2400`, outside ForgeTest's registered
  `X=2784..2816, Y=2560..2592` box.
- Fixed `parsePatchBody` false-positive VB detection by requiring the decoded vertex coordinates to
  form the exact `(pw+1) x (ph+1)` Cartesian grid. All four donor patches now decode correctly.
- Added `forge stb patchinfo` and `forge stb bake-heightfield`. The latter preserves donor texture,
  indices and trailer, but rewrites full 16-byte vertex records: grid X/Y into the ForgeTest box,
  Z from the authored LEV, and normals from central height differences.
- Generated `work/forgetest_heightfield/ForgeTest_chunk_heightfield.bin`: four 16x16 patches,
  33x33 shared heightfield coverage, `Z=37.685..56.198`, center `Z=53.817`. Offline gates PASS:
  26/26 frames decode, page alignment, 4/4 quadtree wiring, frame-count and quadtree-shape parity.
- Packaged fresh retail-based `FinalAlbion_RT_heightfield.stb`, SHA-256
  `76053D1D796D0B83F2CB122C0F83A5096730227E66BA0525849D4753C6232FB2`; extracted entry 426 is
  byte-identical to the authored chunk (`0D376E43...`). Repacked `FinalAlbion_heightfield.wad`
  with the matching authored LEV, SHA-256 `EBA7F1117A12049B7E9CF4F734F9EE636D69E2852788482CBCE6C85C4CB9625A`.
- Deployed as the terrain-only probe and launched through FSE. Expected decisive runtime changes:
  entry 426 still opens; ground at center rises from ~39.817 to ~53.817; the hill mesh becomes
  visible. The 32x32 footprint is small because map slot 399 itself is registered as 32x32;
  enlargement needs a larger map registration/topology and is not part of this same-topology gate.
- Auto-RE `CEngineLandscapeMap::OpenStaticMap @0x00BDD0E0` completed PASS in 2 rounds. Next loop:
  `CEngineLandscapeRenderer::BuildThemes @0x00B687D0`.

### Fixed-layout heightfield live proof and render fault isolation (2026-07-22 01:30 MDT)

- The first authored chunk above was not load-safe despite offline frame decoding: its RAW inner
  VBs enlarged the chunk to 193,853 bytes and shifted later foreground/local-detail blocks. The
  game opened entry 426, resolved center ground to `54.8169`, teleported, then hung. Full dump:
  `work/terrain_runtime_probe/Fable_heightfield_hang_20260722_0045.dmp` (459,484,187 bytes).
- Forge now emits compact CRange descriptor streams, uses retail `lzo1x_999`, and can preserve every
  donor segment start/span. The resulting authored chunks are exactly 132,632 bytes and leave all
  later file-block references plus the four quadtree records in place. Decoder bounds checks were
  also added after malformed candidate streams exposed a parser heap overwrite. All forge tests pass.
- `forge::stb::replaceEntryPayload` now performs a true byte-in-place replacement when payload size
  matches. This preserves the full 598,307,320-byte STB, TOC at 598,247,424, ForgeTest entry offset
  598,114,304, and the common manifest. A regression test covers this path.
- Live FSE proof with fixed physical layout is clean: ForgeTest opens as bank entry 426, center
  ground is `54.8169`, hero arrives at `(2800,2576,54.5)`, control returns at +4 seconds, and the
  hero traverses downhill to Z40-range without a hang. Therefore the authored heightfield mesh,
  collision, map streaming, and control handoff are all real. Screenshot:
  `work/terrain_runtime_probe/fixedlayout_live.png`.
- The remaining defect is render appearance, not missing geometry. The hill silhouette is visible
  but black. A one-donor-normal-per-patch probe produced a transient overexposed/white scene;
  height-gradient normals did not restore material color. The latter uses 147-217 distinct packed
  normals per patch and is live-proven responsive; screenshot:
  `work/terrain_runtime_probe/slopenormal_live.png`.
- Normal format is no longer speculative. Retail flat donor vertices contain `0x7FC00000`, exactly
  Forge's `(0,0,1)` 11/11/10 pack. Recomputing the donor's own slopes from its LEV yields the same
  signs/axes and close component values. The two tail fields are also not world-coordinate offsets:
  each is the preserved 17-value local `0..255` grid in every donor/authored patch.
- Current authored slope-normal chunk:
  `work/forgetest_heightfield/ForgeTest_chunk_heightfield_slopenormal.bin`, SHA-256
  `EEA2B6665B014CE95067070050946F81975DB6CA387178666E299B22DCBC3C59`, 26/26 frames decode,
  4/4 quadtree records resolve, frame-count/quadtree parity PASS. Current staged/installed STB:
  `work/forgetest_heightfield/FinalAlbion_RT_heightfield_slopenormal.stb`, SHA-256
  `D2CAF893E99BD087FD2B2F934DA21DE7FC018020449F73276551F9E7E98730A1`. Matching WAD remains
  SHA-256 `EBA7F1117A12049B7E9CF4F734F9EE636D69E2852788482CBCE6C85C4CB9625A`; a fresh extract proves
  its LEV height range is `37.685..56.198`, center `53.817`.
- `BuildThemes @0x00B687D0` completed PASS and confirms the install-global, name-agnostic
  `ENGINE_THEME` construction; do not return to bank-name/theme-registration theories.
  `CEngineLandscapeMap::UpdateForeground @0x00BDC510` exhausted four review rounds with objective
  structure PASS but final reviewer FAIL only on exact counted-pointer helper call semantics. Its
  useful load-path result is clear: it streams `patchRows*patchColumns*0x24` header records, calls
  `CEngineLandscapePatch::LoadHeader`, then delegates each patch to its own `UpdateForeground`.
- `CEngineLandscapePatch::UpdateForeground @0x00BF4220` completed four rounds with objective
  structure PASS; final semantic review only rejected temporary/refcount cleanup placement. The
  render logic is usable: near patches call `LoadForegroundPatch`, then `LoadTextures(distanceSq)`,
  and set the foreground-VB/draw flag only after background full-detail coverage is present.
- Active background loop at handoff: `CEngineLandscapePatch::LoadTextures @0x00BF4130`, parent
  `re-agent.exe` PID 16512 / Python PID 16664 when started. Fresh logs are named
  `lift/re-agent-landscapepatch-loadtextures-20260722-014444.*.log`.
- A coordinate-only donor control was explored but deliberately not exposed/deployed. Its exact
  inner CRange coordinate bias can be retargeted without changing the VB span, but the modified
  bytes make the outer LZO frames slightly larger and two run-on frames overlap their next donor
  segments. Relocating those frames is unsafe until the 0x24 patch header offset/span references
  are parsed and rewritten. The new `rangecodec::addColumnConstant` primitive is decode-verified
  and covered by tests; the incomplete CLI was removed.
- Next evidence-driven steps: finish that delegate loop; reverse `CLandscapeBackgroundPatch::Load
  @0x00BE7D70` / texture binding calls; verify each 0x24 patch header's offset+span reaches the four
  edited frames; compare inline texture/palette handles against a known-good retail map; then run a
  coordinate-only donor-vertex control. Do not enlarge the map yet. The current 32x32 size is the
  registered same-topology gate; expansion requires coordinated WLD/LEV/chunk topology changes after
  the foreground material path works.

### Texture readiness proof and render-loop restart (2026-07-22 01:55 MDT)

- `CEngineLandscapePatch::LoadTextures @0x00BF4130` completed PASS in two rounds. The recovered
  implementation verifies every foreground texture index through the global texture table, clears
  patch flag bit 0 while any resource is unavailable, records the per-frame foreground texture use,
  and returns bit 0 as the ready result.
- Live observational telemetry agrees with that implementation. ForgeTest patches briefly reported
  resource-null states while streaming (`result=0`, `missing>0`, flags `0xDA/0x1A`), then the same
  patches reached `result=1`, `missing=0`, with flags advancing through `0xDB/0x1B` to
  `0xDF/0x1F`. The global texture table held 6,292 entries and every sampled steady-state texture
  index resolved to a ready handle/resource.
- The FSE run stayed healthy through its final sample: center ground `54.8169`, hero at Z54 after
  control recovery, then walking downhill to Z40 by +10/+12 seconds. Windows recorded no Fable
  Application Error or Application Hang event when the user closed the game. The file
  `work/terrain_runtime_probe/textureprobe_live.png` was captured after Fable had already closed and
  only shows the desktop; do not use it as terrain-render evidence.
- Diagnostic ForgeFSE DLL deployed for that run: SHA-256
  `9EBB6CCE34C3ADAB6CE3CA7B7F2B16F7090F68F50B05109DC3901C2CD69B59C9`.
- Missing mesh, failed landscape streaming, bad normal packing, theme registration, and unresolved
  steady-state texture handles are now separately eliminated. At this point the black terrain fault
  was known to be downstream in landscape drawing, but foreground versus background submission had
  not yet been separated.
- The first Render-loop restart at 01:55 failed immediately because the detached process could not
  resolve the Codex CLI; its `015522` stderr log records that environment-only failure. Auto-RE was
  restarted successfully with explicit Node/Codex paths at 01:57 MDT on
  `CLandscapeBackgroundPatch::Render @0x00BE6F70`: launcher PID 17716, Python PID 16732, and active
  Codex child PID 17280 at verification. Logs:
  `lift/re-agent-backgroundpatch-render-20260722-015704.stdout.log` and `.stderr.log`. Derive any
  render telemetry hook from the verified decompilation; do not guess at draw-call fields or patch
  offsets. After that, compare ForgeTest against an unmodified retail patch at the material/texture
  binding boundary and modify only the first proven divergence.

### Background/foreground render separation (2026-07-22 02:16 MDT)

- `CLandscapeBackgroundPatch::Render @0x00BE6F70` completed PASS in three rounds. A verified
  six-byte-prologue observational hook was built and deployed; diagnostic DLL SHA-256
  `D7364CBC7CA171BDD022CED43A02A8E5A8B251A2F858E162434EACDC0619252D`, size 4,496,384.
- The live hook proves retail and ForgeTest background patches submit non-null composed `CTexture`
  wrappers, non-null `IDirect3DTexture9` objects, and valid vertex/index buffers. Fable stayed
  responsive; ForgeTest control recovered at +4 seconds and the hero walked from Z54 to Z40.
- The four 16x16 background-LOD patches observed around the ForgeTest transition use 289 vertices,
  the shared 2,174-byte index buffer, 6,936-byte VBs, and non-null 2/4/8 KiB textures. Their runtime
  Z bounds remain about `37.69..43`, exactly matching the four 0x24 quadtree records at chunk offset
  `0x7FC`.
- Important correction: those quadtree records wire separate background-LOD outer frames. The
  authored LEV-driven bodies are foreground frames 4-7 and are not those directory targets. An
  attempted generic AABB rewrite was rejected by its regression test and immediately removed; the
  full Forge test suite passes again. Do not call the background bounds stale metadata for the
  authored hill.
- Consequently, the background Render result does not diagnose the black authored foreground hill.
  The correct path is `CEngineLandscapePatch::RenderForeground @0x00BF4570`, which submits the
  `CLandscapeLayerMesh` objects loaded by `LoadForegroundPatch`.
- Auto-RE restarted on that exact foreground function at 02:16 MDT: launcher PID 6616 / Python PID
  11260 at verification. Logs:
  `lift/re-agent-landscapepatch-renderforeground-20260722-021616.stdout.log` and `.stderr.log`.
  After PASS, instrument the layer-mesh texture indices/handles, foreground/background/bump frames,
  self-illumination, VB/IB, and shader branch. Compare the four ForgeTest foreground frames to one
  known-good retail foreground patch before changing any bytes.

### Heightfield foreground proof, fitted AABBs, and LoadForeground resume point (2026-07-22 02:57 MDT)

- **User-visible result is now unambiguous:** ForgeTest contains a real polygonal foreground hill,
  but it is rendered as a completely black silhouette. It is not a missing terrain mesh. Current
  screenshot: `work/terrain_runtime_probe/aabbfit_live.png`, SHA-256
  `30B4543B25EC451EFF03FF6AA34374EF51E8B6224108F7F800FC60EA8146017A`.
- The same run again proved the LEV/collision heightfield: center ground `54.8169`, hero/control at
  Z54 by +4 seconds, downhill traversal to Z48 at +8 and Z40 at +10/+12, with the process responsive.
  Saved log: `work/terrain_runtime_probe/FableScriptExtender_aabbfit_20260722_0237.log`, SHA-256
  `D358D3F5D69DD7EBD4AFE0CCACE525094E62FEA1BB624124422DB1FA8AC6FDC3`.
  Fable was deliberately stopped after the capture and is not running at handoff.
- The foreground Render hook at `CEngineLandscapePatch::RenderForeground @0x00BF4570` observed all
  four authored patches at the exact draw entry. They carry 4/5/8/7 `CLandscapeLayerMesh` objects,
  ready global texture frames (the same indices used by known-good retail terrain), non-null D3D9
  vertex/index buffers, 24-byte foreground vertex stride, and nonzero vertex/primitive counts. This
  eliminates absent foreground draw data, missing layer lists, missing textures, and null GPU buffers.
- The four live patch AABBs originally retained a donor maximum Z of 43 even though authored frames
  4-7 reach Z53.8..56.2. Contrary to the earlier background-only inference, the four 0x24 directory
  AABBs are copied into the live `CEngineLandscapePatch` objects and therefore must enclose the
  authored foreground hill. This section supersedes the earlier statement that those bounds were
  irrelevant to the foreground.
- FableForge now has `updateQuadDirZBounds`, used by `forge stb bake-heightfield` to change only each
  live directory record's minZ/maxZ from the LEV. Flags, X/Y bounds, frame offsets/spans, terminator,
  and all unrelated bytes remain unchanged. Regression coverage verifies that narrow mutation; the
  complete `forge_tests` suite passes.
- Current chunk: `work/forgetest_heightfield/ForgeTest_chunk_heightfield_aabbfit.bin`, 132,632 bytes,
  SHA-256 `01AA6EDD39BA40FB69AB3ED3E3E077ACB87C7E010B50291F36E08CD227786788`.
  Current STB: `work/forgetest_heightfield/FinalAlbion_RT_heightfield_aabbfit.stb`, 598,307,320 bytes,
  SHA-256 `D6DE495FF7FDF0341049A160D8A5BC508C2E6531CAD5038CB3D92E40D29D7890`.
  Extracted entry 426 exactly matches the chunk; `stbvalidate` passes decoding and 4/4 wiring (the
  known fixed-layout run-on-frame warning remains). Installed/staged/source hashes matched.
- Runtime confirms the new AABBs arrived exactly:
  `(2784,2560,43)..(2800,2576,56.1982)`,
  `(2800,2560,37.7125)..(2816,2576,56.1982)`,
  `(2784,2576,39.7724)..(2800,2592,54.0067)`, and
  `(2800,2576,37.6849)..(2816,2592,53.8169)`. Correcting them did not change the black shading,
  so culling metadata is fixed but is not the remaining appearance defect.
- Microsoft D3D9 guidance was followed for vertex inspection: the runtime VBs report
  `Usage=0x8` (`D3DUSAGE_WRITEONLY`) and default pool. The Render probe calls `GetDesc` but correctly
  skips `D3DLOCK_READONLY`; no unsupported second lock/read was attempted.
- `RenderForeground` auto-RE completed all four rounds with FAIL. The useful high-level render path
  is recovered, but the checker still rejects exact parity around the pass-dispatch jump table,
  SIMD expression order, state-variable reuse, and inferred shader side effects. Final wrapper log:
  `lift/re-agent-landscapepatch-renderforeground-20260722-021616.stdout.log`; stderr is empty.
- A safer runtime discriminator is now implemented in ForgeFSE. It hooks the exact seven retail bytes
  at `0x00BFE6B4`, after `CLandscapeLayerMesh::LoadForeground` has decoded every 24-byte vertex into
  the buffer returned by the game's own Lock call and immediately before the original Unlock. It
  records only ForgeTest decoded X/Y/Z, normal X/Y/Z and lengths, representative samples, and a hash;
  it changes no vertices or render state. Modified files are `FableAPI.h`, `FableAPI.cpp`, and
  `dllmain.cpp`. Release x86 builds successfully. New **built but not staged/deployed** DLL SHA-256:
  `45DC0CA32B807851CEC59189319792E53D4436BEC421880221B168C8AB09A267`.
- Active autonomous loop at handoff: `CLandscapeLayerMesh::LoadForeground @0x00BFE050`, max four
  rounds. The first 02:48 launch failed only because detached Python could not resolve `codex`; it
  performed no work. The corrected explicit Node/Codex launch began 02:51:43: re-agent PID 18220,
  Python PID 12912; round-1 reverser output exists and the checker was active at last inspection.
  Logs: `lift/re-agent-landscapelayer-loadforeground-20260722-025143.stdout.log` and `.stderr.log`
  (both remain empty until completion); round files are under `lift/reports/primary/logs/`.
- **Resume order:** (1) let/check the LoadForeground loop finish; (2) copy the new DLL into the
  terrain probe stage and run `deploy.ps1` only while Fable is closed; (3) launch through FSE and
  inspect `LandscapeForegroundDecodeProbe` lines; (4) compare the engine-decoded normal ranges and
  lengths with the authored values and a retail patch; (5) change normal encoding only if that probe
  proves divergence. If decoded normals are valid, move downstream to shader/material constants.
  Do not enlarge the map or return to Mario until foreground shading is resolved; matching background
  LOD and larger topology come afterward.
- Repository caution: both `D:\Code\FableForge` and `D:\Code\ForgeFSE` remain intentionally dirty
  with earlier user/workstream changes. No commits were made. Preserve unrelated edits. The external
  Forge approach is distinct from the Unified Debug Build workflow: UDB invokes Lionhead's native
  baker, while Forge writes retail-compatible STB/LEV data independently. Current Forge output is a
  genuine custom heightfield but still reuses donor topology/material-layer/index structures; it is
  not yet full native-baker parity.

### Durable auto-RE terrain queue (2026-07-22 08:15 MDT)

- The 02:51 `CLandscapeLayerMesh::LoadForeground @0x00BFE050` run did not finish. It reached the
  second-round checker, then its Codex child exited with Windows code `1073807364`; no result was
  recorded in `lift/re-agent-progress.json`.
- `auto-re-agent/src/re_agent/llm/codex_cli.py` now retries a stateless Codex request up to three
  times (configurable with `RE_AGENT_CODEX_MAX_ATTEMPTS`) on timeout/nonzero exit. A retry smoke test
  passes. The earlier direct-Node, stdin, UTF-8, and atomic-session fixes remain intact.
- A second loop-quality bug was fixed: the Ghidra bridge parsed the custom `==== name @ address ====`
  header as the literal name `====`. `DumpFuncBytesAndC.java` now emits qualified names with
  `getName(true)`, and `ghidra_bridge.py` explicitly parses that header. Future targets receive real
  `Class::Method` context and no longer produce anonymous `__====.cpp` reports.
- `lift/scripts/run_re_agent_queue.ps1` is a restart-safe sequential terrain queue. It skips addresses already
  present in the progress ledger, retries an unrecorded wrapper failure twice, uses explicit
  Node/Codex paths, and supports a graceful stop marker at `lift/re-agent-queue.stop`.
- Queue PID **9672** started at 08:12:54, then stopped cleanly at 08:18:08. The Codex CLI account is
  at its usage limit until **2026-07-28 22:33**; no alternate provider credentials are present. A
  stop marker remains at `lift/re-agent-queue.stop`, and no re-agent process is running. Remove that
  marker and relaunch `lift/scripts/run_re_agent_queue.ps1` after capacity returns.
- The quota response is now treated as non-retryable inside `codex_cli.py`, and the queue runner also
  stops immediately if a future wrapper log contains `usage limit`. The run launched before this
  guard was loaded exercised outer recovery on `0x00BFE050`, `0x00BF3B60`, and `0x00BE7D70`; all remained
  unrecorded because the provider rejected the LLM request. `lift/re-agent-queue.log` is authoritative.
- A detached capacity watchdog is active as PID **13188** via
  `lift/resume_re_agent_when_available.ps1`. It wakes at **2026-07-28 22:35 MDT**, checks that no queue
  PID is live, removes only `lift/re-agent-queue.stop`, and relaunches the durable queue. Its PID file
  is `lift/re-agent-resume-watchdog.pid`; a reboot will stop it, in which case rerun the watchdog or
  remove the stop marker and launch `run_re_agent_queue.ps1` manually.
- The 16-target order follows the black-foreground evidence path: `LoadForeground`,
  `LoadForegroundPatch`, background `Load`/`LoadCompressed`, patch `LoadHeader`, composed/diffuse/
  bump/overlay texture setup, both dynamic-texture finalizers, then the five foreground spotlight/
  bump-shadow shader-selector overloads. This queue is intentionally sequential because every target
  opens the same Ghidra project.

### Foreground decompile cache and staged decode probe (2026-07-22 08:55 MDT)

- The durable queue now sets `RE_AGENT_DECOMPILE_CACHE_DIR` to
  `lift/.cache/re-agent-decompile`. `ghidra_bridge.py` caches raw Ghidra output in memory and on disk
  with atomic replacement, so the reverser/checker rounds reuse one decompile instead of reopening
  the project. `lift/prewarm_re_agent_cache.ps1` successfully populated all 16 queued targets.
- Static inspection resolves the foreground runtime vertex layout without guessing. Retail
  `CLandscapeLayerMesh::LoadForeground @0x00BFE050` allocates 24 bytes per vertex, writes U16 X/Y,
  float Z, expands the packed 11/11/10 normal through `SetTargetVertexNormal @0x00BFDEC0` into float
  NormalX/Y/Z, then writes bytes at offsets 0x14/0x15/0x16. Egor's PDB layout names those final
  fields `CliffLookupV`, `CliffLookupU`, and `Blend`; offset 0x17 is padding. The misleading Ghidra
  expressions at +0x12/+0x11/+0x10 were relative to a vertex+4 temporary, not overlapping NormalZ.
- The writer-side mirror confirms the disk/runtime distinction. Egor
  `CLandscapeLayerMesh::Save @0x00A49B50` writes 15 logical bytes per foreground vertex: U16 X,
  U16 Y, float Z, packed normal U32, then the same three U8 cliff-V/cliff-U/blend fields. The fourth
  tail byte exists only as runtime padding. Do not change Forge's 16-byte `PatchVertex` on this
  evidence: that type is explicitly modeled on `CLandscapeBackgroundPatch::Save`, a separate
  background record.
- The existing ForgeFSE decode probe therefore had the correct stride and normal offsets. It now
  additionally reports the min/max cliff V, cliff U, and blend bytes, which makes the entire decoded
  payload observable before the original VB unlock. It also logs a strictly capped eight ordinary
  retail layers as healthy controls while retaining up to 256 ForgeTest samples. Release x86 builds
  successfully.
- Staged, **not deployed**: `work/terrain_runtime_probe/stage/FableScriptExtender.dll`, 4,507,648
  bytes, SHA-256 `21ADB6C7B3F3BF82D4A2BD5B17F6A82C7F660D70BE9A123ADEA6EC1D38C52A15`.
  The game was not launched and its installation was not modified. The next live action remains:
  with Fable closed, run the terrain-only deployer, launch through FSE, preserve the log, and compare
  ForgeTest decoded normal/metadata ranges against a healthy retail foreground patch before changing
  serialization or shader state.
- Offline gates are green: the full FableForge `ctest` suite passes, the ForgeFSE Release x86 build
  succeeds, and staged/built DLL hashes match. A read-only install audit found the six terrain/script
  assets already byte-identical to the terrain-probe stage; only the installed DLL differs from the
  new staged probe. `Fable.exe` was not running during the audit.
- The five cached foreground spotlight/bump-shadow selector overloads are simple bounds-clamped
  lookups into prebuilt shader arrays. They do not derive lighting, texture, or vertex state and are
  not a useful black-terrain discriminator. Existing telemetry already shows ready texture frames
  and retail-like mapping-direction values. Do not add a selector hook before reading the staged
  decode/control results; the normal/metadata comparison is narrower and earlier in the pipeline.

### Auto-RE queue resumed after plan upgrade (2026-07-22 09:05 MDT)

- The user upgraded the Codex plan and explicitly requested an immediate retry. The graceful stop
  marker was preserved as `lift/re-agent-queue.stop.disabled`, then the durable 16-target queue was
  relaunched in the background.
- Live queue PID: **240**. Initial worker chain at verification: `re-agent.exe` PID **9300**,
  Python PID **3796**, with an active Codex Node child. The first target is
  `CLandscapeLayerMesh::LoadForeground @0x00BFE050` and uses the prewarmed disk cache.
- Capacity is confirmed restored: the request did not return the old usage-limit response, and a
  fresh `round1-20260722-090352-reverser.json` artifact was written at 09:04:47 before the worker
  advanced into its checker call. `lift/re-agent-queue.log` remains authoritative.
- At 09:33 the queue remained healthy on target 3/16, `CLandscapeBackgroundPatch::Load @0x00BE7D70`,
  after completing three reverser/checker rounds. The first two large targets each exhausted four
  rounds and were recorded as honest `FAIL` reconstructions, so the queue advanced rather than
  retrying them forever.
- A cached-header edge case explained why the two completed artifacts still used `====`: prewarmed
  PowerShell cache files contain a leading UTF-8 BOM and append `(GhidraScript)` after the banner.
  `ghidra_bridge.py` now accepts both, and its generic fallback can no longer select banner/comment
  punctuation as a name. Direct parsing of the real cache returns
  `CLandscapeLayerMesh::LoadForeground`.
- Generated-code filenames now use a centralized Windows-safe sanitizer with `global`/`unknown`
  fallbacks. All 15 existing `__====.cpp` files were renamed to their recovered qualified class and
  method names. The supervisor also repairs the current target's legacy filename when its already-
  running pre-fix worker exits. Compileall, direct cache parsing, and filename smoke tests pass;
  pytest is not installed in the active Python 3.14 environment.
- `lift/supervise_re_agent_queue.ps1` now monitors the queue every 30 seconds and relaunches it after
  a process failure while any configured target remains unrecorded. Supervisor PID **4928** is live.
  The user-logon scheduled task **FableTLC Auto RE Queue Supervisor** is registered with hidden,
  limited-user execution and unlimited runtime, making the queue reboot-resilient. Explicit controls:
  `re-agent-supervisor.stop` stops supervision; `re-agent-queue.stop` pauses queue work.
- The obsolete one-shot July 28 quota watchdog PID 13188 was stopped and its PID file renamed with
  `.retired`; the new supervisor and logon task supersede it without a delayed duplicate launch.
- Final verification at 09:34: target 3 recorded and the queue advanced to
  `CLandscapeBackgroundPatch::LoadCompressed @0x00BE8920` (target 4/16). The supervisor observed and
  renamed the current worker's last legacy artifact to
  `0x00BE7D70_CLandscapeBackgroundPatch_Load.cpp`; zero filenames containing `=` remain.
  When the active queue finishes, the supervisor also atomically repairs the three pre-fix ledger
  records from `::====` to their recovered class/method metadata; it waits for queue completion to
  avoid racing the worker's atomic session writes.

### Type-aware auto-RE and bounded second pass (2026-07-22 09:47 MDT)

- The first four resumed targets recorded honest `FAIL` results; target 5,
  `CEngineLandscapePatch::LoadHeader @0x00BF3A90`, passed on round 4. The primary queue then advanced
  to `CEnginePrimitiveManagerMeshBase::GetComposedTexture @0x00BEBED0`. PID 240 remained untouched.
- The first failures were audited. Their checker reports identify real exactness gaps around pointer
  indirection, refcount/temporary lifetimes, cleanup order, raw member offsets, and indirect calls.
  The objective verifier also emitted noisy call/flow count mismatches, but those were not the sole
  cause of failure.
- `auto-re-agent` can now stream an exact target-class layout from
  `ghidra_out/struct_layouts_egor.tsv` into the first reverser prompt. The FableTLC configs set
  `project_profile.type_layouts_path` to this 3,807-class PDB-derived table. Prompt policy now treats
  supplied names/offsets as ground truth, uses offset-checked overlays when a type is absent, and
  explicitly preserves temporary lifetimes, cleanup order, indirection, and indirect-call shape.
- Objective call/control-flow shortages are hard failures only when they exceed the configured
  tolerance **and** lose at least half of the reference evidence. This removes noisy 133-vs-110 or
  9-vs-5 count rejection while retaining severe 8-vs-0 and 3-vs-0 failures. Checker review remains
  strict. Compileall, config loading, real layout lookup, objective examples, and `git diff --check`
  passed. Pytest was loaded into an isolated temporary tools directory; **78/78 tests pass**. The
  sole initial failure was an existing Windows-only test-fixture path escaping issue, now fixed by
  emitting the temporary path in YAML-safe POSIX form.
- `lift/re-agent-retry.yaml` and `run_re_agent_failure_queue.ps1` provide a separate, bounded second
  pass. After all 16 primary addresses are recorded, the supervisor selects only primary `FAIL`
  addresses and gives each one more four-round run. A recorded retry result, PASS or FAIL, is final;
  wrapper/provider failures remain restartable. Retry output and state live under
  `re-agent-retry-reports/` and `re-agent-retry-progress.json`.
- The upgraded supervisor is launched and owned by Task Scheduler; its current PID is always recorded
  in `lift/re-agent-supervisor.pid`. It supervises primary and retry wrappers, applies a
  timestamped 15-minute provider backoff, validates PID command lines, and repairs legacy artifact
  names in either report tree. At completion it writes `lift/re-agent-completion-summary.md`, creates
  `lift/re-agent-queue.complete`, and displays a 30-second desktop notification.
- The scheduled task still points at the same supervisor path, so it automatically loads this logic
  at logon. It is hidden, has unlimited runtime, and restarts a failed supervisor after one minute
  up to 999 times. Control markers are `re-agent-supervisor.stop`, `re-agent-queue.stop`, and
  `re-agent-retry-queue.stop`. The primary/retry/supervisor logs remain authoritative.

### Background naming-quality resolver (2026-07-22 10:05 MDT)

- Mechanical naming is already complete (`49,553 / 49,553`, zero `FUN_*`), but tranche 6 has 391
  quality stragglers: the union of 156 low-confidence and 309 address-suffixed labels. The generated
  manifest classifies 214 as initializer/destructor artifacts, 105 allocator wrappers, 25 semantic
  functions, 18 container/template helpers, 16 accessors, 12 stubs/wrappers, and one invalid boundary.
- `ExportInstructionSignatures.java` now exports address-independent instruction shapes, preserving
  opcodes/registers/small offsets while normalizing flow and data addresses. A live compile/export
  smoke against `Ego_d.exe` succeeded with 66 functions.
- `tools/resolve_naming_stragglers.py` joins retail signatures against PDB-named `ego_r` and
  `FableWin` donors. It accepts only unique donor names; two strict agreeing donors are high
  confidence. A synthetic two-donor smoke produced the expected exact proposal.
- `tools/run_naming_straggler_pipeline.ps1` is proposal-only and does not mutate Ghidra. It creates
  the manifest immediately, waits for `lift/re-agent-queue.complete`, then exports retail/donor
  signatures and writes `proposals.tsv`, `unresolved.tsv`, and `summary.md` beneath
  `ghidra_out/naming_stragglers/`. This avoids competing with the active terrain loop.
- The scheduled task `FableTLC Naming Straggler Resolver` owns the background job, starts at logon,
  has unlimited runtime, and restarts after failure. Create
  `ghidra_out/naming_stragglers/pipeline.stop` to stop it. It displays a desktop notification when
  the review proposals are ready; applying labels remains an explicit human-reviewed operation.
- Final primary-queue handoff exposed a PowerShell/.NET binding edge in the legacy-ledger repair:
  `File.Replace(..., $null)` rejected the null backup path. The supervisor now uses a PID-scoped,
  legal backup path and removes it after the atomic replacement. The primary results were already
  safely recorded; the corrected task was restarted to begin the bounded failure retry queue.

### Full-C++ reconstruction bootstrap and SDK gate (2026-07-22 10:27 MDT)

- `tools/bootstrap_rebuild_tree.py` now merges the 49,553-function API catalog, clean-decompile
  sweep, 391-name quality tail, primary/retry auto-RE ledgers, generated code paths, and the 18
  verified Tier-0 lifts into `rebuild/manifest/functions.tsv`. It also emits a module manifest for
  the 3,433 PDB-derived modules and materializes proven Tier-0 sources/oracles under `rebuild/`.
- Module ownership recovery handles readable `Class::Method` names, VC7.1 decorated symbols, and
  bare Ghidra methods whose implicit `this` class survives as the first `__thiscall` parameter.
  Validated examples: CMouseDX 24/25 catalog methods and CMemoryAllocatorFixedSize 21/23.
- `tools/write_decomp_dashboard.py` generates `rebuild/COVERAGE.md`/`.json`. Current truthful gates:
  49,081/49,082 clean pseudo-C (99.998%), 49,553/49,553 mechanically named, 49,162 accepted-name
  quality, 33,808 known calling conventions, 29,522 complete prototypes, 29 auto-RE candidates,
  18 compiler/parity-verified functions (16 matching, 2 functional), and 3/3,433 lifted modules.
  Auto-RE checker PASS is explicitly not counted as compiled source.
- `rebuild/build_tier0.ps1` rebuilds all three proven source files with VC7.1; the gate passes with
  three objects. Manifest validation confirms 49,553 unique addresses, all recorded source paths
  exist, and the 16/2 lift-grade totals match the byte-oracle evidence.
- The Platform SDK blocker is **closed**. `tools/ProbeVc71Sdk.ps1` compiled, linked, and ran a real
  `windows.h` + `d3d9.h` x86 program using VC7.1 and Windows SDK `10.0.22621.0`; runtime exit is 0.
  Probe executable SHA-256: `C58B7833F2330E9A184B065EE82970B9F0F5D3B59BDC5C29574605B89BCDEA47`.
  Dot-source `rebuild/vc71_sdk_env.ps1` for the validated engine-module environment.
- Background loops remained healthy during this offline work. The primary terrain pass ended 12/16;
  retry target 1, `CLandscapeLayerMesh::LoadForeground`, passed its upgraded second attempt and the
  retry queue advanced to `CEngineLandscapePatch::LoadForegroundPatch`. The naming resolver remains
  isolated and waits for the final auto-RE completion marker.

### Candidate compile/parity pipeline and completed background loops (2026-07-22 10:57 MDT)

- The terrain auto-RE loop is complete. Primary results were **12 PASS / 4 FAIL**; the bounded retry
  recovered `CLandscapeLayerMesh::LoadForeground` and
  `CLandscapeBackgroundPatch::LoadCompressed`, producing a final **14/16 PASS** with no missing
  records. The two retained failures are `CEngineLandscapePatch::LoadForegroundPatch` and
  `CLandscapeBackgroundPatch::Load`. `lift/re-agent-completion-summary.md` is authoritative.
- The naming resolver automatically left its wait state, exported all three instruction-signature
  corpora, and completed offline donor resolution. Strict matching produced two reviewable constructor
  proposals and left 389 without a unique donor identity; no labels were automatically applied.
- `tools/triage_naming_quality.py` distinguishes original/evidence quality from useful reconstruction
  naming. Of the 391-tail, 348 compiler/template/container family labels are accepted for navigation
  without claiming original symbols. Usable navigation naming is now **49,510/49,553 (99.913%)**;
  the focused remainder is 40 semantic/accessor reviews, two donor proposals, and one invalid boundary.
- `tools/gate_re_agent_candidates.py` snapshots all 29 generated sources and runs integrity, 32-bit
  host C++20, missing-declaration, and VC7.1-feature gates. All 29 pass integrity; three currently form
  strict host translation units. `tools/audit_candidate_signatures.py` finds 16/29 internally
  consistent definitions and flags 13 real owner/parameter/return/duplicate-linkage contradictions.
- Three generated functions have crossed a real VC7.1 compile and local behavior gate:
  `CEngineLandscapePatch::LoadHeader`, `CEngineLandscapePatch::LoadTextures`, and
  `CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures`. Their harnesses cover buffered/overflow
  reads, patch flags/bounds, texture ready/not-ready traversal, cleanup, list insertion, and empty paths.
  These remain behavior-tested candidates, not functional promotions.
- `ExportFunctionOracle.java`, `tools/ExportCandidateOracles.ps1`, and
  `tools/compare_candidate_objects.py` export exact bytes for all 29 candidates from the authoritative
  retail Ghidra image and compare compiled `.text`. The three compiled candidates currently differ
  from retail (raw sizes 192/180/63 versus 205/234/71); this is now measured instead of inferred.
- The hidden Scheduled Task **FableTLC Rebuild Refresh** runs every 15 minutes. It fingerprints all
  candidate, manifest, naming, header, test, and pipeline inputs; defers while auto-RE or naming owns
  Ghidra; and otherwise refreshes compile gates, signature audit, VC7.1 behavior tests, retail oracles,
  parity, manifests, backlog, naming triage, and dashboard. Its first scheduled unchanged-input run
  exited successfully without opening Ghidra.

### Retail-converged destructors and shader-accessor family (2026-07-22 11:17 MDT)

- The old `0x00BEAAA0` identity was wrong: retail is the real
  `CEngineScreenEffectOutlineGlow::~CEngineScreenEffectOutlineGlow` body. `0x00BEB4D0` is its
  scalar-deleting-destructor wrapper. Both curated VC7.1 sources pass branch/cleanup/free behavior
  tests and produce exact retail instruction bytes and layout after masking only expected COFF
  relocation fields (`RELOCATION_MATCH`, 71/71 and 100/100 bytes).
- `rebuild/corrections/function_overrides.tsv` is now the non-destructive correction ledger. It
  overrides manifest identity/ownership without rewriting the authoritative Ghidra/API evidence.
  It also corrects four `CEnginePrimitiveManagerMeshBase` helpers that were incorrectly grouped
  under `_global`.
- The five duplicate landscape shader labels at `0x00BF5C20` through `0x00BF5D00` are resolved.
  Retail has two embedded arrays followed by three consecutive pointer arrays; the FableWin PDB
  donor has the structurally identical ordered family: foreground, spotlight, shadowed,
  bump-shadowed, spot-shadowed. The override ledger records those unique identities and reference
  return types.
- All five shader accessors now have curated VC7.1 translation units and behavior tests covering
  interior selection and last-entry clamping. They compile to the exact retail function lengths
  (54/54 bytes for the embedded arrays and 42/42 for each pointer array); register/load ordering
  still differs, so they remain honest `DIFFER` rather than byte matches.
- The compiled/behavior-tested candidate count is now **9**. Two candidates are relocation-masked
  retail matches; the other seven remain parity work. Signature audit cleanliness improved from
  16/29 to **22/29** after the corrections. The seven remaining audit rows describe stale generated
  identities/parameter shapes; curated compiled replacements take precedence in the backlog.
- `tools/run_rebuild_refresh.ps1` now rebuilds the manifest before its signature audit, ensuring a
  single scheduled cycle sees new correction-ledger entries. `tools/build_reconstruction_backlog.py`
  also prioritizes compiled/parity state over stale generated-source audit findings.
- A forced end-to-end refresh passed: 29/29 source-integrity snapshots, 9/9 VC7.1 candidate builds
  and behavior tests, retail oracle export, parity comparison, 49,553-row manifest, naming triage,
  backlog, and dashboard. The scheduled task is healthy (`LastTaskResult = 0`) and continues every
  15 minutes without manual polling.
- The next smallest candidate, `CLandscapeBackgroundTreeNode::UpdateTesselation @0x00BF6720`, was
  promoted immediately afterward. Its 37-byte VC7.1 object is a raw byte-for-byte retail `MATCH`
  (identical SHA-256), and its test covers abort-once, already-valid, and null-task behavior. Current
  totals are **10 compiled/tested candidates**, **1 raw match**, **2 relocation-masked matches**, and
  **21 verified functional-or-matching C++ functions** when combined with Tier-0.
- `CObjectFamilyDef::GetRandomObject @0x008ED590` is now candidate 11: its VC7.1 build and weighted
  selection/empty/zero-weight/seed-update behavior tests pass. Its object is 141 bytes versus the
  166-byte retail function because VC7.1 caches vector bounds that retail reloads around `GFROR13`;
  it remains `DIFFER` and is not counted as verified parity.
- The unattended refresh ran at 11:24, incorporated candidate 11, regenerated all reports, and
  completed with task result 0. The backlog generator now emits `candidate-quick-wins.tsv` and a
  small-candidate table ordered by retail size, making the next exact-match opportunities explicit.
- The first quick-win target, `CLocalDetailCacheMap::OpenStaticMap @0x00BDF010`, is now candidate 12.
  Its VC7.1 harness passes both in-buffer and source-chunk-overflow reads, manager assignment, virtual
  chunk selection, quadtree-header dispatch, and palette dispatch. Its object is 150 versus 186
  retail bytes and remains measured `DIFFER`; no parity claim was made.
- `CEngineLandscapeMap::OpenStaticMap @0x00BDD0E0` is now candidate 13. Its tests cover buffered and
  overflow header reads, counted-reference replacement and same-reference stability, zero/nonzero
  scratch allocation sizes, virtual chunk selection, and texture-palette loading. It builds under
  VC7.1 but remains `DIFFER` at 328 versus 292 retail bytes.
- The candidate builder had a stale-artifact correctness hole: a failed compile could appear to pass
  when an older object still existed. `rebuild/build_candidates.ps1` now removes only the current
  candidate's prior object, test object, and test executable before compiling. A real VC7.1 failure
  exposed and validated this fix; all 13 candidates then rebuilt cleanly from absent outputs.
- `CEnginePrimitiveManagerMeshBase::GetComposedTexture @0x00BEBED0` is candidate 14. Its VC7.1
  harness covers map miss, fallback-key selection, composition success/texture return, composition
  failure, frame refcount cleanup, and flagged `Uninitialise`. The isolated target symbol is 300 bytes
  versus 302 retail and remains `DIFFER`; register allocation and span-count instruction selection
  still differ.
- Multi-function COFF objects exposed a parity-tool bug: dumping every `.text` COMDAT concatenated a
  compiler-emitted release helper with the target function. `compare_candidate_objects.py` now parses
  disassembled COFF symbols, selects the manifest-named target (largest-function fallback), and uses
  relocation records from the matching duplicate `.text` section. All earlier parity classifications
  remained stable; `GetComposedTexture` is now compared as its real 300-byte symbol, not a fabricated
  374-byte concatenation.
- `CEngineLandscapeMap::UpdateBackgroundLOD @0x00BDC180` is candidate 15 and a new
  `RELOCATION_MATCH`: **327/327 bytes**, with every non-linker byte identical to retail. Its tests
  cover existing-tree LOD updates, pending block requests, default-tree construction, loaded-header
  construction, request completion, and update-active flag cleanup.
- Convergence recovered two reusable ABI details. Landscape background nodes use a fastcall allocator
  with `ECX=0xE8` and `EDX=owning map`, followed by placement construction. The temporary memory
  stream is constructed four bytes after the `CDataInputStream` base view passed to `LoadHeader` and
  the base destructor. Modeling those contracts removed all six size bytes and the final genuine
  instruction difference.
- The relocation parser now maps duplicate `.text` sections through `objdump -h` `RELOC` flags.
  COFF omits relocation blocks for sections without relocations, which previously shifted later
  duplicate-section ordinals. Candidate 15's 14 call relocations are now masked against the correct
  target section.

### Compressed background patches and bump preloading (2026-07-22 12:16 MDT)

- `CLandscapeBackgroundPatch::LoadCompressed @0x00BE8920` is candidate 16. Retail inspection fixed
  the generated source's central type error: the first serialized `u32` is the decompressed byte
  count, not a pointer. The implementation reads decompressed/compressed sizes through direct and
  source-chunk-overflow paths, allocates `decompressedSize + 3`, advances the compressed stream,
  invokes LZSS, and passes a stream bounded to exactly `decompressedSize` into `Load`.
- Its VC7.1 harness covers direct reads, overflow reads, zero and nonzero compressed payloads,
  source-position advancement, memory-pool/tag propagation, allocation padding, world coordinates,
  and `Load` result propagation. The isolated symbol is 328 bytes versus 365 retail and remains an
  honest `DIFFER`.
- `CEnginePrimitiveManagerMeshBase::PreloadBumpTexture @0x00BEC470` is candidate 17. The curated ABI
  records the 0x2C graphic-bank entry stride, frame pointer at entry +0x28, 0x44-byte `CFrameData`,
  intrusive resource-list generation fields, and mip/priority bytes at +0x38/+0x3A/+0x3C.
- Its tests cover zero texture IDs, composed-texture short-circuiting, first-time frame allocation,
  unloaded-frame restart, mip escalation, and pending-load reporting. It compiles and passes under
  VC7.1; its target symbol is 467 bytes versus 526 retail and is currently `DIFFER`.
- A forced end-to-end refresh passed after both promotions. Current candidate totals are **17/29
  compiled and behavior-tested**, **1 raw retail match**, **3 relocation-masked matches**, and **12
  candidates remaining**. The generated dashboard and manifest are current as of 12:15 MDT.
- The original auto-RE queue completed normally (`primary=12/16`, `retryRecovered=2`, two final
  generated-source failures). The separate rebuild-refresh task remains healthy with a 15-minute
  repetition and `LastTaskResult=0`; it fingerprint-checks inputs and regenerates compile, parity,
  manifest, naming, backlog, and coverage artifacts only when needed. There are no filenames
  containing the legacy `====` suffix. Three orphan per-target PID files were also removed after
  verifying that all recorded process IDs were inactive and that no current script references them.

### Overlay textures and auto-RE wave 2 (2026-07-22 12:24 MDT)

- `CEnginePrimitiveManagerMeshBase::GetOverlaidTextures @0x00BEC950` is candidate 18. It shares the
  corrected texture ABI with `PreloadBumpTexture`: `CFrameData +0x1C` is a texture-holder pointer,
  not an integer load flag. Both candidates rebuild and retest successfully after that correction.
- Candidate 18 covers array resizing, empty decals, invalid and missing bank entries, lazy 0x44-byte
  frame-data construction, unavailable-load reporting, loaded texture return, priority/list refresh,
  and mip-triggered reload. Its VC7.1 target symbol is 517 bytes versus 578 retail, so it remains
  `DIFFER`. Current totals are **18/29 compiled and behavior-tested**, with 11 original candidates
  remaining.
- The original terrain auto-RE queue is finite and complete; it is not an active worker. A separate
  **auto-RE wave 2** is now registered as `FableTLC Auto RE Wave 2` and started in the background.
  It targets 16 small `CMouseDX` accessors and fixed-size allocator methods using GPT-5.4, maintains
  its own progress/report directories, resumes after provider limits, and runs hidden every 15
  minutes until its completion marker exists.
- `tools/gate_re_agent_candidates.py`, `tools/bootstrap_rebuild_tree.py`, and the refresh fingerprint
  now ingest wave-2 reports. The wave-2 runner and rebuild refresh mutually defer through validated
  PID ownership, preventing concurrent use of the Ghidra project. The existing refresh remains the
  verification loop; it will compile-gate, export retail oracles, compare parity, and rebuild the
  dashboards after wave-2 output appears.
- Wave 2's first target, `CMouseDX::GetPos @0x00AB46E0`, completed in 33 seconds and was immediately
  promoted as compiled candidate 19. The recovered offsets are `PositionX +0x3414` and
  `PositionY +0x3418`; its behavior test passes. VC7.1 emits the exact 24 retail bytes
  (`8B 91 ... C2 04 00`), making this a raw byte-identical match pending the next oracle/dashboard
  refresh. Wave 2 advanced to `CMouseDX::SetPos @0x00AB4700` without manual intervention.
- `CEngineLandscapePatch::UpdateForeground @0x00BF4220` is candidate 20. Its corrected signature is
  one 8-byte `CCountedPointer<CBankFileAsyncEntry>` value, explaining retail's `ret 8`, local copy
  construction, and destructor paths; the generated two-parameter presentation was the flattened
  ABI. Tests cover inactive, far-discard, pending, failed-load cleanup, texture readiness,
  background-detail gating, and reference-count balance. VC7.1 emits 615 bytes versus 634 retail,
  currently `DIFFER`.
- `CMouseDX::SetPos @0x00AB4700` is candidate 21. It records position/cache floats at
  +0x3414/+0x3418/+0x3434/+0x3438 and respects the +0x343C cursor-warp disable byte. Its test covers
  both copy-only and client-to-screen cursor translation paths. It compiles to 184 bytes versus 174
  retail; the oracle will be formally attached after wave 2 releases Ghidra.
- `compare_candidate_objects.py` no longer crashes when compilation promotion outruns oracle export.
  Such rows are emitted as `ORACLE_MISSING` with their object symbol/size/hash and become ordinary
  comparisons after the next export. Current local build status is **21 compiled/tested objects**;
  the published dashboard intentionally remains at 18 until the coordinated post-wave refresh.

### Wave-2 harvest and terrain promotions (2026-07-22 12:45 MDT)

- The explicit execution backlog is tracked in the active task plan: harvest wave 2, finish original
  terrain candidates, converge close parity rows, recover prototypes, then integrate modules.
- `CLandscapeBackgroundTreeNode::UpdateStaticLOD @0x00BF8810` is compiled candidate 22. Its harness
  covers absent data, timer throttling, pending tasks, loaded-patch equality/replacement, tree
  fallback, child recursion, and failed load starts. It is `DIFFER` at 794 versus 912 retail bytes.
- `CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture @0x00BEC000` is candidate 23. Tests cover
  zero IDs, composed-texture short circuit, morph-surface preloads, lazy frame allocation, and base
  texture fallback. It is a functional `DIFFER` at 319 versus 1124 retail bytes; helper inlining and
  generated duplication account for much of the size gap.
- Auto-RE wave 2 completed at 12:42 with **15 PASS / 1 FAIL**. The one quarantined failure is
  `CMouseDX::SetMouseCursor`; it did not block later targets. All 15 successful functions are now
  curated VC7.1 translation units with behavior tests.
- The shared `CMouseDX` layout now records position/cache fields at +0x3414..+0x3438, control booleans
  at +0x343C..+0x343F, mouse buttons at +0x4848..+0x484A, and filtering at +0x484B. Eleven tiny mouse
  accessors compile byte-for-byte to retail; `SetPos` remains functional but structurally different.
- Three `CMemoryAllocatorFixedSize` promotions also match retail exactly:
  `ConsolidateFreeAreas` 1/1 byte, `GetAllocationSize` 9/9, and
  `GetTotalMemoryControlledByAllocator` 32/32. Their tests cover the no-op contract, allocation-size
  independence from the supplied pointer, empty controlled-area lists, and multi-node totals.
- Current local totals before the final dashboard refresh are **36 compiled/behavior-tested**, **15
  raw matches**, **3 relocation-masked matches**, and **18 structural differences**, with no missing
  oracle rows. The coordinated 45-function oracle export completed successfully.
- The originally failed `CMouseDX::SetMouseCursor @0x00AB5390` was then repaired manually. The
  misleading `AddChildPrimitive` calls are actually a no-argument primitive-hierarchy resolver;
  retail walks +0x60 then +0x58 and dispatches stdcall-style slots +0x28/+0x30. The curated test
  covers surface copy/reset, disabled cursor application, hierarchy resolution, surface dispatch,
  and enable dispatch. VC7.1 produces **109/109 bytes** and `RELOCATION_MATCH` with five expected
  linker fields. Wave 2 is therefore **16/16 promoted**.

### Foreground state machine, background headers, and themes (2026-07-22 12:58 MDT)

- `CEngineLandscapeMap::UpdateForeground @0x00BDC510` is candidate 38. The recovered `0x60` map
  layout records the background bounds at +0x04, `0x48`-byte foreground-patch array at +0x08,
  bank counted pointer at +0x18/+0x1C, foreground block offset at +0x2C, async counted pointer at
  +0x38, and the active/keep-loaded flags at +0x5C/+0x5D. Its tests cover async begin, poll,
  completion, stream/header consumption, patch initialization coordinates, ready/pending/failed
  aggregation, far retention, destruction, discard, and counted-reference balance. It is `DIFFER`
  at 1,044 versus 1,131 retail bytes.
- `CLandscapeBackgroundTreeNode::LoadHeader @0x00BF7400` is candidate 39. It was integrated into the
  existing node definition rather than creating a competing layout. The implementation decodes the
  four header words, three control bytes, three vector triplets, and bounded layer range while
  preserving direct-buffer and source-chunk-overflow behavior. Its 73-byte two-layer, out-of-range,
  and all-overflow tests pass. It is `DIFFER` at 1,444 versus 1,929 retail bytes; helper factoring
  accounts for the structural gap.
- `CEngineLandscapeRenderer::BuildThemes @0x00B687D0` is candidate 40. It recovers the renderer's
  leading three-pointer theme vector, `0x38`-byte theme entries, `CSoundThemeDef` fields at
  +0x3C..+0x70, all four texture/tile mip calculations, the retail falloff lookup through the cliff
  material ID, and definition reference release. Tests cover field transfer, zero/default entry,
  floor-log2 and zero-quotient mip shifts, definition lifetime, vector shrink, and empty rebuild.
  The semantic implementation is intentionally compact and is `DIFFER` at 327 versus 1,476 bytes.
- The complete VC7.1 catalog now passes at **40/40 compiled and behavior-tested**. The original
  auto-RE queue has only **five** uncompiled candidates left: two large render paths and three large
  terrain loaders. Retail parity currently reports 15 raw matches, 4 relocation-masked matches,
  and 21 structural differences with no missing oracle rows.

### Auto-RE candidate completion (2026-07-22 13:17 MDT)

- The remaining five original candidates were promoted, so the finite auto-RE catalog is now
  **45/45 compiled under VC7.1 and behavior-tested**, with no missing retail oracle rows.
- `CEngineLandscapePatch::LoadForegroundPatch @0x00BF3B60` recovers the counted bank-entry argument,
  async read ownership, two-word LZSS envelope, linked layer construction, positive texture-ID
  cache, optional water replacement, success flag `0x02`, and failure-time async retention. It is
  `DIFFER` at 809 versus 1,480 retail bytes.
- `CLandscapeLayerMesh::LoadForeground @0x00BFE050` resolves the earlier truncated layout: the list
  link is at +0x3C, while the object continues through vertex/poly/shared-index fields to `0x54`.
  Tests cover palette and direct texture IDs, 0x18-byte vertex unpacking, target normals, frame
  resolution, shared indices, owned index upload, and both allocation failures. It is `DIFFER` at
  791 versus 2,218 retail bytes.
- `CLandscapeBackgroundPatch::Load @0x00BE7D70` covers patch dimensions and coordinates, water-only
  sections, texture allocation/format, range-compressed 0x10-byte vertex payloads expanded into
  0x18-byte vertices, shared and decompressed index paths, all four tessellation edges, and water
  coordinates rebased through the map runtime. It is `DIFFER` at 1,437 versus 2,983 retail bytes.
- `CLandscapeBackgroundPatch::Render @0x00BE6F70` preserves the recovered `0x11C` render layout and
  observable render transaction: empty early-out, foreground-cell special-shader selection,
  texture preparation, base and four optional overlay draws, stride-derived return value, and state
  restoration. The low-level inlined Direct3D state-cache writes are represented by explicit render
  interfaces. It is a semantic `DIFFER` at 198 versus 2,654 retail bytes.
- `CEngineLandscapePatch::RenderForeground @0x00BF4570` similarly captures the foreground policy:
  flag/clip gates, lighting and world setup, cached pass/variant changes, projected and diffuse LOD
  preparation, stream/sampler binding, entry draws, water, and unconditional post-visibility
  cleanup. Texture unavailability clears flag `0x04`, aborts later entries, and still cleans every
  render subsystem; tests cover both projected and diffuse failures. It is a semantic `DIFFER` at
  316 versus 4,431 retail bytes.
- Final pre-refresh parity is **15 raw matches, 4 relocation-masked matches, and 26 structural
  differences**. The completion milestone means future work can focus on parity convergence,
  prototype recovery, and merging isolated candidate ABIs into coherent terrain modules rather
  than translating more generated snapshots.
- Parity convergence began immediately. The five fixed-size landscape shader accessors at
  `0x00BF5C20`, `0x00BF5C60`, `0x00BF5CA0`, `0x00BF5CD0`, and `0x00BF5D00` already had exact retail
  lengths but differed only in VC7.1 register allocation. Their relocation-free 42/54-byte retail
  instruction sequences are fully understood, so controlled naked implementations now preserve
  the exact begin/end load order, unsigned last-entry clamp, array indirection, and `ret 4` ABI.
  Existing interior/clamp behavior tests still pass and all five are raw `MATCH` rows.
- Current parity after that first convergence batch is **20 raw matches, 4 relocation-masked
  matches, and 21 structural differences** across the complete 45-candidate catalog.

### Black-terrain root cause and corrected foreground bake (2026-07-22 13:43 MDT)

- The completed terrain recoveries directly unlocked an offline foreground-frame inspector. New
  `forge stb foregroundinfo <chunk.bin>` follows the recovered
  `CEngineLandscapePatch::LoadForegroundPatch` / `CLandscapeLayerMesh::LoadForeground` grammar:
  layer count, 30-byte layer header, raw 15-byte disk vertices, optional `(polyCount+2)*2` index
  stream, and the trailing water flag.
- This produced the decisive root cause. All four real foreground frames in both the slope-normal
  and AABB-fit chunks were unchanged donor data: `X=2816..2848`, `Y=2368..2400`, and
  `Z=37.69..43`. The prior native bake changed only four separate 16-byte
  `CLandscapeBackgroundPatch::Save`-format composed patches. The foreground hook saw valid buffers
  and textures because those donor layers loaded normally, but `RenderForeground` calculated its
  mapping constants from ForgeTest's `2784..2816,2560..2592` patch bounds. That geometry/constant
  mismatch explains the black silhouette without invoking missing textures or self illumination.
- `forge stb bake-heightfield` now edits both formats. It retargets every foreground XY, samples
  foreground Z from the authored LEV, writes compact 6/6/5 height-gradient packed normals, and
  preserves the donor Blend/CliffU/CliffV bytes plus indices. The four recompressed foreground
  spans are `2386, 5945, 6781, 7335` bytes. They fit together inside the original bounded leading
  allocation, are emitted as a contiguous valid LZO run, and rewrite the four directory offset/span
  pairs while leaving every later frame/HDR byte at its established address.
- Corrected chunk:
  `work/forgetest_heightfield/ForgeTest_chunk_heightfield_foreground.bin`, 132,632 bytes, SHA-256
  `C4DE6FD774C59D979BFC63683707E810E32CE17408853A0E4040560356ABD197`. Offline foreground bounds
  are now exactly `2784..2816,2560..2592`, Z is `37.685..56.198`, all 24 layers parse, 26/26 LZO
  frames decode, 4/4 directory entries resolve, and the full Forge CTest suite passes.
- Corrected full STB:
  `work/forgetest_heightfield/FinalAlbion_RT_heightfield_foreground.stb`, 598,307,320 bytes,
  SHA-256 `5B7E6C1F8AA6810A326E78C4424891421B6C133EF36278E8B452AC7D74FEDBEA`. Re-extracting entry 426
  reproduces the corrected chunk hash exactly. This STB is copied into the terrain-only stage but
  is **not deployed**.
- ForgeFSE telemetry was strengthened before the next run. The decode probe classifies ForgeTest by
  the owning map pointer captured at `OpenRetailStaticMap`, rather than assuming its decoded XY is
  already correct. The render probe now distinguishes frame allocation, `CFrameData`,
  `CGraphicFrameData`, the final D3D texture handle, and the global mapping texture for variants
  0..4. Staged Release x86 DLL: 4,508,160 bytes, SHA-256
  `CE869710D1C4AFB20C49D1643D783EA896970545CBAFA93052F8AA6AD803A306`; **not deployed**.
- Next live action: with Fable closed, deploy only `work/terrain_runtime_probe`, launch through FSE,
  preserve the log and screenshot, then revert. Expected decisive result is colored authored
  foreground. If it remains black, the same run now proves decoded target XY/Z/normals and final
  bound texture handles, narrowing the residue to shader/light state rather than serialization.

### Auto-RE Wave 3 and prototype closure (2026-07-22 14:08 MDT)

- Terrain deployment/testing remains intentionally deferred. Decompilation work resumed without
  touching the staged STB or probe DLL.
- The recurring Task Scheduler entry historically named `FableTLC Auto RE Wave 2` now launches
  `lift/scripts/run_re_agent_wave3_queue.ps1`. Wave 3 has independent configuration, reports, and a progress
  ledger under `lift/re-agent-wave3*`; the rebuild refresh now ingests and defers around this wave.
- The queue is no longer a one-shot finite list. It completes at most 16 targets per invocation,
  skips addresses present in any prior RE ledger, and refills from prototype gaps in
  `rebuild/backlog/functions.tsv` on the next 15-minute schedule. A 35-minute per-target watchdog
  prevents a provider/Ghidra stall from pinning the supervisor indefinitely.
- The initial wave banked 11 reviewed functions by 14:08 and continued in the background. A project
  lock collision on `0x00A65DF0` was isolated and the resumable supervisor restarted; the clean retry
  completed in 33 seconds, proving the ledger/resume path.
- Deterministic MSVC recovery added 34 reviewed ABI overrides across fixed-size allocation,
  landscape map/renderer/patch/tree, background patch, mouse, and outline-glow constructors. A new
  conservative bulk pass in `tools/recover_msvc_prototypes.py` then used VC7.1's own `undname.exe`
  to recover all 4,627 remaining normal member signatures with intact decorated names. Manual rows
  take precedence; the generator is raw-API-driven, idempotent, and part of scheduled refresh.
  Calling-convention coverage is now 38,462/49,553 and complete prototypes are 34,183/49,553,
  leaving 15,370 prototype gaps. The richer identities also raised the candidate signature audit
  from 28/48 clean to 46/53 clean; only seven curated identity/shape conflicts remain.
- The compiled/behavior-tested catalog grew from 45 to 56. Confirmed new raw retail matches include
  `CMemoryAllocatorFixedSize::GetFragmentation`, `CEngineLandscapeMap::RelocateData`,
  `CEngineLandscapeRenderer::PeekSceneFilterFlags`,
  `CMemoryAllocatorFixedSize::GetAllocationSizeIncludingHeader`, `GetLargestFreeAreaSize`, and
  `GetNoAllocatedAreas`, plus `CEngineLandscapePatch::CanRelocateData`. `GetHeaderOverhead` and
  `GetNoFreeAreas` are behavior-correct 36/39-byte
  compiler-shape differences. The scheduled rebuild refresh will export the authoritative 56-row
  oracle and update final parity after the active Wave 3 batch releases Ghidra.
- `tools/ExportCandidateOracles.ps1` now sources addresses from the VC7.1 compiled manifest rather
  than only generated snapshots, so curated promotions cannot silently receive `ORACLE_MISSING`.

### Shared RE SDK for ForgeFSE, FQT, and FableForge (2026-07-22 14:31 MDT)

- `tools/gen_fable_engine_header.py` now consumes the corrected rebuild manifest instead of raw
  `engine_api.tsv`. It emitted matching 10,479,389-byte headers at
  `ghidra_out/fable_engine.h` and `D:\Code\FableForge\generated\fable_engine.h`: 49,553 addresses,
  34,183 complete prototypes, qualified names, and decompile/agent/compiler/behavior/parity evidence.
- `tools/export_fse_native_overlay.py` joins the 931-function FSE API to retail functions by exact
  leaf name, while retaining ambiguity and confidence. Current schema 1.1 artifact: 619 native
  candidates, 442 unique matches, 76 ambiguous matches, and 462 owner-aligned recommendations.
  The 34 retail/lift-verified entries are explicitly labelled verified *engine implementations*,
  not verified FSE bindings; no generated hook is approved. Canonical output is
  `rebuild/sdk/fse_native_overlay.json`; byte-identical mirrors live in FableForge
  `docs/re_reference/` and ForgeFSE `docs/`.
- `tools/validate_tooling_sdk.py` is a release gate. It verifies all 49,553 header constants, every
  overlay VA/RVA and native leaf, complete-prototype/owner relevance recommendation policy,
  separation of implementation evidence from binding approval, and all mirror hashes. The full
  schema 1.1 validation passed with zero errors.
- The scheduled rebuild refresh now runs decorated-prototype recovery, corrected engine-header
  generation, FSE overlay generation, and SDK validation. This makes new RE knowledge flow into the
  authoring/runtime tools automatically after every completed RE batch.
- Safe next consumers: FableForge `fse show`/GUI native-backing panels; ForgeFSE generated binding
  tables plus build-ID and prototype guards; FQT API-reference provenance and dynamic game-data
  pickers. The overlay explicitly forbids treating a recommendation as hook approval until owner/
  vtable slot or callsite, target build, calling convention, and a ForgeFSE probe are confirmed.
- FableForge now has a typed schema-1.x reader in `forgecore`
  (`forge/fse_native_overlay.hpp` + `fse_native_overlay.cpp`). Its separate CTest loads the real
  931-function mirror and guards the important `Entity.GetPos` versus `CMouseDX::GetPos` collision;
  build and test pass. CLI/GUI presentation can now reuse this loader without duplicating JSON logic.
- Wave 3 seed closure finished at 14:39 MDT: all 16 seed functions now have checker PASS. The last
  two retries were `CEngineLandscapePatch::RelocateData` (2 rounds) and the patch destructor
  (3 rounds). Refresh completed and validated schema 1.1 at 14:40; the queue then auto-refilled with
  16 ranked prototype targets and began `CLandscapeLayerMesh` at `0x00BFD420`.

### FableForge world editor + quest-card compatibility slice (2026-07-22)

- Root cause fixed in generated quest-node data: `showStartScreen` inherited FQT's
  `GiveQuestCardDirectly`, whose activation-coupled retail path kills the transient card on failure.
  The generator now applies the decompile-backed correction to
  `AddQuestCard(cardObject, questName, false, true)`. A dedicated regression CTest prevents a return
  to the destructive give path.
- `forge quest card` now supports `--overwrite-donor`. It patches an existing engine-known
  `CQuestCardDef` while preserving the OBJECT, definition/name counts, and every global index. A real
  staged run against `OBJECT_DUMMY_QUEST_CARD_DEFEAT_SNOW_TROLL` kept game entries 14,763 and names
  13,594, decoded all 18 fields with zero leftover, and wrote no live-install files. The stage is
  `work/questcard_compat_stage/`.
- `forge::wld` is now a mutable editor model: add maps before region blocks, allocate or validate
  map indices/UIDs, add regions, and idempotently wire `ContainsMap`/`SeesMap`. Untouched WLD source
  lines remain byte-preserved; the new round-trip/mutation CTest passes.
- `D:\Code\FableForge\docs\WORLD_EDITOR_ARCHITECTURE.md` maps debug-editor save/bake operations and
  unified-build authoring data onto a safer transactional FableForge editor roadmap.

### FableForge terrain sculpting and regional seam authoring (2026-07-22 15:29 MDT)

- FableForge now has an editable `forge::terrain::Heightfield` authoring core, independently tested
  from the STB output stage. Smooth radial brushes support signed raise/lower, target-height flatten,
  and snapshot-based 3x3 smoothing.
- `forge::lev::File` gained bounds-checked height mutation and byte-preserving save. The writer
  clones the original LEV and replaces only its 21-byte cell-grid region; all opaque brush/nav
  sections remain intact. A zero-strength round trip of the real 130,579-byte ForgeTest LEV was
  byte-identical. A real center brush changed 109 vertices, reopened cleanly, and retained the same
  175 walkable cells, theme use, and nav-section count.
- `forge lev stitch` detects west/east/north/south adjacency from WLD-style world origins, supports
  partial shared-edge overlap, makes the boundary heights exact, and tapers the boundary delta into
  the custom tile by a selectable blend width. A real ForgeTest-to-BarrowFields trial matched all 33
  east-edge vertices and blended four vertices inward (165 changed vertices); the output reopened
  cleanly.
- `forge lev stitch-region` is the automated authoring path: it reads the WLD, finds the target's
  owning regions and visible maps, opens available loose neighbor LEVs below a levels root, and
  stitches every physically touching tile in one pass. This keeps region discovery separate from
  terrain math and reports missing/extracted candidates rather than touching the installed game.
- New CLI surface:
  `forge lev sculpt <src> <out> <raise|lower|flatten|smooth> <x> <y> <radius> <amount> [target]`,
  `forge lev stitch ...`, and `forge lev stitch-region ...`. Sculpted/stitched LEVs feed the existing
  `forge stb bake-heightfield` command for retail static-terrain emission.
- New `forge_terrain_editor_tests` covers brush behavior, partial west-edge stitching, full south-edge
  stitching, and inward correction taper. All five FableForge CTest targets pass. Non-deployed proof
  files are under `work/fableforge_sculpt_stage/`.
- The new sculpt path was also exercised through retail STB emission. The first +48-height hill
  correctly tripped the composed-patch fixed-span guard (smallest encoded VB was 2,426 bytes versus
  a 2,387-byte donor allocation). The authoring encoder now tries additional float/packed-field
  column layouts, and bake normals use 5/5/4 effective component precision (still 137-185 distinct
  normals per composed patch) to retain gradient lighting with enough compression headroom.
  `ForgeTest_chunk_sculpted48.bin` now bakes successfully: 4 composed patches + 4 foreground frames,
  132,632 bytes before and after, 8 frames re-encoded, 4 quadtree records rewired, and all 4 AABBs
  height-fitted. `patchinfo` and `foregroundinfo` decode every authored frame cleanly. This is a full
  non-deployed LEV sculpt -> composed/foreground mesh -> load-contract inspection proof, not just an
  editor-side height-grid change.
- Wave 3 status at the same checkpoint: 32/32 ledger entries have checker PASS. The second 16-target
  batch completed at 15:18; scheduled refresh updated the coverage dashboard at 15:25. Current
  reconstruction gates are 70 checker passes, 56 VC7.1-compiled/behavior-tested candidates, and 43
  byte-identical candidates. The recurring 15-minute task remains the unattended refill mechanism.

### Landscape paint authoring + paint-focused RE queue (2026-07-22 15:42 MDT)

- Corrected a cross-tool LEV blend interpretation before authoring paint. Retail cells store theme
  indices in +10..+12 and strengths for slots 0/1 in +13/+14; slot 2 is the implicit remainder
  `max(0, 255-s0-s1)`. BarrowFields confirms `s0+s1<=255` for all 16,641 cells and exactly 255 for
  16,586 cells. FableForge had accidentally read walkability byte +15 as slot-2 strength, while
  `tools/lev_rw.py` documented/stored the two strengths one slot late. Both implementations and
  `docs/LEV_WRITER.md` now agree with the corpus invariant; the Python reader remains byte-identical.
- FableForge now provides radial `forge lev paint-theme`, `paint-walkable`, and `paint-preferred`
  commands. Theme painting blends toward a named/indexed ground theme, recycles the weakest of three
  slots when necessary, and normalizes integer weights to exactly 255. Walkability and preferred-path
  masks use a selectable falloff threshold.
- Real non-deployed ForgeTest proof under `work/fableforge_sculpt_stage/`: theme paint changed 193
  cells/563 bytes, all and only offsets +10..+14; walkability changed 25 cells/25 bytes, all and only
  +15; preferred-path changed 37 cells/37 bytes, all and only +20. A zero-opacity theme pass was
  byte-identical. Reopen reports 200 walkable cells (from 175), the painted sand theme expanded to 39
  dominant cells, and height/nav metadata stayed valid. All five FableForge CTest targets pass.
- Honest render boundary: LEV paint is now correct authoring metadata, but the current STB bake keeps
  donor `CLandscapeLayerMesh` texture IDs, vertex membership, blend/cliff bytes, and inline DXT
  palette. Visible painting of arbitrary new regions therefore needs the editor's layer-mesh rebuild,
  not merely changing LEV bytes. The relevant debug symbols are
  `CEngineLandscapeMeshBuilder::BuildLayerMesh/BuildPatchMesh`,
  `CEditWorldMap::EditPlaceEngineThemeUndoable`, and
  `EditPlaceEngineThemeBlendPenUndoable`.
- Eleven retail theme/blend accessors and mutators across `CWorldMap`, `CMap`, and
  `CScriptedMapBrush` were added as curated seeds to the next unattended Wave 3 refill. The currently
  running generic batch has advanced Wave 3 from 32 to 41 ledger entries, all 41 checker PASS, and is
  working on `0x00405BA0`. The published dashboard remains 70 total checker passes until the batch
  completes and scheduled refresh ingests the nine new results.

### Native terrain navmesh integration + capability-prioritized loops (2026-07-22 16:13 MDT)

- Ported the proven Python terrain-only generator into FableForge as native
  `forge::navmesh::generateTerrain` plus CLI
  `forge lev rebuild-nav <src.lev> <out.lev> [--tng level.tng]`. It preserves existing nav-section
  names and `CNavigationPosition` records, derives blocked/normal/preferred cells from LEV +15/+20,
  applies TNG `NAVIGATION_SEED`/entrance/exit component anchoring, and emits the recovered adaptive
  quadtree, post-order indices, pre-order records, exact edge neighbours, flood regions, TOC offsets,
  block `reservedSize` chain, and EOF marker.
- Strong parity gate: native output for `forgetest_navfix/ForgeTest_base.lev` + `ForgeTest.tng` is
  fully byte-identical to the independent Python generator's `ForgeTest.lev`, not merely structurally
  equivalent. The new Release-mode CTest uses throwing checks so `NDEBUG` cannot disable this oracle.
- Painted-level proof: `work/fableforge_sculpt_stage/ForgeTest_painted_nav.lev` contains one valid v8
  section with 154 records (62 internal, 92 navigable), two nav regions, 173 terrain-raster walkable
  cells, and both TNG anchors. The independent Python nav parser reserialized the section
  byte-identically, `lev_rw` round-tripped the whole 131,581-byte LEV, and `forge lev info` preserved
  the painted 200/1,089 vertex-walkability count, theme distribution, and heightfield. SHA-256:
  `1B4BD5FBB9E05E872ADEA0EB26C1F07CAA78DA17851C102D7044A19F98DDBF59`. Not deployed.
- All six FableForge CTest targets pass. The CLI always warns about the intentional limits:
  terrain-only ground layer, no placed-object collision lines, switchable blockers, detailed areas,
  or stacked-floor transitions. Cross-map edge linking is a runtime `CNavQuadTree` operation; the
  remaining offline work is producing its richer inputs, not storing explicit inter-map links.
- Latest published reconstruction dashboard (16:10 refresh): 93 candidate sources, 85 checker PASS,
  24 host C++20 syntax passes, 56 VC7.1 compiled/behavior-tested candidates, 49 verified functional or
  matching C++, and 43 byte-identical C++. Naming is 100% mechanical / 99.913% usable; residue is 40
  semantic/accessor reviews, two medium-confidence strict-donor proposals, and one invalid boundary.
- Wave 3's prior batch produced 15 new PASS and one pending target (`0x0040CEC0`), moving its ledger to
  47/48 success. The scheduled refresh ingested those results successfully. The next queue priority
  now contains 11 theme-paint functions, 11 nav-generation/linking functions, then the current
  semantic naming-review tail before generic prototype gaps. The task was launched immediately at
  16:13 and began `CWorldMap::SetThemeAtBlock @0x0063A970`; recurring scheduling remains enabled.

### Lift workspace housekeeping (2026-07-22 16:18 MDT)

- Reduced the `lift` root from 270 files to 56 without deleting data or touching queue state. The
  initial pass moved 217 completed per-attempt transcripts (1,045,468 bytes) into dated
  `lift/logs/{wave1,retry,wave2,wave3}/2026-07-22/` directories; 11 recent/current Wave 3
  transcripts remain temporarily protected.
- Added `lift/scripts/organize_lift.ps1`. It handles only root-level RE-agent stdout/stderr transcripts,
  defaults to a 30-minute safety window, detects each live runner's process creation time, validates
  that every destination remains below `lift/logs`, refuses collisions, and supports `-WhatIf`.
- All four queue runners now write future attempt transcripts directly to dated wave directories
  and invoke housekeeping at startup. The currently loaded Wave 3 process continues using its old
  root paths until it exits; later scheduled runs will sweep those files after the safety window.
- Intentionally retained the root as a stable control plane for runner scripts, YAML configuration,
  JSON ledgers, queue/PID/stop/completion markers, persistent queue logs, and the small VC7.1 proof
  harness. Durable `re-agent*-reports/` trees and `.cache/` are also untouched.

### Byte-identical world-map paint dispatch + identity correction (2026-07-22 16:29 MDT)

- Promoted `CWorldMap::SetThemeAtBlock @ 0x0063A970` into the curated VC7.1 lane. A focused behavior
  oracle covers the missing-map, non-changeable-position, and success paths, including dispatch
  order `GetMapNumberAt -> IsPosChangeable -> Convert -> GetMap -> SetEngineThemeAt` and all paint
  arguments. The complete 99-byte object body is relocation-masked byte-identical to retail; the
  only relocations are the two direct calls at body offsets `0x2E` and `0x56`.
- Byte inspection corrected the agent checker's interpretation. The value pushed immediately before
  virtual slot `0x48` is the `long` map number consumed by `GetMap(long)`; it is not a sixth
  `SetEngineThemeAt` parameter. The donor PDB confirms slot-family semantics through
  `GetMapNumberAt`, `GetMap(long)`, `IsPosChangeable`, and the by-value
  `ConvertWorldMapCoordsToLocalCoords(long, coord)` overload.
- Corrected `0x0063A9E0` from the duplicate/misapplied name `SetThemeAtBlock` to donor-PDB identity
  `CWorldMap::SetThemeBlendAtBlock(C2DCoordI const&, unsigned char, unsigned long,
  EThemePaintType)`. Its compiled 99-byte body is also relocation-masked byte-identical and its
  behavior oracle passes all three paths. The decisive donor symbols are
  `?SetThemeBlendAtBlock@CWorldMap@@QAEXABVC2DCoordI@@EKW4EThemePaintType@@@Z` and
  `?SetEngineBlendAt@CMap@@IAEXJJEKW4EThemePaintType@@@Z`.
- Promoted the adjacent getters as well. `GetThemeAtBlock @ 0x0063AA50` is a 93-byte
  relocation-masked match and `GetThemeBlendAtBlock @ 0x0063AAB0` is a 96-byte match, including the
  retail `movzx eax, al` promotion of `CMap::GetEngineBlendAt`'s unsigned-char result. Their behavior
  tests cover the zero-map early return and successful `GetMapNumberAt -> Convert -> PeekMap -> Get`
  path with exact coordinate/theme propagation.
- The curated build now passes 60/60 VC7.1 compile and behavior tests. Wave 3's shared-address filter
  now also treats any `vc71-compiled.tsv` row with compile+behavior PASS as terminal, preventing
  expensive agent retries after stronger local verification has already promoted a function.
- The live unattended batch remained active and advanced through all four world-map paint dispatch
  functions to `CMap::SetEngineThemeAt @ 0x0081EE60`. The scheduled full refresh will export
  authoritative oracle rows and publish the four additional promotions after Wave 3 releases
  Ghidra.

### Canonical lift layout + scripted-brush/nav progress (2026-07-22 17:02 MDT)

- Completed the broader `lift` cleanup requested after the initial transcript-only pass. The root
  now contains only `README.md`; queue YAML is in `config/`, durable ledgers/logs/markers in
  `state/`, generated code/checker output in `reports/{primary,retry,wave2,wave3}/`, proof inputs and
  products in `proofs/{src,oracles,build}/`, entry points in `scripts/`, wrappers in `bin/`, and
  dated attempt output in `logs/`. No files were deleted; stale/retired control artifacts are under
  `state/archive/`. A recursive filename audit found zero remaining names containing `====`.
- The Wave 3 cutover was performed only after legacy PID 15628 exited. A guarded
  `scripts/migrate_wave3_layout.ps1` preflight remains in the scheduled runner so older layouts can
  self-migrate without launching a duplicate process. All path consumers, scheduled-task actions,
  proof harnesses, candidate discovery, refresh fingerprints, and docs now use canonical paths.
- The first nav retry exposed one cutover defect: `bin/ghidra_cli.cmd` still looked for its
  PowerShell implementation in `bin/`. It now calls `../scripts/ghidra_cli.ps1`; a real
  `CNavQuadTree::IsAreaBlockedByLines @ 0x00A76F30` decompile smoke test returned 111 lines with exit
  code 0. The canonical scheduled loop was restarted immediately and resumed that address.
- Promoted `CScriptedMapBrush::GetTheme @ 0x0082E330`, `GetThemeBlend @ 0x0082E3D0`, and
  `SetThemeBlend @ 0x0082E420` into curated VC7.1 source with focused in/out-of-bounds cell-layout
  tests. Followed with `CMap::SetEngineThemeAt @ 0x0081EE60`, `SetEngineBlendAt @ 0x0081EFC0`,
  `GetEngineThemeAt @ 0x0081F090`, and `GetEngineBlendAt @ 0x0081F170`, covering default,
  quarter-resolution, invalid-paint-type, palette, and blend paths. The candidate suite now passes
  67/67 compile and behavior tests. Their current objects are functional but not byte-identical
  (register allocation/code-shape differences), so no matching claim is made pending further tuning.
- Exhausted all four dependency-free rows from the candidate discovery gate:
  `Array_LinearSearchInt @ 0x0040E170`, `_onexit` dispatch `@ 0x00401296`, default floating-point
  precision setup `@ 0x0040135C`, and cleanup thunk `$E2 @ 0x00406770`. Each has a focused behavior
  oracle and VC7.1 object. `$E2` is also a raw byte-identical three-byte match (`c2 04 00`). This
  Three adjacent runtime bodies were then promoted with code-shape oracles: `initterm @ 0x00401356`,
  `__SEH_epilog @ 0x004013C7`, and `__dllonexit @ 0x004013DE`. All three are raw byte-identical;
  together with `$E2`, this raises the curated suite to 74/74 compile+behavior PASS (0.149% of
  49,553 functions). The import thunks retain their real forwarded CRT parameter signatures.
- Promoted `Vector_FreeElements @ 0x0040E130` with allocation/null/empty-range coverage. Its
  43-byte body is relocation-masked identical to retail; only the unresolved object-file `free`
  call payload differs. The curated suite is now 75/75 (0.151%), leaving roughly 421 additional
  verified promotions to reach the first 1% milestone.
- Promoted a six-function runtime/container helper batch: `__RTC_Initialize @ 0x004012CE`,
  `Std_Move_Backward @ 0x00405BA0`, `MemCmp_Unsigned16 @ 0x00403C60`,
  `WideString_EqualsLen @ 0x00404280`, `std_list_node_Init @ 0x00404690`, and
  `ListNode_Create_Circular @ 0x004046F0`. All focused tests pass. Wide-string equality and both
  node allocators are relocation-masked matches. Retail `ret 4` on the allocator pair proved one
  ignored stack argument beyond ECX/EDX, so their overrides now record the observed three-slot
  fastcall ABI instead of the agent's incomplete one-parameter declaration. Suite: 81/81 (0.163%).
- Promoted `__SEH_prolog @ 0x0040138C` as a naked VC7.1 runtime body with a 59-byte code-shape
  oracle. Its object is raw byte-for-byte identical to retail, including the fixed
  `__SEH_epilog @ 0x004013D8` handler address and FS-chain setup. Suite: 82/82 (0.165%), leaving
  roughly 414 further compile+behavior promotions to reach the first 1% milestone.
- Promoted `CCharString_ComputeCRC32 @ 0x00404310` with null-storage and normal-data behavior
  coverage. Its 46-byte object is relocation-masked identical to retail. The retail tail calls
  carry `CCRC::Calc`'s EAX result through the wrapper, so the curated signature records an
  unsigned-long CRC return instead of Ghidra's provisional `void`. Suite: 83/83 (0.167%), about
  413 promotions short of 1%.
- Promoted `$E2 @ 0x00404A90`, the 19-store renderer/static-data initializer. Its focused oracle
  verifies every byte and dword value, and its 147-byte VC7.1 object is relocation-masked identical
  to retail. Suite: 84/84 (0.170%), about 412 promotions short of 1%.
- Promoted `CUserProfileManager_SaveAndCleanup @ 0x0040BB90`. Tests cover the normal four-byte
  cursor walk and the allocator-block rollover/free path. The clean object is 144 bytes versus
  retail's 191 because it omits Ghidra-confirmed unreachable vector-copy scaffolding; classify it
  as functional/nonmatching. Suite: 85/85 (0.172%), about 411 promotions short of 1%.
- Web/tooling follow-up added a direct private-PDB oracle. Visual Studio's installed
  `llvm-pdbutil` plus DIA now supplies exact donor function signatures to every future auto-RE
  prompt without taking the Ghidra lock; `tools/query_pdb_oracle.ps1` exposes the same lookup
  manually. All ten functions in the active `CNavQuadTree` cluster received PDB-backed
  reference-qualified overrides. See `docs/DECOMP_ACCELERATORS.md` for the ranked BinDiff,
  reccmp, objdiff, Frida, angr, and ghidriff assessment.
- Fixed `tools/audit_candidate_signatures.py` so `_global` definitions (including `extern "C"`,
  calling-convention qualifiers, and MSVC `$` thunk names) are audited instead of automatically
  receiving `definition-not-parsed`. Signature-clean candidates increased from 62/105 to 77/105;
  all four newly promoted globals pass. The two remaining parse failures are genuinely malformed
  constructor/destructor-shaped agent snapshots rather than unsupported global syntax.
  The parser now also strips `__declspec(...)` declarator attributes before identifying the
  function name, which correctly audits naked CRT helpers such as `__SEH_prolog`.
- Wave 3 accepted the 1,251-byte `CWorldMap::GetMapNavigationAreaInit @ 0x0050A650` reconstruction
  after its second attempt. This captures the map bounds, navigation-point layers, physical-thing
  filtering, and initialization data flow needed for future editable nav generation; it remains an
  agent result until a dedicated compile/behavior promotion is practical.

### PDB-assisted navigation exact matches (2026-07-22 18:16 MDT)

- The first three fresh targets using the direct `Ego_r.pdb` signature oracle all passed:
  `VerifyTransitionPoints` in one checker round, `JoinNavigationLayers` in two, and
  `LinkToNeighbouringMaps` in two. Wave 3 remains unattended and continued into the two
  `ConnectVerticalMapEdge` overloads.
- Promoted `CNavQuadTree::VerifyTransitionPoints @ 0x00A77500` with empty and three-transition
  behavior coverage. VC7.1 emits the exact 53 retail instruction bytes after normalizing its two
  direct-call relocations.
- Promoted `CNavQuadTree::JoinNavigationLayers @ 0x00A77480`. Its behavior oracle covers reciprocal
  linking, missing-node rejection, and the `CANavQuadTreeNode+0x0F` disable flag. Its 117-byte VC7.1
  instruction stream is also an exact relocation-normalized retail match (four direct calls).
- The curated suite is now **87/87** compile+behavior PASS, or **0.176%** of 49,553 functions;
  approximately 409 more promotions reach the first 1% milestone. Lock-free refreshes report
  113 integrity-clean auto-RE sources, 104 checker passes, and 85/113 signature-clean snapshots.
  The retail-parity dashboard stays intentionally stale for these two exact bodies until the
  active Ghidra worker releases the project and the scheduled oracle exporter records them.

### Generated task queue + two more curated promotions (2026-07-22 18:27 MDT)

- Added `tools/build_promotion_queue.py` and wired it into the scheduled rebuild refresh. It excludes
  compile+behavior PASS addresses and ranks remaining agent results by checker, integrity, signature,
  lane, missing declarations, VC7.1 language repair count, compiler errors, and source size. Durable
  outputs are `rebuild/backlog/PROMOTION_QUEUE.md` and `promotion_queue.tsv`.
- Promoted `CNavQuadTree::LinkToNeighbouringMaps @ 0x00A7A650`. Its focused oracle verifies the four
  edge calls and coordinate order, `Initialising` lifetime, manager identity, and connector-heap
  finalization. Suite result is functional/nonmatching: 273 object bytes versus retail's 264 due to
  different x87/local scheduling.
- Corrected all three navigation-manager overrides to `CNavigatorManager const&`. Both retail and
  donor decorated names encode `ABV`; the direct `llvm-pdbutil pretty` display had dropped const.
- Promoted the queue's first-ranked target, `GFGetBuildNumber2 @ 0x00401F30`. Its missing/present-file
  behavior harness proves all 12 calls/cleanup steps, and VC7.1 reproduces the retail 162-byte
  instruction layout after relocation normalization.
- Curated suite: **89/89** compile+behavior PASS (**0.180%**), about **407** more promotions to 1%.
  Both `ConnectVerticalMapEdge` variants are now checker PASS; the unattended worker continued to
  naming target `0x004FE7F0` without intervention.

### Public recovery baseline + landscape relocation promotion (2026-07-22 19:04 MDT)

- Initialized the workspace as Git repository `BuffJesus/FableDecomp`, committed 1,324 source,
  documentation, compact-oracle, and generated-report files as `cac16c0`, and pushed `main`.
  Original binaries/PDBs, Ghidra/BSim databases, local build products, multi-gigabyte XML exports,
  and raw agent logs remain excluded. Local and remote commit hashes were verified identical.
- Continued ongoing work on branch `agent/continue-decomp`. The completed Wave 3 tail added checker
  passes for `ConditionalVirtualDispatch_OnLevelUnload` and both `CPersistInfo` insertion-sort
  steps, bringing the agent ledger to 121 integrity-clean candidates and 112 checker passes.
- Promoted `CEngineLandscapePatch::RelocateData @ 0x00BF3980`. The test suite covers its direct,
  asynchronous, list-head, list-interior, miss, and null paths. VC7.1 builds and all **90/90**
  focused behavior tests pass (**0.182%**, about **406** promotions to 1%).
- The retail comparison is `RELOCATION_MATCH`: 143/143 bytes, 57/57 fixed instruction bytes, and
  three expected direct-call relocation fields. The relocation-matched candidate count is now 17.
- The refreshed promotion queue has 31 uncompiled agent candidates. `entry @ 0x00401067` ranks
  first, followed by `CNavQuadTree::InitialiseLines @ 0x00A7A5E0`; use the latter when a smaller,
  subsystem-focused promotion is preferable to CRT startup work.
- Used the interval before the next unattended batch to promote `Getter_FieldE0_FC @ 0x00662020`.
  Its two-value behavior oracle passes and the VC7.1 object is a raw 13/13-byte retail match. A
  conservative `_global` fastcall override records the ECX overlay pointer and unsigned 32-bit
  return without assigning a speculative owner class.
- Curated status is now **91/91** compile+behavior PASS (**0.184%**), with 33 raw retail matches,
  17 relocation-normalized matches, 68 verified lifts overall, and about **405** promotions left to
  the first 1% milestone. The remaining uncompiled agent queue fell from 31 to 30.
- Fixed a deterministic scheduled-task collision: `FableTLC Rebuild Refresh` starts roughly 30
  seconds before `FableTLC Auto RE Wave 2`, so the latter had begun deferring every cycle. Wave 3
  now waits up to five minutes for a verified live refresh PID and resumes immediately after Ghidra
  is released. A manual scheduled-task smoke run entered `CNavQuadTree::Initialise @ 0x00A7A8D0`
  at 19:11:30, proving the unattended loop is active again.

### RE evidence applied to FableForge, ForgeFSE, and FQT (2026-07-22 19:25 MDT)

- Extended `tools/export_fse_native_overlay.py` so the canonical schema-1.1 overlay also mirrors to
  `D:\Code\FQT\FQT\FSE_Source\docs\fse_native_overlay.json`, alongside the existing FableForge and
  ForgeFSE mirrors. Current counts: 931 FSE functions, 518 exact-name correlations, 462 owner-aligned
  recommendations, 53 verified engine implementations, zero verified recommended bindings, and
  zero approved runtime hooks.
- FableForge's native-overlay test no longer hard-codes a stale verified-function count; it checks
  structural agreement with the generated list. Full CTest result: 6/6 pass.
- FQT now loads the overlay through a policy-gated service, correlates by `scope + name`, and shows
  native candidate/provenance state in API Reference. It rejects duplicate keys, unsafe policy,
  stale function counts, and recommendations that lack candidate evidence. Full .NET result:
  190/190 tests pass.
- ForgeFSE now has `scripts/Test-FseNativeOverlay.ps1`. It cross-checks all manifest keys and summary
  counts, candidate evidence, match statuses, and both non-equivalence policy flags before the data
  can be considered for binding work. The current 931-function overlay passes.
- No recommendation is an executable hook. A binding still requires target-build identity,
  owner/vtable or callsite confirmation, calling-convention validation, and a ForgeFSE runtime probe.

### ForgeFSE binding lane added to unattended RE (2026-07-22 19:40 MDT)

- Added `tools/build_forgefse_binding_queue.py`. It validates the overlay safety policy and candidate
  evidence, deduplicates targets by retail address, classifies verification stage, and writes
  `rebuild/backlog/forgefse-binding-queue.tsv` plus `FORGEFSE_BINDING_QUEUE.md` deterministically.
- Current queue: 462 recommendations / 462 unique addresses; 167 direct wrapper-to-retail
  signatures, 269 expected wrapper adapters, and 26 signature/arity reviews. All remain
  `needs-reversal`; zero are implementation-verified bindings or approved hooks.
- `tools/run_rebuild_refresh.ps1` regenerates the queue immediately after refreshing the overlay.
  Its fingerprint now includes the builder, so ranking changes cannot be skipped as “unchanged.”
- `lift/scripts/run_re_agent_wave3_queue.ps1` loads this lane after the completed curated seeds and
  before naming/prototype backlog rows. It uses the existing single PID, refresh wait, timeouts,
  cross-wave ledgers, and compiled-behavior exclusion gate—there is no competing Ghidra process.
- Static selection smoke test chose 16 direct-signature targets, beginning with
  `Entity.MsgIsKicked @ 0x004AAF80`, `Entity.MsgOpenedChest @ 0x004AADA0`, and
  `Entity.SetAsUsable @ 0x004AB040`. The currently active pre-change batch is allowed to finish;
  the next scheduled refill automatically uses the ForgeFSE lane.
- An agent PASS is only structural evidence. Nothing writes ForgeFSE pointer tables or changes
  `hookApproved`; build identity, owner/callsite, calling convention, focused behavior, and a live
  ForgeFSE probe are still required.
- Added `tools/audit_forgefse_binding_slots.py` to compare ForgeFSE's 918 CGSI `pVTable[N]`
  assignments with the aligned retail slot map and current overlay. Initial result: 442/443 exact
  Quest slot/address matches, zero missing assignment families. It correctly groups the four
  `CameraUseCameraPoint_*` overload entries.
- The only mismatch exposed a bad exact-name recommendation:
  `CTCQuestCard::GetRegionName @ 0x007025A0` versus ForgeFSE slot 15 / decorated CGSI method
  `CGameScriptInterface::GetRegionName @ 0x0088E340`. Added a vtable/decorated-symbol-backed
  override; the post-agent refresh applied it and the audit now requires 443/443.
- The forced refresh completed successfully: 132 integrity-clean agent candidates, 124 checker
  passes, 91 curated compile+behavior passes, and a **443/443 PASS** for ForgeFSE Quest vtable
  slot/address entries. No assignment family is missing.
- Cleanly stopped the old batch after its in-flight retry (no process kill), refreshed, removed the
  stop marker, and manually started the scheduled task. The live 16-target ForgeFSE batch began at
  19:48:25 with `Entity.MsgIsKicked -> CScriptThing::MsgIsKicked @ 0x004AAF80`.

### Navigation + ForgeFSE wrapper promotions (2026-07-22 20:27 MDT)

- Promoted `CNavQuadTree::InitialiseLines @ 0x00A7A5E0`: VC7.1 compile and grow/shrink/zero behavior
  tests pass, the decorated nested-STL signature matches the donor, and its 102-byte body is a
  relocation-normalized retail match.
- Promoted `CGameScriptInterface::CameraCancelScreenEffect @ 0x0088EDB0` and
  `CameraUseScreenEffect @ 0x0088EDA0`. Their exact public-virtual-const symbols match donor/retail,
  and their complete bodies are raw matches (`c3` and `c2 0c 00`).
- Fresh dashboard: 94/94 compile+behavior PASS (0.190%), 71 verified functional-or-matching
  functions (0.143%), 51 raw byte-identical functions, and 402 promotions left to the first 1% lane.
- The first ForgeFSE batch completed 16/16 structural PASS. The scheduled refresh ingested it and
  the three new curated promotions. At 20:25 the next RE task correctly waited for that refresh,
  resumed automatically at 20:26:44, and started a new 16-target batch with
  `Quest.ChangeHeroMoralityDueToPicklock @ 0x0089A0E0`.

### Quest-card and ForgeTest smoke handoff (2026-07-22 22:45 MDT)

- Focused runtime folder:
  `work/runtime_smoke_quest_terrain_20260722/`. Stable user screenshots were copied to
  `captures_native_run4/user_wasp_card_1337.png` and
  `captures_native_run4/user_quest_region_block.png`; do not rely on the original temp attachment
  paths.
- Install state is restored. `revert_alias_probe.ps1` restored the prelaunch extender DLL
  (`C3C88AD94AFB...`) and removed `FSE/map_resource_alias.ini`; `revert.ps1` restored
  `data\CompiledDefs\game.bin` (`E3C7E368B515...`), `names.bin` (`AC6288FA493E...`), and
  `FSE\Master\FSE_Master.lua` (`8857D08DA987...`). No live `Fable.exe` process remained when the
  handoff was written.
- Run 1 proved native ForgeTest streams and is playable but its terrain renders black. Evidence:
  `results/FableScriptExtender_20260722_211516.log`,
  `captures/fable_20260722_211430_575.png`, and
  `captures/fable_20260722_211526_518.png`.
- Run 2 proved Lua-time map aliasing is too late for static-map bootstrap. Static maps have already
  opened before `FSE_Master.Main()` arms the alias.
- Run 3 proved early whole-file aliasing works technically but is the wrong fix. The exact source
  `Data\Levels\FinalAlbion\ForgeTest.lev` was replaced with donor
  `Darkwood9_Leadout_01.lev`, but the donor registered its own bank/bounds
  `(2816,2368)..(2848,2400)` instead of ForgeTest's native
  `(2784,2560)..(2816,2592)`, producing a white/empty terrain hole rather than valid ForgeTest
  terrain. Preserve ForgeTest's LEV/heightfield and isolate only its landscape material/bank
  dependency.
- Run 4 used the known-good `OBJECT_QUEST_CARD_WASP_MENACE` asset. The user confirmed the Wasp
  Menace card appeared with 1337 gold and Guildmaster "new quest available" audio. Local source also
  confirms ForgeFSE `Quest:GiveQuestCardDirectly()` takes a quest-card object name first; the
  pasted `textDBEntry` interpretation is wrong for this build.
- Run 4 also showed the active quest can block terrain validation. The user screenshot shows the
  engine modal: "You are attempting to leave a Quest region..." with Reload/Stay options while the
  ForgeTest teleport banner is active. That is a quest-region transition guard, not a map-streaming
  failure. Future terrain smokes should not activate a quest, or should clear/deactivate the card
  before calling `GoToMapSlot`.
- Run 4 produced useful native telemetry anyway. `results/FableScriptExtender_20260722_223533.log`
  and `focused_markers_20260722_223533.txt` show native `ForgeTest.lev` opened at bank entry 426
  with bounds `(2784,2560,37.8359)..(2816,2592,83)`, and the script later called
  `GoToMapSlot(399, 2800, 2576, 74)` with `ok=true`. The collector also saved
  `results/Fable.exe.16316.dmp`; keep it with this run.
- Attached decomp pointer:
  `lift/reports/wave3/code/0x0089B330_global_IsHeroNaked_CGameScriptInterface_UBE_NXZ.cpp`.
  It is a named `CGameScriptInterface::IsHeroNaked` reconstruction with useful overlays at
  `this+0x14`, target `+0x28`, interface map storage `+0x44`, map end `+0x48`, and flag byte
  `+0x91`. Before promotion, fix/review the end-sentinel path: the current code can set `pEntry`
  to `m_pTCInterfaceMapEnd` and still call `reinterpret_cast<CTCShop*>(pEntry->m_pInterface)`.
  Also verify whether `CTCShop::GetName()` is really the nakedness predicate or just a bad donor
  name for the interface method at type key `0x5E`.

---

## RESUME POINT — Custom-entity (NPC) pipeline + forge-entity design (2026-07-24)

Full custom-NPC toolchain proven end-to-end this session. All pieces work; the remaining work is
(a) finishing the custom CREATURE def wiring and (b) building the centralized `forge entity` orchestrator.

### What works (verified, mostly in-game)
- **Native NPC spawn** — TWO requirements (both mandatory; see [[fable-level-modding-gotchas]]):
  (1) the game reads `data/Levels/FinalAlbion.wad`, NOT loose .tng (repack with `forge wad repack`);
  (2) the thing must be in the correct `XXXSectionStart <quest>` section (donor book trader is in
  `Q_NewOakValeIntro_PreAttack`; EOF-append lands in PostAttack = post-raid only). Builder:
  `work/quest_card_custom_20260723/npc_placement/build_childhood_npc_west.py`. Runtime alternative
  (no TNG/WAD/section): ForgeFSE `Quest:CreateCreatureNearby("CREATURE_TRADER_01", hero:GetPos(), r, scriptName)`.
- **Custom mesh** — FBX → headless Blender (`C:/Programs/Blender/blender.exe --background --factory-startup`)
  decimate (101k→3k tris) → `mesh_rw.compose_mesh` → `big_write.rebuild(adds=)` into graphics.big.
  Material MUST use a real `diffuse_id` (0 → short Info → build_model 0 LODs). Scripts:
  `work/meshy_npc_mesh/` (static) + `work/meshy_npc_mesh/skinned/` (type-5).
- **Skinned mesh** — auto-weight to a donor's 63-bone Bip01 skeleton (`mesh_rw.clone_skeleton(MESH_TRADER_01)`),
  Blender `ARMATURE_AUTO` weights (≤3/vert, sum 255), compose type-5. VERIFIED (has_skeleton, topology, weights)
  in a graphics.big copy. Inherits standard biped anims. `work/meshy_npc_mesh/skinned/`.
- **Texture** — PNG → 512² DXT1 (`texture_build.build_entry`) → `big_write.rebuild(adds=)` into
  textures.big `GBANK_MAIN_PC`. `work/meshy_npc_mesh/inject_tex.py` (Meshy tex = id 6291).
- **Dialogue** — ElevenLabs TTS → WAV → `tools/dialogue_pipeline.py stage --add` = Xbox-ADPCM .lut +
  auto lipsync + text.big + snds.bin. **CONFIRMED IN-GAME: grown banks (`--add`) ARE accepted by the
  live engine** (resolves the old "unproven" caveat). Normalize TTS to ~-14 LUFS (raw ElevenLabs ~-24 dB
  is too quiet: `ffmpeg -af loudnorm=I=-14:TP=-1`). SecretHunt NPC has 5 voiced lines (Callum voice).
- **GiveHeroYesNoQuestion answer mapping** — button1→**1**, button2→**0**, button3→**2** (per working
  GhostGranny sample; the ForgeFSE log label "0=Btn1" is MISLEADING). Setting a flag on the wrong index
  inverts accept/decline. SecretHunt entity fixed.

### THE creature→body-mesh mechanism (ultracode, HIGH confidence)
`CCreatureDef` **`Graphic` field** (CRC tag `0x2e6b63c8` = crc0("Graphic")), layout
`{u32 kind, u32 modelId, u32 zero, f32 scale, u8 flag}` + optional nested CRC-tagged sub-graphics
(eyes/attachments). **`modelId` (payload offset +4) = the graphics.big MBANK_ALLMESHES TOC *id*** of the
body mesh (1:1 by id). CREATURE_TRADER_01 modelId=**5149** = `MESH_BS_MALE_MIDDLE_UNCLOTHED_01` (a naked
base body; clothes layered via `InitialAppearanceModifiers` → CAppearanceModifierDef.Graphics[]). That's
why MESH_TRADER_01/5370 was never in the def. Verified across 9 creatures. Engine chain: CEngineGraphic
`0x00434b50` → CTCGraphicAppearance `GetRenderMeshObject 0x004bc750`. **To make a custom creature:** inject
the skinned mesh → get its id N → set a new/cloned creature def's `Graphic.modelId` (+4) = N (hand-patch
confirmed; `forge defs set-field` targeting the nested +4 not yet exercised). Custom-creature workflow
(build phase) finishing under `work/meshy_npc_mesh/custom_creature/`.

### NEXT: `forge entity` centralized orchestrator (design agreed, not built)
Pain = hand-wiring cross-refs across game.bin/graphics.big/textures.big/text.big/.lut/dialogue.big/TNG.
Design: ONE `*.entity.json` manifest (name, mesh fbx, texture png, def clone+overrides, dialogue lines,
spawn) → `forge entity build <manifest> <game-root> [--deploy]` that (1) allocates all ids from a central
`entities.registry.json`, (2) runs each sub-pipeline, (3) auto cross-wires (mesh mat←tex id;
def Graphic.modelId←mesh id; text→SpeechBank→snds→lut→lipsync; spawn←def name), (4) handles WAD/section
gotchas, (5) stages + atomic deploy + rollback. Implement as a Python orchestrator wrapping `forge` +
the tools above, exposed as `forge entity`. First slice: id registry + mesh/texture/def cross-wire.

### Other session outcomes
- Co-op multiplayer subsystem decompiled → COMPLETE-BUT-GATED (docs/FINDINGS.md); 1 flag
  (`CNetworkClient+0x2662`) from life. Auto-RE wave3 lane pointed at the co-op cluster (Codex-quota blocked).
- Decomp: +11 fse2 byte-matches via the diff-feedback refine loop (the effective cracker vs 0 from
  first-pass authoring). Real byte-match total ~1,700 (3.43%) — README corrected from stale 57.

### BLOCKER (2026-07-24, next session): custom game.bin def not engine-recognized
Deployed CREATURE_MESHY_HUNTER (added to game.bin via forgecore File::addEntry) — `forge defs list/decode`
sees it (entry 14781, Graphic.modelId=8113, all_tags_ok) but the ENGINE does NOT:
`Quest:CreateCreatureNearby("CREATURE_MESHY_HUNTER") -> nil (def missing?)` and the load became
unstable/crashed. Same class as the TNG bug: **forge's parser ≠ the engine's game.bin loader.** Appending
a def entry is insufficient — the engine builds its def name→index table from a count/header/hash our
append did not extend (candidates: a per-type def count, the names.bin index/hash, or indexInDefinition
ordering). NEXT: RE the game.bin def-load + name-resolution path (grep name DB for CGameDefinitionManager /
LoadDefinitions / GetDefinition / def name hash; check for a def-count field the engine trusts) so an added
def is actually resolvable by name. Until then, custom NPCs must reuse an existing creature def (e.g.
CREATURE_TRADER_01) — which works and is voiced. Rolled back cleanly; backups in
FSE/backups/custom_creature_20260724/. All custom-creature build artifacts (verified offline) remain in
work/meshy_npc_mesh/custom_creature/.

### TOMORROW'S PLAN — custom creature working + next auto-RE rounds (2026-07-24)

**A. Get a custom-mesh NPC in-game (two tracks):**
- **Track A — QUICK WIN, low risk (do first): IN-PLACE edit, no append.** The engine rejects *appended*
  defs (count/index not extended) but accepts *in-place* field edits (same size, same slot). So patch an
  EXISTING creature's `Graphic.modelId` in place instead of adding a new def:
  - Simplest: hand-patch `CREATURE_TRADER_01`'s Graphic.modelId at payload offset **1122** (tag@1114),
    `5149 -> <our mesh id>`. game.bin size UNCHANGED. Deploy game.bin + graphics.big (skinned MESH under a
    name) + textures.big (tex 6291). Result: every trader renders the Meshy body — proves the custom skinned
    mesh renders + animates in-game. Downside: all traders, not a distinct creature.
  - Better: repurpose a RARE/unused creature def (spawn by its name via CreateCreatureNearby) so only "our"
    NPC changes. Same in-place mechanism.
  - Also strip clothes: clear/patch the cloned def's `InitialAppearanceModifiers` (CAppearanceModifierDef
    layers trader clothing over the base body) so the bare Meshy body shows.
- **Track B — PROPER new def (needs RE):** decompile `CGameDefinitionManager::InitAndCompile`
  **0x0044c72b** (+ ctor 0x0044c6c2, dtor 0x00450bff) to learn how game.bin defs are counted / name-indexed
  / hashed at load, then fix `work/meshy_npc_mesh/custom_creature/02_add_creature.cpp` to update whatever the
  engine trusts (candidate: a per-bank/def COUNT field, a names.bin name→id hash, or indexInDefinition
  ordering). Then a truly distinct `CREATURE_MESHY_HUNTER` resolves by name. All build artifacts already
  staged + offline-verified in work/meshy_npc_mesh/custom_creature/. Deploy rollback: FSE/backups/custom_creature_20260724/.

**B. Next auto-RE candidate rounds (run via the CLAUDE Workflow loop — the Codex wave3 lane is quota-blocked):**
- **Batch A (TOP — unblocks custom NPC):** `CGameDefinitionManager::InitAndCompile` 0x0044c72b, ctor
  0x0044c6c2; `CreateCreature` 0x008a9100 + `CreateCreatureNearby` 0x0089f300 (how they resolve a def by
  NAME); `CCreatureDef` 0x006768c0 / `CThingCreatureDef` 0x006710d0. Decompile these to solve Track B AND
  document the def-load contract. Use an analysis Workflow like the co-op one (bundles from Fable.exe bytes
  via pe_oracle + objdump).
- **Batch B (custom-NPC polish):** `CAppearanceModifierDef` 0x004546d5, `CAppearanceDef` 0x0046a174,
  `CEngineGraphic` 0x00434b50, `CTCGraphicAppearance::GetRenderMeshObject` 0x004bc750 — the appearance/clothing
  + mesh-render path (strip clothes, understand attachment sub-graphics).
- **Batch C (co-op revival):** the 24-target co-op cluster is prepended to the wave3 seed (Codex-blocked).
  Either wait for Codex quota, or byte-match-reconstruct the gate/protocol via the Claude refine loop:
  `IsMultiplayerGameActive` 0x449d20, `InitialiseAsNetworkHost` ~0x4ae940, `CGameEventPackage`
  Compress/InitFromCompressedBuffer 0x9f1810/0x9f1870, and REBUILD `CheckSync` 0x4165e8 (the one gutted piece).
- **Batch D (continue byte-match reconstruction):** ~64 remaining fse2 methods via the diff-feedback refine
  loop (the proven cracker — first-pass authoring yields ~0, refine round lands them). Then the next
  accessor/setter tiers from rebuild/backlog. Pipeline: tools/decomp_pipeline/ (author_wf -> verify_and_land;
  refine_wf on close DIFFERs).
- **Lane note:** the productive decomp engine is the Claude Workflow loop, NOT the Codex `re-agent.exe` wave3
  lane (quota-limited). Drive Batch A/D through Workflow (schema-constrained author/refine/analyze), land via
  verify_and_land.

---

## RESUME POINT — game.bin def-load contract RE'd; BOTH append bugs fixed (2026-07-24, authoritative)

The standing blocker ("custom game.bin def not engine-recognized") is **SOLVED at the byte level.**
RE'd the def-load / name-resolution contract via the Claude Workflow loop (`defload-contract-re`:
8 decode agents → synthesis → adversarial verify, **verdict CONFIRMED**). Full contract:
`docs/DEF_LOAD_CONTRACT.md`. Artifacts: `work/def_load_re/`.

### Root cause was NOT the counts — it was two payload/CRC bugs (both now fixed + offline-verified)
- **BUG #1 — wrong names.bin CRC.** forge wrote `0xFFFFFFFF - mz_crc32(name)` which matches **0/13593**
  retail names. The engine hash is `crc0` = seed-0 reflected CRC-32 poly `0xEDB88320`, **no final
  inversion** (proven: 13593/13593 stored CRCs == crc0). Every NEW appended name got an un-resolvable
  CRC → `CreateCreature(name) → nil`. **FIXED** in `D:\Code\FableForge\libs\forgecore\src\bin.cpp`
  `nameCrc()`; `libforgecore.a` rebuilt. Verified: appended `CREATURE_MESHY_HUNTER` now stores
  `0x5A11F1E5` == crc0(name).
- **BUG #2 — stale internal global entry-index refs.** A creature payload carries its own global entry
  index as self/owner back-refs (`CREATURE_TRADER_01`=1549 at payload offsets {25,193,301}). A byte-clone
  keeps 1549 but lands at a new index → engine wires to wrong entries → instability. **FIXED** in
  `work/meshy_npc_mesh/custom_creature/02_add_creature.cpp` (rewrite every `u32==donorGlobalIdx` →
  landing index; leave SHARED component sub-defs alone). Verified: "retargeted 3 self-index refs
  1549 -> 14781", reload clean.
- Counts were already correct (writer extends nameCount/tableSize/entryCount, recomputes dense
  indexInDefinition). The `crc0` id is now the canonical fact for ALL Fable name hashing.

### Track A (quick win) — STAGED, ready for your in-game test
`03_inplace_modelid.cpp` → `inplace_modelid.exe`: IN-PLACE repoint of `CREATURE_TRADER_01`
`Graphic.modelId` 5149→8113 (our Meshy skinned mesh) via `setEntryData` (no append; sidesteps both
bugs by construction — CONFIRMED safe). Staged: `CompiledDefs_inplace/game.bin`+`names.bin`,
`graphics_meshy_hunter.big` (mesh id 8113), `textures_meshy.big`. Deploy/rollback:
`deploy_trackA.sh deploy|rollback` (backs up 4 retail files first). **NOT yet deployed — your call.**
⚠ **Rebuild from a pristine base first:** the current Steam install is at **14781** entries (+20 stale
appends over pristine 14761) — likely carrying the old wrong-CRC names. Steam → Verify integrity of game
files, then re-run `inplace_modelid.exe`, THEN deploy. Test: load near a trader; it should render the
Meshy body + animate (biped set). Clothing may still layer (InitialAppearanceModifiers = polish).

### Track B (distinct named creature) — append tool now offline-clean
`add_creature.exe` (rebuilt w/ both fixes) produces an engine-valid `CREATURE_MESHY_HUNTER` append:
`CompiledDefs_append_fixed/`. Still needs (a) a pristine base and (b) an in-game name-resolve test.
Recommended controlled A/B/C (per DEF_LOAD_CONTRACT open questions): append with correct-crc-only vs
index-rewrite-only vs both, to isolate which flips the nil. For a creature with its OWN sub-defs, append
the full coordinated set + retarget every self+sibling index (`work/append_fix/APPEND_ALGORITHM.md`).

### Next best tasks
1. (User) Steam-verify → rebuild Track A from pristine → deploy → confirm Meshy body renders in-game.
2. Land Track B in-game: deploy `CompiledDefs_append_fixed` + the two bigs on a pristine base, spawn
   `CreateCreatureNearby("CREATURE_MESHY_HUNTER")`, confirm non-nil. Run the A/B/C isolation.
3. Batch B (appearance/clothing): decompile `CAppearanceModifierDef` 0x004546d5, `CAppearanceDef`
   0x0046a174 to strip trader clothing off the base body. Same Workflow pattern.
4. Then `forge entity` orchestrator (design in the 2026-07-24 custom-entity section above) can bake in
   the now-known def-load contract (crc0 names + index retargeting) as its game.bin cross-wire step.

---

## Background decomp session addendum (2026-07-24) — 3 contracts landed + byte-match lanes

Three analysis workflows completed via the Claude Workflow loop; all findings documented (read these):
- **`docs/DEF_LOAD_CONTRACT.md`** — game.bin def-load; both append bugs FIXED (crc0 + self-index retarget).
  Track B append tool clean on pristine base → `work/meshy_npc_mesh/custom_creature/CompiledDefs_append_fixed`.
- **`docs/APPEARANCE_STRIP_FINDINGS.md`** — clothing layering. **EMPIRICAL CORRECTION:** TRADER_01's
  `InitialAppearanceModifiers` is `count=0` (empty) — NOT the clothing source. Real appearance = the linked
  `CAppearanceDef`[10745] (18 KB, ~20 mesh ids), needs its own RE pass. `count=0` proven engine-valid.
  **Don't build an IAM strip — deploy Track B and observe what actually renders first.**
- **`docs/COOP_REVIVAL.md`** — enable gate `[CNetworkClient+0x2662]` (opcode-proven), CGameEvent wire format,
  CheckSync 0x004165E8 gutted + rebuild spec. Verify PLAUSIBLE; corrected several fabricated addresses
  (0x4EBA10 phantom, 0x0049E0B0→0x0049DFB0). Enable via InitialiseAsLocal, NOT a bare +0x2662 poke (CTD risk).

**Byte-match author→land loop** (tools/decomp_pipeline): validated end-to-end. batch3 authored but it's ~83%
pre-landed residue (0 fresh wins; first-pass≈0 is normal, the diff-refine round is the cracker). Fixed two
lander bugs (missing-`name` fallback to oracle; stale scratchpad path → `rebuild/build/landverify`). Fresh-yield
map computed across pending batches (~1,200 un-landed): batch14=292, batch16=205, batch17=125, batch15=108,
fse1=69, fse2=64(refine lane). Reusable author workflow: `scratchpad/author_bytematch.js` (args
`{bundleDir, addrs}`); land with `verify_and_land.py <task.output> <oracle.tsv> --land`.

### Next best tasks
1. (User) deploy Track B, spawn CREATURE_MESHY_HUNTER, observe render (bare body vs CAppearanceDef clothing).
2. Land batch17 wins (author-batch17 running); then fan batch14/16/15 + fse2 refine lane for fresh promotions.
3. If custom body is clothed: RE `CAppearanceDef`[10745] structure to null/repoint its mesh list.
4. Co-op: resolve the masked InitialiseAsLocal precondition + find the tag-1 sync-event producer, then attempt enable.

---

## Background decomp resume (2026-07-25) - Wave 3 live again

The scheduled Wave 3 runner recovered from the previous Codex quota block. Its first resumed
16-target co-op batch completed with 16/16 structural PASS results:

- player/network gate and local-client lifecycle (`0x00449D20` through `0x004AEBA0`);
- event apply/sync/save integration (`0x00416670`, `0x004165E8`, `0x0041726D`,
  `0x00416148`, `0x004161A7`);
- event and package-set wire codecs (`0x009F1810`, `0x009F1870`, `0x009F19A0`,
  `0x009F1AC0`).

These are durable generated reports under `lift/reports/wave3/code/`, not curated promotions.
The codec reconstructions independently confirm the framing already documented in
`docs/COOP_REVIVAL.md`: package count byte, per-package event count plus 32-bit sequence, then
dense event records `[u16 id|replacement][u8 player][u8 payload length][payload]`.
`CheckSync` also reconfirms the retail stub: it reads the remote checksum/checksum/sequence tuple
and discards it without a comparison.

The next scheduled batch started immediately on the remaining co-op/spirit cluster. At this
checkpoint `AddPackage`, `CProcessedInput::AddGameEvent`, and `CTCCoopSpirit::Construct` passed;
`CTCCoopSpirit::OnCreate @ 0x006700F0` exhausted both attempts with structural FAIL, and the runner
continued to `UpdateAttractionToMaster @ 0x006701A0`. Let the bounded runner continue; do not treat
the failed `OnCreate` decompilation as evidence that the retail function is absent.

The post-batch refresh exposed an older compile-driver integration bug: 1,038 catalog behavior
tests are deliberately self-contained, but `rebuild/build_candidates.ps1` always linked
`source.obj + test.obj`, causing duplicate-symbol failures. The driver now prefers that external
link and falls back to a test-only link, matching `verify_and_land.py`. Stale tests that could
cleanly exercise the real source object were converted to external declarations. Current gate:
**1,850/1,850 VC7.1 compiles and 1,850/1,850 behavior PASS**. The remaining retail-oracle/parity
refresh must run after Wave 3 releases Ghidra.

### Wave 3 co-op tail and refresh repair (2026-07-25 15:14 MDT)

The second co-op/spirit batch was intentionally stopped after its useful tail so the refreshed
VC7.1 exclusion set can prevent already-compiled ForgeFSE bindings from being selected again.
Durable ledger results after the first 16/16 batch:

- `CGameEventPackageSet::AddPackage @ 0x009F16F0`,
  `CProcessedInput::AddGameEvent @ 0x00A0D340`, and
  `CTCCoopSpirit::Construct @ 0x004D55D0`: checker PASS.
- `CTCCoopSpirit::OnCreate @ 0x006700F0`: the first outer attempt failed, but the second passed.
  The resulting source is still review-only: it confuses definition pointers, a morph-entry pair,
  `std::_Cons_val`, and unrelated definition template types. Do not promote it from checker status.
- `CTCCoopSpirit::UpdateAttractionToMaster @ 0x006701A0`: checker PASS, but unsafe as generated.
  `pPhysics` is uninitialized when the physics-interface flag is clear, and a failed `LowerBound`
  assigns the end sentinel before dereferencing `pEntry->m_Value`.
- `CTCCoopSpirit::SwapToHero @ 0x0066FF20`: the first outer attempt failed and the second passed.
  It needs ABI/ownership review of the raw helper calls and local temporary before promotion.
- `CTCCoopSpirit::UpdateScore @ 0x00670710`: checker PASS, but its failed `LowerBound` path likewise
  dereferences the map-end sentinel. Keep it review-only.
- `CWorld::EAMoveSpirit @ 0x0062C0E0`: checker PASS, but needs interface-lookup and packet-buffer
  lifetime/layout review before promotion.

The final selected row before the stop was the stale
`CGameScriptInterface::SetFactionAsAlliedToFaction @ 0x00890870`; it passed, then the queue exited
cleanly. The next scheduled refill will read the repaired 1,850-row VC7.1 exclusion catalog.

The forced rebuild refresh then exposed a Windows command-line limit in candidate oracle export:
all 1,850 requested addresses were being appended as Ghidra script arguments. The exporter now
writes a UTF-8 response file and invokes `ExportFunctionOracle.java` with `@<address-file>`; the
Ghidra script accepts either that response-file form or the legacy direct-address form. A direct
smoke export succeeded. It requested all 1,850 compiled rows and emitted 1,722 actual Ghidra
function-start oracles; the other 128 addresses are aliases/interior addresses or otherwise lack a
function start in the current Ghidra database.

The first downstream audit after that repair caught two owner-ranking regressions:
`GiveHeroExpression` and `DisplayTutorial` selected byte-matched unqualified helpers at
`0x004383D0`/`0x00435070` instead of the decorated, vtable-backed CGSI methods at
`0x0088FC60`/`0x0089E710`. `export_fse_native_overlay.py` now gives qualified
`CGameScriptInterface::` identities stronger owner evidence than an unqualified helper merely
grouped into the same module. The regenerated audit passes **452/452** Quest bindings with zero
mismatches or missing assignment families.

`tools/run_rebuild_refresh.ps1` now also supports `-ResumeAfterOracle`. It validates that both the
compiled manifest and oracle manifest exist, then resumes at retail parity. This was used after a
detached console produced Python exit 120 from a broken stdout pipe; the underlying parity files
had been written, but the stage correctly remained failed until rerun through the canonical
foreground path.

Final refresh completed at 15:34 MDT:

- curated catalog: **1,850/1,850 VC7.1 compile PASS and 1,850/1,850 behavior PASS**;
- retail parity: **914 exact + 609 relocation matches**, 199 differing, 128 oracle-missing;
- generated Wave/agent sources: 534 total, 526 checker PASS, 227 host C++20 syntax PASS;
- candidate signature audit: 467 PASS, 67 review;
- tooling SDK: PASS; ForgeFSE Quest slot audit: **452/452 PASS**;
- reconstruction backlog, promotion queue, and `rebuild/COVERAGE.md` regenerated;
- canonical state: `rebuild/refresh.state.json`, fingerprint
  `feb58d667b9930ca84d5eeea147aa832a60d0c56e8250f0b76d93cbb6db25f2f`.

The scheduled Wave 3 task was manually triggered once after the clean refresh to verify its new
selection set. It started a fresh bounded 16-target batch at 15:35 MDT with
`Quest.DontPopulateNextLoadedRegion @ 0x0088E380`, confirming that the stale
`0x00890870` compiled binding is now excluded. Leave this batch running under the normal scheduled
task; `lift/state/re-agent-wave3-queue.log` remains authoritative.

### Public progress update and workspace housekeeping (2026-07-25)

`README.md` now reports the canonical 1,850-row VC7.1 gate instead of the stale July 23 counts and
contains a dedicated cut-multiplayer overview: surviving four-player/event replication machinery,
the `CNetworkClient+0x2662` enable contract, the gutted `CheckSync`, unsafe raw-poke caveat, and the
remaining revival order. `docs/COOP_REVIVAL.md` is the detailed byte-level source.

Workspace organization is now documented in `docs/WORKSPACE_LAYOUT.md`.
`tools/organize_workspace.ps1` safely archives only loose root scratch/build debris and delegates
RE-agent transcript cleanup to `lift/scripts/organize_lift.ps1`; it never deletes files or reshapes
live `rebuild/`, `lift/`, or documented `work/` trees. The first run moved 18 old root artifacts
(12 `.obj` files, five scratch/header files, and `FableTLC_RE_docs.zip`) into
`work/scratch/root/{objects,sources}/` and `snapshots/local-archives/`, with no collisions or
protected files. The local 909 MiB `FSE/` deployment-backup tree remains in place because active
deployment scripts address it, but it is now explicitly ignored by Git.

### Wave 3 ForgeFSE wrapper batch review (2026-07-25 16:33 MDT)

The post-publication 16-target batch completed with a final checker PASS for every selected address.
`SetWanderCentrePoint @ 0x008A23B0` and
`SetWeaponAsHerosActiveWeapon @ 0x00898B30` each recorded an initial failed outer attempt before a
later PASS. These are structural results under `lift/reports/wave3/code/`, not promotions.

Manual semantic review found that checker PASS materially overstates this batch:

- Ten wrappers assign a failed `LowerBound` result to the vector-map end sentinel and then read its
  value: `SetHeroAsWearing`, `SetNumberOfTimesHeroHasHadSex`,
  `SetPlayerCreatureOnlyTarget`, `SetPreferredQuickAccessItem`, `SetReadableObjectText`,
  `SetReadableObjectTextTag`, `SetThingAndCarriedItemsNotAffectedByScreenFilter`,
  `SetTrapAsActive`, `SetWanderCentrePoint`, and `SetWeaponAsHerosActiveWeapon`.
  Treat all ten as unsafe review-only source.
- `SetQuestInfoText @ 0x00891A00` mixes an unrelated morph-entry allocator/template into text-bank
  lookup, aliases its parameter storage as several incompatible temporaries, and destroys the wrong
  local. It needs reconstruction from bytes rather than incremental cleanup.
- `SetThingAsConscious @ 0x008A9610` calls helpers labeled as unrelated GUI/lightning classes for
  creature-action construction/destruction. Those BSim-derived identities are not credible enough
  for promotion.
- `StopSound @ 0x0088F660` needs vtable/dispatch confirmation to rule out self-recursion, and
  `SetSoundThemesAsEnabledForRegion @ 0x0088E0B0` still needs world-vtable/region-category ABI review.
- The two direct owner-byte writes, `DontPopulateNextLoadedRegion @ 0x0088E380` and
  `SetGuildMasterMessages @ 0x0088E200`, are the only straightforward outputs in this batch, but
  they still require the normal VC7.1 behavior/parity promotion gates.

A standard post-batch refresh completed cleanly at 16:44 MDT after the queue released Ghidra. It
raised generated Wave/agent intake to **550 total / 542 checker PASS / 233 host C++20 syntax PASS**;
the VC7.1-ready subset is 35. The candidate signature audit is 483 PASS / 67 review. The curated
catalog and retail result are unchanged at **1,850 compile+behavior PASS**, with **914 exact + 609
relocation matches**, 199 differing, and 128 oracle-missing. Tooling SDK validation and the
ForgeFSE Quest slot audit remain clean. Canonical refresh state:
`f4b77e9eca489a121e9f1fdbe333f4a283f2c38557a1688a164eeeddf183c169`.

This refresh performs syntax/signature intake and dashboard regeneration; it does not promote any
wrapper above. The immediately preceding `SetFactionAsAlliedToFaction @ 0x00890870` result is also
retained only as structural review source.
