# BSim name-port runbook (ego_r → Fable.exe)

Runbook for porting PDB-backed donor function names onto the stripped shipping `Fable.exe` using
Ghidra **BSim** (decompiler-signature similarity). The first completed pass used `ego_r.exe`; the
current expansion adds `FableWin.exe`, the richer editor/debug donor.

## Why BSim (and not the string/instruction shortcuts)
- **String fingerprint** (`ExportFuncFingerprints.java`): tried and rejected. Only **10** of the donor's
  7,271 string-referencing functions carry PDB names — the PDB names the *game-logic* classes, while
  string-heavy code is templated containers (`PKCTR*`) the PDB leaves unnamed. Ceiling ≈ 10. Dead end
  for naming. (Repurposed for module mapping — see `MapSourceFiles.java`, ~119 hits, modest.)
- **Source-file map**: only 119 `Fable.exe` functions reference a `__FILE__` string — too sparse to
  drive naming.
- **BSim** matches on the decompiler's normalized p-code signature, so it works on *all* functions
  regardless of strings — the right tool. Both binaries are the **same VC7.1 compiler on the same Ego
  engine**, so match rates should be high.

## Cost (be honest)
BSim requires **decompiling every function** in both programs to build signatures:
`ego_r` ~65k fns + `Fable.exe` ~44k fns ≈ 109k decompilations = **hours**. Unavoidable. Run headless,
in the background.

## Completed ego_r pass

Correction to stale notes below: the ego_r donor signature generation and `BSimApplyNames.java`
runs are complete and proven on Ghidra 12.1. The first donor raised `Fable.exe` from 3.9% named to
78.4% named after the 0.7 and 0.5 BSim passes plus demangling.
1. **DB created** ✓ — `bsim createdatabase file:/D:/Documents/FableTLC/bsim/fabletlc medium_nosize`
   (`medium_nosize` ignores address/register-size differences — right for cross-build).
2. **Generate donor signatures** [RUNNING as of 2026-07-17, bg task] →
   `bsim generatesigs "ghidra:/D:/Documents/FableTLC/ghidra_proj/FableTLC?/ego_r.exe" bsim/sigs_egor --config medium_nosize`
   (local project URL form verified to work). Log: `ghidra_out/bsim_gensigs_egor.log`.
3. **Commit donor sigs to the DB:**
   `bsim commitsigs file:/D:/Documents/FableTLC/bsim/fabletlc D:/Documents/FableTLC/bsim/sigs_egor`
   Verify: `bsim listexes file:/.../fabletlc` shows ego_r.exe; `bsim getexecount` > 0.
4. **Apply names onto Fable.exe** via the staged script (it generates Fable.exe sigs itself, queries
   the DB, applies matches ≥ threshold):
   ```
   analyzeHeadless ghidra_proj FableTLC -process Fable.exe -noanalysis \
     -postScript BSimApplyNames.java file:/D:/Documents/FableTLC/bsim/fabletlc 0.7 0.0 \
     -scriptPath tools/ghidra_scripts
   ```
   - `BSimApplyNames.java` is **staged but untested** (couldn't compile-check while the donor decompile
     held the project). First run: watch for compile/API errors; likely fix points are
     `db.getLSHVectorFactory()`, `FunctionDescription.getFunctionName()`, and `SimilarityResult`
     iteration — adjust to the installed Ghidra 12.1 BSim API if they differ.
   - Start at similarity **0.7** (conservative, high precision). Review `ghidra_out/bsim_port_audit.tsv`.
     Lower toward 0.5 for more recall once precision is confirmed. Names are applied with a
     `[bsim sim=… <- ego_r]` comment so they're auditable/reversible.
5. **Re-measure coverage:** rerun `DumpStats.java` on Fable.exe → update `ghidra_out/coverage_baseline.md`.
   Expected jump from ~4% named toward tens of percent.

## Alternative / augment
- Add `FableWin.exe`+`FableWin.pdb` (254 MB PDB, richest) as a second donor: import it, generatesigs,
  commit to the same DB — more names to match against.
- For functions BSim misses, fall back to the FSE-target, string-xref, and Lua-natives passes (Phase 1
  in `docs/PLAN.md`), plus manual work on high-value subsystems.

## Current second-donor pipeline (2026-07-18)

Status lives in `docs/HANDOFF.md`. One-command status:

```
powershell -ExecutionPolicy Bypass -File tools/StatusFablePipeline.ps1
```

The active automation is `tools/ContinueFableWinPipeline.ps1`. It waits for the long `FableWin.exe`
import to save, then runs:

1. `ApplyNames.java` on `FableWin.exe` using `ghidra_out/fablewin_pdb_names.tsv`.
2. `CreateMissingFunctions.java` on `FableWin.exe`, then `ApplyNames.java` again.
3. `bsim generatesigs` + `commitsigs` for upgraded `ego_r.exe` into `bsim/fabletlc2`.
4. `bsim generatesigs` + `commitsigs` for `FableWin.exe` into `bsim/fabletlc2`.
5. `BSimApplyNames.java` on `Fable.exe` at similarity 0.7, then 0.5.
6. `DemangleAll.java`, then `DumpStats.java`.

Preflight results:
- `bsim/fabletlc2.mv.db` exists and is empty (`getexecount` = 0).
- `ghidra_out/fablewin_pdb_names.tsv` has 164,501 rows, 164,501 unique addresses, 0 bad addresses,
  and 2,775 names containing whitespace. The apply scripts sanitize whitespace before `setName`.
- Driver log: `ghidra_out/continue_fablewin_pipeline.log`.

Useful lock-free report helpers:

```
powershell -ExecutionPolicy Bypass -File tools/SummarizeBsimAudit.ps1 -Path ghidra_out/bsim_port_audit_fabletlc2_05.tsv
powershell -ExecutionPolicy Bypass -File tools/SummarizeStatsLog.ps1 -Path ghidra_out/stats_fable_after_fablewin.log
powershell -ExecutionPolicy Bypass -File tools/AuditNameTsv.ps1 -Path ghidra_out/fablewin_pdb_names.tsv
powershell -ExecutionPolicy Bypass -File tools/WriteFablePipelineReport.ps1
powershell -ExecutionPolicy Bypass -File tools/SummarizeFableWinEditorSymbols.ps1
powershell -ExecutionPolicy Bypass -File tools/WriteEditorToolingNotes.ps1
```
