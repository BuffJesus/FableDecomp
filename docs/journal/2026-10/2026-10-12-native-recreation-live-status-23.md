# Native recreation live status 23: overnight handoff

User: “Let's call it for tonight … Update docs etc so we can proceed next session.” The persistent native-recreation goal is paused. No further implementation, native compilation, fixture execution or acceptance work occurred after that request. Documentation and read-only state verification only. All three existing helper agents report completed; there is no ongoing tool session from this final turn.

Primary next-session entry point: [NEXT_SESSION.md](../../NEXT_SESSION.md).

## Verified stopping state

Both completed drawing validation packets were revalidated against every captured expected hash at the start of the final continuation; zero drift:

- `work/parity_addtext/visual_native_sprite_and_frame_uv_work_only_validated_20261012.json`: SHA `a35715da9f1191ccc87127d029603c9a2c9586044cfd498e2614ab97358cdfe5`, 1770 controls. Sprite and UV providers remain work-only.
- `work/parity_addtext/native_render_vertex_overflow_WORK_ONLY_validated_20261012.json`: SHA `b80ac6aa66c0b6084551c0da3cd031768de3e54853abad43754b43af86bda72c`, 1704 controls. Complete relocation match, typed malloc/free, passing bounded fixture, original fixture exit failure preserved.

No Add2DTri source/candidate/output exists yet. Its original retail body was read in full. This is the exact implementation resume point, not an interrupted build.

Native retail image SHA remains `41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`, at `C:/Programs/Steam/steamapps/common/Fable The Lost Chapters/Fable.exe`, image base 00400000. VC7.1 is `D:/Tools/vc71`; Python is `C:/Users/Cornelio/AppData/Local/Programs/Python/Python314/python.exe`. Working directory `D:/Documents/FableTLC`; PowerShell commands use `login:false` and explicit UTF-8 reads.

## Add2DTri evidence already inspected

Original 009DC030 is 648 bytes, ending at 009DC2B5 with RET20. Ego_r donor 00B2FB50 and FableWin donor 030E52B0 agree on the exact private method:

`?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z`

Three const vertex references, a signed long blend mode and nullable texture pointer. Work-only declaration belongs in a cloned `fable_display_viewport.h`; do not change the canonical header while the shared-find audit is open. Its existing `CRenderManager2DTestAccess` friendship can expose a fixture entry without changing the original method's access/signature.

Observed original behavior:

1. Load `DisplayManager` from renderer +3A3C and read `ViewportOffscreen` at display +1D8. Any nonzero byte returns before reading vertices or invoking children. The canonical header corroborates this field name/offset.
2. For each vertex, in argument order, copy all 32 bytes into a local before callbacks. Call `GetCurrentAWindow` separately for X and Y. X becomes `2 * (X / (right-left)) - 1`; Y becomes `1 - 2 * (Y / (bottom-top))`. **No subtraction of window left/top from input X/Y.** Force Z to zero and RHW to one; retain the other copied fields.
3. Append each transformed vertex to `QuickDraw2DVertices` at +3E88. Use spare capacity when available; otherwise invoke the exact 009E14B0 specialization with current end, local value, unused false-type tag, fill length one and at-end true. Capacity/end are read after the Y getter. Later input vertices are copied only after previous append/growth callbacks finish.
4. Construct the local quick-draw record after all three vertex appends. Its wide-string and texture members use actual constructors. Store the passed blend mode; assign the nullable texture only when nonnull.
5. Call `GetCurrentAWindow` again and copy all four window floats into the record. Clear Line and IsText; append to `QuickDraw2DTriInfo` at +3E94 using actual copy construction or its actual overflow helper.
6. Uninitialise the local texture and destroy its wide string. Do not invent initialization for unused record fields or claim those uninitialized bytes as defined behavior; inspect the actual copy/consumer before designing the comparison.

Seven current-window calls are observable. Do not collapse/cache them: callbacks may change the window, source vertices, queue pointers or ownership between calls. Preserve the entry-only offscreen check and the late reads evident in retail.

Required children:

| Original address | Provider / state |
| --- | --- |
| 00A0AAA0 | CRenderManagerCore::GetCurrentAWindow, canonical source exists |
| 009E14B0 | Newly validated work-only vertex overflow, exact294B |
| 0099AED0 | CWideString constructor, canonical source exists; manifest module naming is historical |
| 009FA1C0 | CTexture assignment, canonical source exists |
| 009DE890 | CQuickDrawTriInfo copy constructor, canonical source exists |
| 009E1750 | Quick-draw info overflow, canonical source exists |
| 009F9F70 | CTexture uninitialise, canonical source exists |
| 0099B510 | CWideString destructor, canonical source exists |

The inline vector `push_back` body is in opt-in `fable_stlport_vector_impl.h`; avoid accidentally compiling alternate generic overflow bodies alongside retained concrete providers. Preserve the whole original relocation set and current canonical header hashes.

## Validation/promotion backlog retained

Canonical shared-find acceptance through 980 is the distinct saved-evidence recovery receipt, retaining qualification index 967 and false blanket no-local-metadata disclosure. Native 981–1030 is complete but unaccepted: 100 objects, 44 strict / six pending / six original name leaves, 54 fresh / 46 reused / 154 guards / six B proofs. Independent factual audit and root closure are already complete:

- Core `c6099f18bc952f190d6c76e45d72716bdc4b81d5be8f7cbe7cf7fd2a7ee502ae`.
- Seal `5722e94a1508a2c653599577cf9b0ab84ee86d0c78ae9feb5a547a9cfa47c244`.
- Root `collector981_root_FIRST_native_terminal_control_signoff_20261012.json`, SHA `4ec599ca67eda78ceccbfcf7e11744fc96ea8b55c6ea566ee426cda46fbdc88e`, 33126 controls.

The six exact FreePage definition-name cases are 995,996,997,998,1008,1009. Their amended2 SOURCE-only proposal remains unrun in `bound_regression_stage_20261012/collector981_exact_six_FreePage_name_SOURCE_ONLY_UNRUN_proposal_amended2_20261012`: manifest `63c0bb6e3e5fd609ac2a2b6501978e38ba05f9ea640e213b3ecfd63eb608e172`, proposal `fd6be35c64eab75b22a471cdb4635b83a8cc327d72865337b52f0670e862a593`, checker `1975fb8615fff8ffdcc70af735d5a04783b7997ea94f48869c056533fc8b5f3b`. No six-name diagnostic, concrete permission or acceptance operation occurred. No acceptance draft was authored after the preparation hold. Root/independent full source review and separate factual operation are still required. No generic anonymous-name/type/linkage/order waiver.

Exact1078 attribution package remains separate SOURCE-only design. Root source signoff SHA `545b9fd0f1c6b24338e35a4f5f0eaf1b488014fe2865d80a43186dc16f03d16f`; independent static core `7bde855a861110827beb4ed404048fb53def7850e3019a710817897a4a704eb7`, seal `5a5ca4edeb2f76d8a5966930f762b4aa769fb4689f2592ff3c2003dc69251363`. Independent factual checker has **not** run and root closure of the actual static controls is pending. Preserve original 36-path/42-role/12-triple expectations, original wrong scope array and unpinned logical-source DATA-only qualification. No collector edit or general include overlay is applied.

The 61 unchanged-context paired-byte screen remains a supplementary check, not the final independent unchanged-context terminal. Canonical promotion of the three new drawing providers must wait for the complete shared-header audit, including these qualifications.

## Failures and retry caps

Live statuses 21/22 enumerate every new source/fixture failure and its separate correction. Keep original packages, compiler/fixture results, deltas and object hashes. Passing providers should be reused; do not recompile them for byte fitting.

Existing capped GFInitialise, InitLUA, CloseStreams, LoadGraphicFrame, surface lock/copy failures, UID find, async Abort/FreeVirtual and font constructor/glyph/extents failures remain deferred. Static string extents AB7B00's material x87 mismatch remains a stop, not a near-match. No retry, asm bake, invented algorithm or authored production substitute is authorized by this handoff.

The prior journals and old documentation remain unchanged:

- HANDOFF.md SHA `6fe6df2011455d101c6b6107d9bffa37698a46ce4fc514924e537d7ca3583ea2`.
- ROADMAP.md SHA `a933c397e26902b961a730b31ca93179243848cc2727ba99605328cbeab4136b`.
- Live status21 SHA `45bf897c0abcd5297f548ef98295f646f65e54ebb9f5b8b0f157d2e6339f437`.
- Live status22 SHA `1e73c4648ca7760fb1b5cd213846ded1c04486c7100f0c47f72a243e685b96d3`.

No source promotion, dashboard refresh, commit/push, GPU/game launch or Lua work was performed. Next session should start with NEXT_SESSION.md and this handoff, not restart a completed producer or infer visual completion from the regression context count.
