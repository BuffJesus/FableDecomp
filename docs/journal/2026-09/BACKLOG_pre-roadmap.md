# FableTLC RE / decomp / modding BACKLOG (prioritized)

*Generated 2026-07-21 by a fan-out doc-sweep workflow (8 subsystem readers ->
synthesis): 149 gaps mined from the docs, deduped and ranked against the two
north stars: (1) full C++ decomp of Fable.exe, (2) injection-first "own the whole
pipeline" authoring. This is a living triage snapshot, not a spec.*

> **Staleness caveat (read first):** the readers mined both current and older doc
> sections, so a few conclusions lag today's state. Known corrections as of
> 2026-07-21: named coverage is **~83%+ and climbing** (two naming tranches applied
> this session, not "80.14%"); `BSimApplyNames` is **already run** (rank 7 is done —
> the live-loop continuation is what remains); the SAVE childhood-intro bug was
> **cracked** via the zero-shift `--exact-clen` fix ([[custom-content-session-state]]),
> so ranks 10/11 are narrower than stated. Trust HANDOFF.md tail + memory over any
> single ranked line here.

## Subsystem maturity
| Subsystem | Maturity | Open items |
|---|---|---|
| save-profile | **solid** | 11 (mostly live-test) |
| terrain-world | partial | 13 |
| defs-bin | partial | 13 |
| audio-dialogue | partial | 16 |
| mesh-tex-anim | partial | 19 |
| quest-script-gameplay | partial | 20 |
| decomp-infra | partial | 27 |
| ecosystem-integration | partial | 30 |

## The five biggest levers (from the synthesis)
1. **Install VC7.1 / MSVC 2003** — the ONE blocked prerequisite gating decomp bar #2
   (buildable source), the lift harness, and the chest/reward trial. User action only.
2. **One batched in-game live-test session** — ~35 BUILT-but-untested items collapse
   into a single game launch (save edit, Profile.bin slot, text.big, frontend.bin HUD,
   .lut/.lug audio replace+add, custom region/card/terrain). Near-zero engineering.
3. **Port `C3DMesh2::LoadBinary` geometry walk** — one port dedups 5 mesh/Blender gaps
   and completes the mesh round-trip; biggest authoring unlock.
4. **Port LZO1X-1 encoder (miniLZO)** for texture mip0 emit — last gate on texture
   authoring; pure coding, no RE.
5. **Run the game.bin def-container round-trip + write gate** — ships the native def
   writer; gates appearance/quest/card authoring.

## Ranked top-20
| # | kind | val/eff | item |
|---|---|---|---|
| 1 | inject | H/S | Install VC7.1 / MSVC 2003 (unblocks bar #2 + lift harness) |
| 2 | live-test | H/S | Batched in-game session for all built-but-untested writers |
| 3 | more-re | H/M | Port C3DMesh2::LoadBinary geometry walk; fill Blender decode_geometry() |
| 4 | add | H/M | Port LZO1X-1 encoder for texture mip0 emit |
| 5 | live-test | H/S | game.bin def-container round-trip + write gate |
| 6 | fix | H/S | Quest-card / new-def append rejection -> overwrite workaround, then RE count/name-resolution |
| 7 | decomp | H/S | **Continue** the labelling/coverage loop toward 100% pseudo-C (BSim already applied; naming tranches running) |
| 8 | add | H/S | FSE Lua bindings Tier A — 12 decomp-verified novel gameplay verbs (fishing/digging/chests) |
| 9 | add | H/S | FSE Lua bindings Tier B — 15 non-mutating state read-back queries |
| 10 | fix | H/M | save_edit childhood-intro (largely cracked via --exact-clen; verify length-changing edits) |
| 11 | fix | H/L | Hero-stat write path hold-on-reload (component-encoding RE) |
| 12 | decomp | H/L | CWorldMap::GetMapNavigationAreaInit -> collision-hull -> blocking-line extraction |
| 13 | decomp | H/M | Prove ForgeTest terrain white-out root cause (CreateNewBankMap/BuildThemes/bank-symbol reg) |
| 14 | decomp | H/M | Decompile texture-load GBANK decompressor to confirm LZ framing |
| 15 | decomp | M/M | CHeroMorph::Transfer / compositor (layer semantics, replace-vs-blend) |
| 16 | more-re | M/S | Recover EGameAction/EInputKey/EXboxControllerButton enum->name tables |
| 17 | more-re | M/M | Recover save-graph field/tag names (CTCInventory::Transfer, CQuestManager/CFactionManager) |
| 18 | inject | M/S | Rebase ForgeFSE bridge DLL onto real D:\Code\FSE_Source |
| 19 | add | H/M | FQT Milestone 1 — read-only Albion browser |
| 20 | add | H/M | Blender B2 add-on — custom mesh/anim authoring via EgoCore orchestration |

## By kind (149 deduped)
- **live-test (~38):** cheapest ROI — nearly all gated only on "no game launch this
  session," pure scheduling. Cluster into one session.
- **add-feature (~30):** the injection-first pipeline. LZO encoder (#4), FSE Tier A/B/C
  (47 verified methods), Blender B2, FQT Milestones, native def/frontend/script writer,
  lipsync-from-audio (the one genuinely large SP task). NOTE: anim writer, mesh-compose
  type-1/type-5, animation decoder are **SHIPPED/CLOSED** — not backlog.
- **more-re (~40):** mostly small Ghidra passes, many dedup-able (mesh LoadBinary walk = 5
  entries; controller enums = 5; save-graph names = ~6). Opaque-preserve items are
  round-trip-safe and deferrable.
- **decomp (~14):** north-star #1, mostly large/blocked. Full pseudo-C (running), navmesh
  collision->line, terrain white-out, GBANK decompressor, hero-morph compositor, STB baker
  from FableWin PDB, native script-VM/cutscene decomp (needs VC7.1).
- **inject (4):** VC7.1 install (blocks the bar-#2 chain), ForgeFSE rebase, mesh geometry
  writer (shipped). See docs/FORGE_INTEGRATION_PLAN.md (in FableForge) for the ForgeFSE/FQT
  generate-bindings-from-DB plan.
- **fix (~10):** root-cause clusters — def-append rejection (4 dups, has overwrite
  workaround), save reconstruction, silent swapped-audio (runtime gate).

*Full 149-item structured list: `ghidra_out/_gap_result.json` (allGaps).*
