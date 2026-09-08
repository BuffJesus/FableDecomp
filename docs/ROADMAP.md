# FableTLC roadmap (single source of truth for "what's done / in flight / next")

Updated 2026-09-07. Checklists only. `[x]` = done with evidence, `[ ]` = open, "(verify)" = status
unconfirmed. Numbers come from `rebuild/COVERAGE.md` (dashboard generated 2026-08-28). Resume
instructions live in `docs/HANDOFF.md`; long history in `docs/journal/HANDOFF_ARCHIVE.md`.
Superseded inputs: `docs/journal/2026-09/{PLAN_pre-roadmap,BACKLOG_pre-roadmap,ACTIVE_TASK_LIST}.md`.

## Current focus (2026-09)

- [ ] Typed shared headers: 2026-09-07 `--all-trusted --compile-check` = 957 compiling headers (257 quarantined); automatic passes plus the parity-gated manual tail have retyped 596 landed TUs onto PDB-named headers (110 manual conversions across twenty-two checkpoints). Both `VTABLE_TYPED` (37→0) and `VIRTUAL_THIS` (54→0) are closed in the live `rebuild/backlog/retype_log.tsv`; the `AMBIG_ALIGN` lane is now under review, with two stale entries recovered in the first pass — `tools/decomp_pipeline/`
- [x] 2026-09-07 Aeon's 12 new Lua ports ingested (20 packages, 71 scripts; foundation audit 15/18) — `docs/scripts/AEON_LUA_PORTS.md`
- [ ] Port the 12 FSE 6.9.26 bindings LUAGameflow needs into ForgeFSE-retail-shadow; add package→native-name aliases so the new seeds correlate (8/20 today) — `docs/scripts/AEON_LUA_PORTS.md`
- [ ] Keep the parity crawl moving from `work/crawl_batch155` (ledger 5,693; catalog 18,631 landed) — `docs/pipeline/FULL_DECOMP.md`
- [ ] Live-test ForgeFSE-retail-shadow branch `feat/upstream-fse-2026-09-02` (commit `3f417ee`) in-game: CreateThread script + region unload — user-driven, needs Fable open
- [ ] Merge `feat/script-recovery-marathon` into `main` once CI (`docs-consistency`) is green on the branch

## (a) Repo health & public presentation — `CONTRIBUTING.md`, `docs/ARCHITECTURE.md`

- [x] 2026-09-07 `main` fast-forwarded to include 519 commits and pushed to `BuffJesus/FableDecomp`
- [x] 2026-09-07 22 agent branches archived as `archive/*` tags (24 tags total)
- [x] 2026-09-07 root scratch files + the `CON` file removed; `LICENSE`, `CONTRIBUTING.md`, CI (`docs-consistency`) added
- [x] 2026-09-07 docs restructured into `ARCHITECTURE.md`, `BUILDING.md`, `engine/`, `formats/`, `pipeline/`, `scripts/`, `journal/`, `modding/`
- [x] 2026-09-07 `docs/ROADMAP.md` + one-page `docs/HANDOFF.md` written (this file)
- [x] 2026-09-07 `docs_reorg.py --check-links --check-root` clean; `update_readme_progress.py --check` clean; both run in CI (`.github/workflows/docs-consistency.yml`)
- [x] 2026-09-07 `rebuild/COVERAGE.md` regenerated with genuine-C++ / asm-bake / readability rows; README rows generated from it
- [x] 2026-09-07 all 56 FINDINGS_LOG entries re-homed into reference docs (Verified facts sections; new `engine/CUTSCENES.md`); `--check-findings` now runs in CI
- [x] 2026-09-07 doc families merged: `formats/SAVE.md` (8→1), `formats/BIG.md` (3→1), `formats/MESH.md` (4→1), `engine/QUEST_CARDS.md` (5→1), `engine/NAVIGATION.md` (2→1); sources left as 3-line stubs
- [x] 2026-09-07 remaining families merged: `formats/{ANIM,TEXTURE,TEXT,AUDIO,DEFS}.md`, `engine/{HERO_APPEARANCE,INPUT,GAME_ENTRY_PATHS,QUEST_SCRIPTS}.md` (sources are 3-line stubs; INDEX lists both)
- [ ] Turn `rebuild/build_candidates.ps1` (130k lines of PowerShell objects) into a TSV + small driver
- [ ] Prune superseded intake drafts: 600/1,066 `rebuild/candidates/snapshots` and 604/1,079 `lift/reports/*/code` files have a landed reconstruction at their address (counted 2026-09-07). Needs the manifest/status generators to stop counting them first (`agent_candidates`, `agent_source_path`)
- [x] 2026-09-07 FQT + Fable-2 scene-import one-offs (21 scripts) moved to `tools/archive/` with a README
- [x] 2026-09-07 hidden scheduled tasks documented in `docs/BUILDING.md` (Background automation) with the disable command

## (b) Symbols & types — `docs/pipeline/SYMBOLS.md`, `docs/pipeline/NAMING_STRAGGLERS.md`

- [x] Ghidra 12.1 import + RTTI pass; PDB two-binary symbol port (FableWin.pdb / Ego_r.pdb donors)
- [x] BSim names applied; RTTI vtable-slot force port (`ghidra_out/labels_rtti_port.tsv`)
- [x] 100% mechanically named (49,568/49,568); 99.2% accepted naming quality; 40 names in review tail
- [x] FSE manifest corrected to 933 functions and propagated to FableForge
- [x] FSE address map (`refs/script_recovery/fse_address_name_map.json`, 80 addrs, 63 manifest clashes) wired into readiness analyzer
- [x] 2026-09-07 typed shared headers `rebuild/include/engine/`: 957 compiling headers from the PDB layouts, per-header compile check, quarantine on donor/retail conflict
- [x] 2026-09-07 automatic and manual parity-gated retyping applied to 596 landed TUs; `VTABLE_TYPED` and `VIRTUAL_THIS` tails closed (see Current focus for the active `AMBIG_ALIGN` review)
- [ ] Bulk `.gdt` import of donor struct layouts into the Ghidra DB (open)
- [ ] Calling convention known 77.7% -> close the 11,031 unknown-CC functions
- [ ] Complete prototypes 69.2% -> close the 15,280 incomplete ones
- [ ] Apply the 63 FSE-vs-BSim name clashes as manifest corrections (FSE ASLR address wins in `0x00CBxxxx`)

## (c) Byte-parity reconstruction — `docs/pipeline/FULL_DECOMP.md`, `docs/pipeline/DEBAKE_WORKLIST.md`

- [x] VC7.1 RTM 13.10.3077 harness; `verify_and_land.py` oracle->author->verify->land loop
- [x] Byte-identical 8,082 (16.3%); functional-or-matching 18,579 (37.5%) as of 2026-08-28
- [x] Manifest gap recovery: 7,529 xref-confirmed starts merged (functions.tsv 49,568 -> 57,097) — `docs/pipeline/MANIFEST_GAP_RECOVERY.md`
- [x] Over-capture trimming (`trim_overcapture.py`, `trim_tailjmp.py`) repo-wide
- [x] Jump-table lander `verify_land_jumptable.py` (raw-COFF extraction) proven on 0x557CA0 / 0x5578A0
- [x] Skeleton clone harvester + de-bake family tooling (`debake_family.py`, `harvest_all`, `crossfam`, `shape_author`)
- [x] Parity crawl batches 153 (8/8) and 154 (7/8) landed 2026-09-02; `probe.py` + `disasm.py` + `iat_lookup.py` helpers in `work/crawl_batch153-154`
- [ ] De-bake: 614 `__declspec(naked)`/`__asm` + 218 `_emit` non-genuine sources remain — regenerate with `crawl/bake_families.py` (verify counts)
- [ ] Mislabelled dtor families (`_Dest_val`/`_Cons_val` BSim labels on non-dtor bodies) re-attributed on landing; sweep the rest
- [ ] `00995e70 PeekData` honest DIFFER(16v15) — attack load-through-lea, not the compare
- [ ] `LinkToNeighbouringMaps 0x00A7A650` 9 bytes long; `CNavQuadTree::UpdateLines 0x00A781A0` fixture-first
- [ ] GFMain `0x00402510` dependency closure; CRT `entry 0x00401067` parity (deferred until GFMain boundary useful)
- [ ] Wave-3 review tails (CGSI wrappers with end-sentinel derefs, co-op spirit tail) rebuilt from bytes before promotion
- [ ] 84 deferred over-captures worklist `rebuild/backlog/overcapture-recovery-worklist.tsv` (~11 recovered)
- [ ] Lifted modules 3/3,433 -> next module-level link proof after OpenRetailBank ring (`LINK_WORKLIST.md`)

## (d) Running frontend / renderer parity — `docs/pipeline/VISUAL_PARITY_STATUS.md`, `docs/engine/GAME_ENTRY_PATHS.md#continue-and-load-game-path`

- [x] Stage-0/1 executable integration: VC7.1 links byte-identical fns into an owned Win32 process; `WinMain 0x403480` promoted
- [x] `FableTLC-Reconstruction-VisualCheckpoint.exe` renders the D3D9 frontend headlessly (synth-click + screenshot QA recipe)
- [x] Profile-name font doubling fixed 2026-08-10 (half-texel UV inset in `AppendProfileNameText`)
- [x] Save-enumeration seam opened 2026-08-10; 6 display-name-tier fns landed; find-loop blocker solved 2026-08-16
- [x] `CGame::Play 0x412f90` landed byte-exact; `CMainGameComponent::InitWorld 0x41735a` RELOC_MATCH
- [ ] Continue Game screen flash on press (D3D9 present/clear path) — known, unfixed
- [ ] `CWorld::LoadGameState` -> subsystem deserializers -> level stream (Continue Game core still unreconstructed)
- [ ] `lzo1x_decompress` for minimap decode is a hard defer (regalloc); keep as functional oracle
- [ ] Hero-in-world Phase A: wire `FableGFMainPhase10PlayBoundary` to real `CGame::Play` behind `FABLETLC_EXECUTE_EXACT_PLAY` after debugger live-oracle capture at `0x412f90` (verify: adapter ring still UNWIRED) — `docs/engine/HERO_IN_WORLD_ROADMAP.md`
- [ ] Phase B: streaming crash `0x7dd1d3` (`CEngineLandscapeMap::OpenStaticMap 0xBDD0E0` NULL malloc; BP `0xBDD1B2` reads `field_04`)
- [ ] Phases C-F (CWorld::PostInit ctors, tick orchestration, hero spawn, RenderCell/D3D9 device layer) — months; renderer is the cost driver

## (e) Engine subsystem RE — `docs/engine/SYSTEMS_ANALYSIS.md`, `docs/engine/CAPABILITY_INDEX.md`

- [x] Nav: quadtree line init/update, multilayer nav, navmesh RE — `engine/NAVIGATION.md` (merged from `NAVMESH_RE.md`, `MULTILAYER_NAV.md`)
- [x] Quest logic is compiled C++ (161-entry allocator table @ `0x00CD52D0`, no VM) — `QUEST_SCRIPTS.md#quest-binding-compiled-c-classes`, `QUEST_SCRIPTS.md#cutscene-and-region-script-interpreter`
- [x] Quest cards: display map, body art (3D mesh via `Graphic.modelId`), CUIDefs 8487-8489, empty-card fix — `engine/QUEST_CARDS.md` (merged from `QUEST_CARD_*.md`)
- [x] Input: Redefine Keys system, controller enums from FableWin.pdb — `INPUT.md#binding-store-and-redefine-subsystem`, `INPUT.md#controller-enum-tables`
- [x] Video system, minimap, hero morph, appearance strip, console commands, demon door face — per-doc
- [x] Terrain: whiteout + render fixes; ForgeTest64 terrain renders in-game (stage33); foliage group-sphere contract recovered (stage46)
- [x] Cut co-op multiplayer + EgoMP donor audits — `CUT_COOP_MULTIPLAYER.md`, `EGOMP_*.md`
- [ ] Nav: `CWorldMap::GetMapNavigationAreaInit` -> collision-hull -> blocking-line extraction
- [ ] Foliage stage46 runtime gate (custom collection-load probe + visible grass) — needs Fable closed then relaunched
- [ ] stage45 near-black terrain faces: compare authored foreground packed normals vs retail patch
- [ ] Save graph: `CTCInventory::Transfer`, `CQuestManager`/`CFactionManager` field/tag names
- [ ] Statue Master restoration `mods/StatueMasterRestoration/RUNTIME_PROBE.md` ten-case live probe (user-driven)
- [ ] Recover full 31-action `EGameAction` int->name table (3 sourcing passes failed; do not re-scan binary/web) — `docs/engine/INPUT.md#open-gaps`

## (f) Quest-script recovery / Lua — `docs/scripts/SCRIPT_RECOVERY_PIPELINE.md`, `docs/scripts/AEON_LUA_PORTS.md`

- [x] 161/161 retail scripts with complete 5-method lifecycle IR; 615/615 entity bindings; 1,708 interface calls resolved
- [x] Aeon's 8 hand-ports ingested (`work/aeon_lua_ports/`: DragonBossFight, GuardianSisterInfo(2), GuardianTrophyDealerInfo, HerosOldHouse, MazeResearch, MeetSister, StatueMaster) as `reconstructed-source` oracles
- [x] 2026-09-02 upstream FSE 14 bindings + `lua_newthread` threading ported onto canonical `D:\Code\ForgeFSE-retail-shadow` (1000-row table, corpus 111/111 callable); two ABI defects fixed (`FadeOutAndKillEntity`, cutscene arg order)
- [x] `map-native-helpers` stage shown to be 73% non-semantic (CRT/STL/lifetime) — not a real gate
- [x] `tools/script_recovery` tests 94/94; foundation audit 17/18
- [ ] Ingest the 2026-09-06 LUAGameflow batch (12 ports) into `work/aeon_lua_ports/`; extend alias table in both tools
- [ ] Ghidra anchor-discovery run for `V_StatueMaster` + `Q_DragonBossFight` (`decompile_native_script.py`) — the 18th audit check
- [ ] Runtime-confirm Aeon's addresses `0x00CBE716`, `0x00CBE87F`, `0x00CBE960` with a live probe
- [ ] First OUR end-to-end hand reconstruction of one `manual-lua-reconstruction` script against its trace fixtures (0 of 161 converted by us so far)
- [ ] Recompute the 69/92 stage split with non-semantic helper categories excluded
- [ ] Live world-entry smoke of the shadow package (previous menu-only smoke proved 0/16 script loads)
- [ ] `MsgOnLevelLoaded` ABI-blocked (retail writes `std::list<CCharString>`)

## (g) Modding toolchain

- [x] Lives in FableForge (`D:\Code\FableForge`) and ForgeFSE-retail-shadow now; moved docs indexed at `docs/modding/README.md` (`docs/from_fabletlc/` in each repo)

## Parked / shelved

- Gamepad redefine patch — shelved 2026-08-10; install reverted to clean retail; #811 clone renders black, row-value read path unknown — `docs/engine/INPUT.md#open-gaps`
- Mario rig (`work/mario_hero/stage_bindaxis4`) — body segments stretch apart; needs animation translation retargeting, not more bind-axis placement
- Unicorn emulation — assessed 2026-07-27 as optional local pilot; not implemented; not on parity totals — `docs/pipeline/UNICORN_EMULATION_PLAN.md`
- Co-op revival — RE audits done (`CUT_COOP_MULTIPLAYER.md`); implementation moved to FableForge docs; no active work
- Installer / settings UI / x64 / broad C++23 modernization — deferred until the reconstructed process reaches the game loop

## Definitions (grade ladder, lowest to highest)

- `candidate` — generated/reviewed source exists; not compile-verified (1,066 rows)
- `checker-pass` — passes the structural checker only; no compiler involved
- `compiled` — builds to a VC7.1 object; no parity claimed (18,604)
- `functional` — behavior fixture passes vs local oracle; bytes differ (`DIFFER`) — never counted in byte totals
- `matching` — `MATCH` (exact `.text`) or `RELOCATION_MATCH` (exact after masking COFF reloc slots) — the only "landed byte-exact" grades
- `asm_bake` — `__declspec(naked)` / `_emit` byte-bake; byte-exact but NOT genuine C++; purity debt tracked by `crawl/purity.py`
