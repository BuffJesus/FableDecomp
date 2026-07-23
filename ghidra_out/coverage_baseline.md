# Coverage baseline — Phase 0 (2026-07-17)

First real numbers, from `DumpStats.java` (post-import). This is the metric `docs/FULL_DECOMP.md`
tracks toward "fully decompiled".

## Two databases in project `ghidra_proj/FableTLC`

| Program | Total fns | Named | Default `FUN_*` | Thunks | Total symbols | Source |
|---|---|---|---|---|---|---|
| **`Fable.exe`** (shipping TLC, blind) | 44,268 | **1,730 (3.9%)** | 42,538 | 193 | 120,764 | Ghidra auto-analysis only (no symbols; retail, no PDB) |
| **`ego_r.exe`** (debug, +`Ego_r.pdb`) | 65,565 | **28,057 (42.8%)** | 37,508 | 551 | 215,509 | PDB private symbols applied |

## Reading it
- The shipping `Fable.exe` is ~96% nameless (`FUN_*`) — the blind starting point. Its 1,730 names are
  just imports/exports + what Ghidra's RTTI/heuristics recovered alone.
- The symbolized `ego_r.exe` gives **28,057 real function names** + 215k symbols (classes, structs,
  members, globals, statics) — the donor set for the symbol port.
- Function-count gap (65,565 vs 44,268): the PDB gives ego_r exact function boundaries → Ghidra splits
  many small/inlined helpers the stripped `Fable.exe` heuristics miss. The true `Fable.exe` count is
  higher than 44,268; closing that gap (recovering unrecognised functions) is part of Phase-4 bar #1.

## The payoff (in progress)
Port ego_r's 28,057 names onto `Fable.exe`'s 42,538 nameless functions via **Ghidra BSim** (decompiler
signature similarity) — same VC7.1 compiler + shared engine → high match rate expected. Track the
post-port `named` count here as the next coverage row.

## ★★★ RESULT (2026-07-17): BSim port SUCCEEDED — 3.9% → 78.4% named, demangled

| Program / stage | Total fns | Named | % named | Note |
|---|---|---|---|---|
| `Fable.exe` BEFORE | 44,268 | 1,730 | 3.9% | blind |
| after BSim port @ sim≥0.7 | 44,268 | 26,753 | 60.4% | +24,923 real names |
| after BSim port @ sim≥0.5 | 44,268 | 34,714 | 78.4% | +7,961 more (0.5–0.7 band, precision verified) |
| **after Demangle** | 44,268 | **34,714** | **78.4%** | 32,689 names → readable `Class::method`, 0 failures; +class namespaces & signatures (symbols 120,764→195,822) |

Verified clean in the DB: `CWorld::ProcessEvent`, `CWorld::DrawGetEnvironment`, `CThing::GetPBaseDef`,
`CScriptThing::operator==`, `CCombatAbilityData::TransferBinaryIn`; 468 `CCombat*`, 144 `CWorld::`,
89 `CThing::`, 79 `CQuestManager::` functions. Audits: `bsim_port_audit_final.tsv` (0.7),
`bsim_port_audit_05final.tsv` (0.5). Note: a few near-duplicate functions share a ported name (BSim
maps structurally-identical instantiations together) — cosmetic, not wrong.

Ported names are correct, high-similarity (0.9–1.0) Lionhead engine functions — e.g. `CWorld::ProcessEvent`,
`CWorld::DrawGetEnvironment`, `CThing::GetPBaseDef`, `CThingSearchTools::GetMapwhoArea`,
`CScriptThing::operator==`, `CActionDoCreatureAction::GetActionName`. Audit: `bsim_port_audit_final.tsv`.

### The pipeline that worked (the crux was fixing the donor's names first)
1. Ghidra's PDB **Universal analyzer applied types but NOT function symbols** (donor had ~0 game names —
   only EH funclets). Root cause of the first failed port.
2. Extracted the real symbols from `Ego_r.pdb` with Ghidra's **`pdb.exe`** (needed **DIA SDK
   `msdia140.dll` registered**, admin) → 1.08 GB XML → `parse_pdb_xml.py` → 58,727 fns / 43,373 addrs
   (RVA+0x400000). `llvm-pdbutil` failed ("too many directory blocks"); DIA/pdb.exe was the path.
3. `ApplyNames.java` → **36,400** real names onto `ego_r` (6,541 PDB fns Ghidra hadn't defined = fail;
   recoverable later).
4. Regenerate donor BSim sigs (now carry 36,278 names) → commit to H2 DB.
5. `BSimApplyNames.java` on `Fable.exe` @ sim≥0.7 → **24,923 names ported**. `results_only_unnamed_donor`
   dropped 32,933 → 49.

### Next to push coverage higher
- Lower BSim threshold to 0.6/0.5 (more recall; check audit precision) — 26,556 fns matched ≥0.7,
  more sit in 0.5–0.7.
- Recover the 6,541 PDB functions Ghidra didn't auto-define (create functions at those addrs, re-apply).
- Add `FableWin.exe`+`FableWin.pdb` (254 MB, richest) as a 2nd donor.
- Run Ghidra Demangler on `Fable.exe` to turn `?Foo@CBar@@` into readable `CBar::Foo`.

## Approaches tried (2026-07-17 autonomous session)
- **String fingerprint port** (`ExportFuncFingerprints.java` + `match_fingerprints.py`): REJECTED.
  Only 10 of the donor's 7,271 string-referencing functions have PDB names — the PDB names game-logic
  classes, not the string-heavy templated container code. Naming ceiling ≈ 10. Documented dead end.
- **Source-file module map** (`MapSourceFiles.java`): only 119 `Fable.exe` / 19 `ego_r` functions
  reference a `__FILE__` string — too sparse to drive naming; minor module-mapping value only.
- **BSim** (`docs/BSIM_PORT.md`): the correct path. DB created; donor (`ego_r`) signature generation
  **launched in background** (decompiling ~65k functions, multi-hour). Apply-names script
  `BSimApplyNames.java` staged. → complete per `docs/BSIM_PORT.md`, then record the new `named` count.

## Also available to raise coverage
- `FableWin.exe`+`FableWin.pdb` (254 MB PDB — richest) and `Ego_d.exe`+`Ego_d.pdb` (debug) as
  additional donor DBs.
- `Data\Defs\DevHeaders\*.h` (dev struct headers) → parse into both DBs.
- FSE manifest (`fse_api.h`, 931 fns) + EgoCore source (asset-format answer key) for the gameplay/
  resource subsystems.

## RESULT (2026-07-18): FableWin donor pass complete - 80.13% named

`FableWin.exe` + `FableWin.pdb` was imported, analyzed, named, committed to a fresh BSim DB with the
upgraded `ego_r` donor, and ported back onto retail `Fable.exe`.

| Program / stage | Total fns | Named | % named | Note |
|---|---:|---:|---:|---|
| after ego_r pass + demangle | 44,268 | 34,714 | 78.4% | previous endpoint |
| after FableWin BSim @ sim>=0.7 | 44,268 | 34,902 | 78.8% | +188, zero failures |
| after FableWin BSim @ sim>=0.5 + demangle | 44,268 | **35,472** | **80.13%** | +570 more; `DemangleAll`: 715 demangled, 0 failures |

FableWin apply details:

- `ApplyNames.java`: 157,042 FableWin PDB names applied.
- `CreateMissingFunctions.java`: 38 missing FableWin functions created/named.
- `bsim_port_audit_fabletlc2_07.tsv`: 188 applied, 0 failed.
- `bsim_port_audit_fabletlc2_05.tsv`: 570 applied, 0 failed.
- `stats_fable_after_fablewin.log`: 35,472 named, 8,796 default-named, 197,375 total symbols.
- Full generated report: `ghidra_out/fablewin_pipeline_report.md`.

## RESULT (2026-07-18): Conservative FSE labels applied - 80.14% named

After the FableWin pass, `LabelApplyDefaultOnly.java` applied conservative FSE-derived function labels
from `ghidra_out/labels_fse_addresses.tsv`.

| Program / stage | Total fns | Named | % named | Note |
|---|---:|---:|---:|---|
| after FableWin BSim + demangle | 44,268 | 35,472 | 80.13% | BSim donor endpoint |
| after conservative FSE labels | 44,269 | **35,479** | **80.14%** | +7 renamed, +1 function created, 31 existing names preserved |

FSE label details:

- `LabelApplyDefaultOnly.java`: rows=39, renamed=7, commented=38, created=1, skippedNamed=31, failed=1.
- Stats log: `ghidra_out/stats_fable_after_fse_labels.log`.

## 2026-07-19 - Bar #1 clean-decompile sweep (DecompCoverage.java)

Full sweep of retail `Fable.exe`, per-function results in `ghidra_out/coverage.tsv`:

- **49,082 functions: 49,080 decompile cleanly (99.996%)**, 427 thunks.
- 2 failures: `RunCutsceneMacro_Func` @ `0x00CBFB7D` (74,778-byte giant, 60s timeout;
  retried with 600s - see below) and `Main` @ `0x00DECAD0` (Pcode decoding error
  "Attribute size is not present" - needs instruction-level investigation).
- Naming state: 40,183 named (81.9%), 8,899 default-named. The unnamed tail is small
  leaf code: it averages 219 bytes/function and totals 1.9 MB of the 14.8 MB `.text`.
- Conclusion: **bar #1 (full pseudo-C coverage) is effectively closed on the decompile
  axis**; remaining bar #1 work is naming/typing quality, not decompilability.

### Straggler resolution (2026-07-19)

- `RunCutsceneMacro_Func` @ `0x00CBFB7D`: **FIXED** - decompiles in 186s with
  `DecompileOptions.setMaxPayloadMBytes(512)` (default payload cap was the failure, not a
  timeout). Output is 1,020,610 chars of pseudo-C. Sweep tooling should carry the raised cap.
- `Main` @ `0x00DECAD0`: still fails ("Pcode: Decoding error: Attribute size is not present").
  All 438 instructions decode individually (`FindBadPcode.java`) and resetting
  signature/custom storage (`FixDecompMeta.java`) does not help - the bad metadata is likely
  in a callee prototype or referenced symbol the decompiler streams in. Single known-bad
  function: **49,081 / 49,082 = 99.998% clean**.

### Main@0x00DECAD0 investigation (2026-07-19)

- `FindBadPcode.java`: all 438 instructions decode individually - not an instruction problem.
- `FixDecompMeta.java`: resetting signature/custom storage did NOT fix it.
- `DumpCallees.java`: the 10 direct callees all have sane prototypes (no zero/negative-length
  datatypes). Callees include `CTCVillage::OnInitialActivate`, `CSubtitleRenderer::SetText`,
  `CMeleeApprentice::Init`, `CSpawnedFunc<CExpression_FollowScript>` ctor, and STL
  `_Cons_val`/`CCharString` helpers.
- Conclusion: the "Attribute size is not present" fault is in a deeper referenced data type
  (a struct field/component streamed in transitively), not a direct callee prototype. Left as
  the single known-bad function; not worth deeper chase at 99.998% clean. Scripts committed for
  future use: `DumpCallees.java`, `FindBadPcode.java`, `FixDecompMeta.java`, `RetryDecomp.java`.
