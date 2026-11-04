# Functional-first reconstruction: near-match verification

User policy, 2026-09-26: genuine C++ recovered from the engine algorithm, identical
behavior demonstrated by fixtures, and byte-close VC7.1 output counts as reconstruction.
Exact MATCH remains preferred but optional. Stop tuning once near_match and behavior
PASS are reached unless the remaining difference exposes a behavior error. Assembly
bakes, invented algorithms and fitted constants remain excluded. No modernization fork
work was started.

## Implementation

`tools/decomp_pipeline/near_match.py` disassembles x86-32 with Capstone. It compares
ordered normalized instruction tokens with SequenceMatcher (autojunk disabled).
Thresholds are inclusive: instruction-count deviation <=10% of retail and similarity
>=90%. Tokens preserve mnemonics, operand kinds/widths, constants, non-stack member
offsets, addressing scale and segment overrides. They mask register allocation,
stack displacements, direct control-flow destinations and relocation operands.
Retail pointer immediates use the PE base-relocation table; absolute memory addresses
are treated as linker-selected globals/imports. Missing retail PE data conservatively
leaves pointer immediates unmasked. Incomplete or empty decode cannot qualify.

This is a proximity score, not a behavior proof. Masking registers, addresses and
stack offsets can hide wrong dataflow or callees; fixtures must check outputs, side
effects and call ordering. Functions containing embedded data/jump tables require
an explicit code-boundary model before near-match grading (the raw-COFF extractor
continues to support exact grades only). Thresholds have not been loosened after
this first sweep.

`verify_and_land.py` now returns NEAR_MATCH, saves per-function similarity JSON,
stops its spelling/flag search, and requires behavior PASS before landing with
`Grade = 'near_match'`. A test that links without the candidate cannot qualify for
this new grade. Purity checks still include shared headers. A legacy longest-symbol
fallback cannot receive NEAR_MATCH when the requested name was not found. Exact
and relocation-match behavior remains compatible. The verifier also restores the
reported base candidate before behavior testing if all flag variants miss; previously
it could test the last rejected variant instead.

## Reclassification sweep

Run from the repository root:

```powershell
python tools/decomp_pipeline/reclassify_near_matches.py
python tools/write_decomp_dashboard.py
python tools/update_readme_progress.py
```

The sweep recompiles current source with catalog flags/toolsets, then runs a
candidate-required fixture. `rebuild/compile-gate/near-match.json` is an independent
classification overlay; the historic exact-byte report is preserved because its
objects may differ from the newly rebuilt source. The dashboard reads the overlay,
checks source/test hashes, excludes later exact promotions, and counts near matches
separately from exact-byte totals. This does not refresh every historical coverage
metric or claim a full fresh compile of the catalog.

43 existing DIFFER rows audited: 31 candidate-linked behavior passes, five rejected
as non-genuine, five test compile failures, and two self-contained fixtures. One
qualifies as NEAR_MATCH: 00404280 WideString_EqualsLen, 27 retail / 29 built
instructions (+7.41%), similarity 96.43%, behavior PASS. An inverted length comparison
fails its fixture. No further byte tuning is needed for this function.

00989760 UpdateAmbient and 009DF060 CStateBlockFunctionSold::Apply also meet numeric
thresholds, but their old fixtures include source via relative paths and fail in the
verifier's scratch directory. They remain unverified; migrate those fixtures to shared
declarations and candidate linkage before promotion. The other DIFFER rows remain
outside the measured margin or lack sufficient fixture/purity evidence.

Validation: 17 targeted tests cover thresholds, normalization, constants/member offsets,
condition-code differences, ordering/width, incomplete decode, assembly rejection,
behavior failures, early stopping, grade emission, stale/duplicate dashboard records,
and public total consistency. README regeneration passes --check.

Full gates PASS: 123/123 boot leaves, component ABI, 460 header dependents with
zero regressions, zero missing bootstrap references, zero unresolved ring symbols,
and the full visual bootstrap build. Log: work/codex-near-match/gates.log.

Resume native execution at CProfileManager construction 009F9160. Apply this relaxed
standard to existing Play scaffolds and new startup dependencies; do not resume
register-allocation byte chasing after near-match plus behavior PASS.
