# Modernization owner journal, 2026-10-28

Continues [2026-10-27](2026-10-27-modernization.md) (152-171 accepted, nothing committed). Native coordination today is with
the native lane session `fabletlc-7b` (0xA20000 range crawl). At session start native was confirmed unfrozen; the 171
freeze had been released at 20:44 on 10-27.

## Bundle 172 (in progress): level theme composition, GetLevelTheme 008884d0 + BlendInLevel 008886e0

### Native oracles (workflow wf_0a48518d-cfe: two capture lanes, then two refuters each)
- **GetLevelTheme** `work/modern_level_theme_20261028/` (validation2 + audit2; FPCW 0x027f).
  - Genuine 0x202-byte body plus 11 genuine STLport/theme helper spans: reserve 889cb0/724e30, ctor 8864a0,
    resize 88a520, fill insert 88a180, copy ctor 8894a0, fill_n 889800, uninitialized_copy 889700, fill-assign 8897d0,
    dtor 6b5d80, destroy range 6b67a0.
  - Recorded boundaries: GetBlendedTheme 00888260 (ret 8 with args out and time, `this` = set element) and
    SetAsWeightedBlendOfThemes 008873c0. malloc/free/memmove 00bfea0e/00bfea14/00bfeae6 are MSVCR71 IAT thunks and are emulated.
  - 2,048 cases, 10/10 controls rejected by output difference; the audit matches every case.
  - Capture asserts zero hits on the branches that cannot run here, and the audit checks that coverage independently.
- Behaviour, GetLevelTheme(out, time, sets):
  - this = the priority-stack entry (Themes CArray at +8/+0xc, 12-byte CThemeWeight).
  - **One theme:** set[id].GetBlendedTheme(out, time). The weight is ignored, so NaN, 0 or a negative weight still gives the full theme.
  - **Otherwise** (including 0): reserve exactly n themes and weights. Per theme:
    1. append a default-constructed theme;
    2. GetBlendedTheme fills it;
    3. push the raw weight bits.
  - Then out.SetAsWeightedBlendOfThemes(themes, weights), called even with empty vectors. Free order: weights, theme buffers (set 2 down to 0), themes.
  - Quirk: ctor 8864a0 leaves TimeOfDay uninitialised; GetBlendedTheme overwrites it.
- **BlendInLevel** `work/modern_blend_in_level_20261028/` (validation2 + audit): genuine 0x2d7-byte body plus 25 helper
  spans, including operator= 6b5df0 and the vector<long>/<float> assigns. 2,048 cases, 10/10 controls rejected by output difference; the audit matches every case.
- Behaviour, BlendInLevel(out, source, time, sets):
  - **Copy:** BaseWeight > 0.9990234375 (0x3f7fc000) gives out = source (operator=; nothing is copied when out is source).
  - **Single:** BaseWeight < 2^-10 strictly with exactly one theme calls GetBlendedTheme only. The source and the weight are ignored.
  - **Weighted:** everything else.
    - base = BaseWeight > 2^-10. When set, the source is copied in first with weight BaseWeight, then the themes as in GetLevelTheme.
    - Then SetAsWeightedBlendOfThemes. It is safe for out == source (CalculateTheme passes out as both).
  - NaN takes neither gate and goes to weighted with no base.
  - The exact reserves mean no growth path runs in retail's domain. The reserve controls reach the grow helpers.
- **Review:** 4 refuters found only two documentation gaps in GetLevelTheme, both fixed: the unlisted fill-insert tail
  branch, and that count 0 returns at the reserve capacity check. They also noted the allocator thunks are malloc/free, not new/delete.

### Portable owner (workflow wf_801c3769-718, running)
- Lane `work/modern_owned_level_theme_20261028/`: LevelTheme.hpp. Themes stay a host-owned std::vector<ThemeWeight>.
  The host binds GetBlendedTheme and SetAsWeightedBlendOfThemes.
- Private qualification finished before the 16:55 session crash: strict x86/x64/gcc final2 pass (exact marker,
  inputs unchanged), mutations_final2 positive plus 13/13 controls rejected, export 4,096 cases (`export.json`).
  An earlier v1 attempt (weight-unlocked export, x64 shadowed-parameter warning) is kept under `attempts/`.

### Bundle `work/modern_owned_level_theme_bundle_20261028/` (derive_from_171.py, prefix `lv_` after `wb_`)
- Promoted locally (promotion.json; the three production hashes were re-checked after the crash and match).
- prepare_matrix ran. The crash interrupted preflight, so its output folder was empty; preflight was re-run after the restart.
- The empty crashed preflight folder was moved to `preflight_interrupted_crash_1654/`. The rerun PASSED:
  `preflight/summary.json` (10,011 inputs, 1,732 snapshot inputs). There is no native drift since the 170 cut, so 172 records no cut.
- Native session `fabletlc-4e` (the native lane, resumed after its own crash) granted "native frozen" at about 18:05. It reported writes
  only under work/native-recreation/wf-a2/** and its journal since 171.
- **Stopped at 18:14 at the user's request** (end of night): wait_matrix was stopped mid-run with no result, and
  its partial output was moved to `validation_matrix_stopped_user_1814/` (only x86.log/x64.log). "native unfrozen" was sent.
  172 is promoted but NOT accepted.

## Session stop (2026-10-28 18:14)
- Accepted: 152-171. 172 is promoted locally, preflighted, and its matrix has not run.
- Resume:
  1. Re-check native drift by rerunning preflight. Move `preflight/` aside first, because the script uses mkdir(exist_ok=False).
  2. If native headers changed, record a 172 cut (prepare_source_cut -> review_notes.json -> qualify_source_cut, then rerun prepare_matrix).
  3. Request a freeze from the native session, then run `wait_matrix.py` (exit 2 = lease held), and send "native unfrozen" when it ends.
- After 172: 006b2ca0 (x87 trig evidence policy), then compose the 161-172 owners into one CEnvironment owner.
