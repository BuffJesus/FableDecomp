# Native recreation live status 21: frontend draw-path recovery

Native continuation now targets concrete frontend drawing dependencies identified in the visual-launch assessment. Existing shared-find acceptance still ends at 980. No Lua work, canonical header/index change, dashboard refresh, new GPU run or game launch occurred. Earlier journals and pending source packages remain preserved.

## UI sprite draw

Recovered ordinary native C++ for `NUISystem::CSprite::DrawWithRenderManager` at 0041A980, a genuine missing frontend provider. The UI namespace-qualified sprite is distinct from the generated global CSprite. Retail and Ego_r PDB/body evidence establishes dimensions, relative position/zoom, virtual centering slot 106, four coordinate conversions, graphics-bank ownership and the typed scaled-graphic renderer call.

The work-only candidate emits 373 bytes / 112 instructions versus retail 377 / 114, with 93.8053% instruction similarity and near-match status under fixed `/nologo /c /O2 /MT`. Its exact emitted definition is `?DrawWithRenderManager@CSprite@NUISystem@@QAEXXZ`. All twelve relocations were inspected: 2^32 and one-half constants, the four coordinate calls, CManager singleton, system getter, correctly typed renderer and C++ delete. The required-provider omission fails on that exact symbol.

Actual original machine code versus the retained compiled provider passes 65536 baseline cases / 131072 full 8192-byte arena snapshots, then 786432 cases / 1572864 snapshots across twelve x87 rounding/precision environments. Full callback arguments and arena bytes agree. Cases include dimension fallbacks/large unsigned widths, both placements, centering, callback mutations after cached geometry, late graphic-index loads, null ownership blocks, reference-count wrapping, draw callback changes and release/deletion ordering.

Stage: `work/parity_addtext/native_nui_sprite_draw_first_candidate_amended4_20261012`.

- Source manifest SHA: `593a89f6123bb7dd55a37b6dfa332e57149b03fc8e8880fcf22a74a61408f282`.
- Object SHA: `8b3dc14ab35f92a8f9de3ea4f7a09140eb14ed09b33ec9d3485f04dd935ea20a`.
- Compile result SHA: `8772550b41f52153ee115051f20c54469e6216a97d47021f15c2c47c2000617a`.

Scope: work-only partial non-owning field/virtual ABI views; no complete UI inheritance, construction/lifetime, actual singleton construction, scaled-graphic child execution or displayed frame claimed. The canonical headers remain unchanged.

Original failures preserved: initial prototype was corrected from pointer to the PDB's const graphics-bank reference before compilation; first compiler attempt rejected VC7.1's unsupported free-function `__thiscall` pointer. An ordinary virtual ABI call replaced it without changing algorithm/profile. An amended source manifest incorrectly included an empty SELF hash; the following author stopped on it. The original actual manifest hash already captured before compilation was used in a separate package, while the invalid empty SELF expectation remains historical data. No current canonical expectation was refreshed.

## Frame UV coordinates

The sprite's missing scaled-graphic child led to a second recovered dependency. Original 009FC810 is `CGraphicFrameData::GetUVBox() const`, confirmed by its actual frame receiver/caller and Ego_r donor 00B6C790. The current manifest's `CEngineInternalPrimitive2DStaticMesh::GetObjectTransform` label remains unchanged pending promotion; it is not used as identity evidence. PDB CUVBox is exactly four U floats followed by four V floats, 32 bytes. The original float constant is exactly `1.0f / 32767.0f`, verified against retail bytes and the donor's 32767 double.

First constructor-return candidate emitted 188 bytes / 54 instructions at 59.2593%, below threshold; it remains preserved. The second named-local return follows the original stack-local/final-copy structure, emitting 187 bytes / 52 instructions against retail 191 / 54, with 90.5660% similarity and near-match status on the unchanged profile. No further body fitting is required. Typed call target is the existing `CTexture::GetDimensions(unsigned long) const`; four constant relocations and that single call were checked. The exact `GetUVBox` provider omission passes.

Original/compiled comparison passes 491520 cases / 983040 full 1024-byte snapshots across twelve x87 environments, five return-buffer aliases and two placements. Getter callback mutation proves left/top caching before the call and width/height/TLX/TLY reloads afterward. Events, every arena byte and x87 status agree, including signed/zero returned dimension bits and extreme 16-bit fields. The fixture's initial incorrect four-argument VirtualFree declaration is preserved; the separate fixture amendment changes only that declaration, reusing the provider without recompilation.

Stage: `work/parity_addtext/native_graphic_frame_uv_named_return_amended2_20261012`.

- Source manifest SHA: `194b3fc89e7cfd1dc3c2bcebb9545f7a79aa92e1a994bd6ac2cdddbe46bc13e4`.
- Object SHA: `8caf956e06f4d8afcae609fd57ec53ba28463d7194d18ec95470650edf574296`.
- Compile result SHA: `48c68a504053c33e3dbfcbdf2536106679a02b60a23c1373f49232fa51445668`.

Scope: work-only frame method and UV value type, mocked dimension getter with exact typed ABI, raw backed field/return-buffer storage. No frame/texture lifetime, real GPU resource or bank loading claimed.

Combined evidence is `work/parity_addtext/visual_native_sprite_and_frame_uv_work_only_validated_20261012.json`. Both candidates are validated work-only recovery, not canonical coverage gains. Promotion still requires the frozen shared-header audit to close.

Next direct draw-path dependencies are `CRenderManager2D::Draw2DScaledGraphic` 009DD210, `Add2DTri` 009DC030 and `CGraphicInfo::GetFrame` 009FF8D0. Previously capped native `LoadGraphicFrame` 009FED30 remains unresolved; do not retry it or conceal it behind an authored production substitute.
