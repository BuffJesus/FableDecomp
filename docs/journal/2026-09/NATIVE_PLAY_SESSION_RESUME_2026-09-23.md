# Standalone engine: next-session resume

Saved at the user's bedtime checkpoint, 2026-09-23. Development is paused here.

## Start in the correct checkout

```powershell
Set-Location D:\Documents\FableTLC-native-play
git branch --show-current
```

Expected branch: `wip/native-cgame-play`. The default environment directory
`D:\Documents\FableTLC` is the separate quest-recovery checkout. Continue the
standalone engine in **native-play**, not the Wasp Menace quest/cutscene work.
The user subsequently requested commit, push and review. The accumulated standalone
checkpoint is being committed on this branch and pushed to origin; use `git log -1`
and the upstream status to identify the saved revision. Preserve any later local
changes. No game or GUI was launched during checkpoint review.

User direction: continue substantial reconstruction work autonomously; reasonable
behavioral equivalence is acceptable. Byte-identical code is not required.

Read [the latest evidence journal](FRONTEND_BANK_ENTRIES_2026-09-23.md) first.
The older continuation blocks in HANDOFF are historical; the newest block wins.

## Where we stopped

The readable **009CFBC0 entry decoder** now connects actual bank setup, word/string/
runtime vectors, tree cleanup, alias lists, archive strings and slow stream reads.
It correctly returns bool. **0049B760 resizes symbols; it does not merely reserve.**

Latest new gates: storage helpers **3,328 PASS**, aliases **3,072 PASS**, decoder
**512 PASS**. Affected regressions: setup **1,024 PASS**, opening **2,048 PASS**,
manager construction **324 PASS**. Twenty-one saved passing reports total **26,793
cases**; this is a saved-report aggregate, not a full-suite rerun. Implementations
are functional DIFFER, with explicit allocator/service boundaries. No full
bootstrap or live-game validation was run.

The opening gate still substitutes a sample-read decoder. The new entry gate
exercises the real decoder separately; do not mistake either for complete disk
loading or playable standalone gameplay.

## Concrete next work

1. Recover **009CE050** entry index registration. Inspect its dependencies:
   **0042D131** symbol-map insertion, **004014A0** CRC (ECX=0, EDX=text,
   stack=length), **009B6300** CRC-pair vector growth, **009CBE10** filename
   processing, **0099ED10** string normalization and **009D34E0** filename-map
   insertion. Verify behavior from retail; propagated names are not authoritative.
2. Recover finalization **009B85A0** CRC sorting, **009B7B10** compaction,
   **009CD740** runtime packing, and bank virtual callbacks **+0x28/+0x30/+0x3C**.
   Replace corresponding explicit doubles in the entry gate as bodies land.
3. Connect the real decoder into `UiBankOpen_test.cpp` / `check_ui_bank_open.py`.
   Replace its synthetic sample payload with valid serialized entries and initialize
   all bank vectors/trees before real preparation runs. Do not invoke it on the
   old fixture's poisoned storage. Verify the complete open-to-decoder chain.
4. Continue real threaded file opening **0098E1E0**, production bindings, frontend
   Run/actions and rendering. Existing D3D CreateDevice DEVICELOST remains open.

## Files and local evidence

- `rebuild/src/compiled/00/9c/CBankFile_ReadEntries_009cfbc0.cpp`
- `rebuild/include/fable_ui_bank_entries.h`, `fable_ui_bank_aliases.h`,
  `fable_ui_bank_storage.h`, `fable_ui_bank_decode.h`
- `rebuild/tests/integration/UiBankEntries_test.cpp`, `UiBankAliases_test.cpp`,
  `UiBankStorageHelpers_test.cpp`, `UiBankStorage_test.cpp`
- `tools/decomp_pipeline/check_ui_bank_entries.py`, `check_ui_bank_aliases.py`,
  `check_ui_bank_storage_helpers.py`, `check_ui_bank_storage.py`
- Reports/disassembly under ignored local `work/ui_bank_*_check/` directories.
  Full retail entry disassembly: `work/ui_bank_storage_check/entry-decoder-retail.txt`.
- New gates are wired into `rebuild/build_bootstrap.ps1`; new function identities
  and artifact paths are recorded in correction/manifest/index TSV files.

Targeted commands, when changes justify rerunning:

```powershell
python tools/decomp_pipeline/check_ui_bank_entries.py
python tools/decomp_pipeline/check_ui_bank_aliases.py
python tools/decomp_pipeline/check_ui_bank_storage_helpers.py
python tools/decomp_pipeline/check_ui_bank_storage.py
python tools/decomp_pipeline/check_ui_bank_open.py
python tools/decomp_pipeline/check_ui_manager_construction.py
```

The gates compile with the existing VC7.1 toolchain and emulate the installed
retail executable using Unicorn. Retail SHA-256:
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

## Details to preserve

- Word resize wrappers are 22 bytes each **plus a shared 231-byte helper**. Do not
  report the wrappers alone as complete implementation sizes.
- Alias allocation cookies determine destruction count; the public count is a
  signed byte. Copy has no self-assignment guard; callers guard where needed.
- Entry update records copy real blob bytes. At retail 009D01BE, an outstanding
  pushed allocator argument shifts the stack offsets: they refer to End/Begin,
  not Capacity/End. Do not reconstruct a spurious descriptor-copy bug.
- Entry callbacks receive the full type word; runtime storage retains its low byte.
  Stream position is restored after callbacks. FileValid is set after finalization.
- Malformed payloads, invalid indices and crashing allocation paths are outside
  decoder coverage. Current fixtures control index/finalization and virtual services.
- Production registry state at 013CA79C and mode at 013CA7B0 overlap and must use
  consistent backing storage. Production exception handling remains a boundary.
