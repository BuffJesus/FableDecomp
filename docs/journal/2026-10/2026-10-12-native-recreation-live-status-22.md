# Native recreation live status 22: genuine triangle-queue growth

Continued the frontend drawing dependency chain after the validated sprite and UV work in live status 21. The original `CRenderManager2D::Add2DTri` 009DC030 calls 009E14B0 at each of its three vertex-vector overflow branches. Original 32-byte records, three owner pointers, five stack arguments and RET20 identify the same STLport overflow specialization pattern already recovered for other concrete value types.

The first work-only `vector<CTVertexRHWColSpecTex1>::_M_insert_overflow` candidate emits exactly 294 bytes / 111 instructions, matching the complete original body after adjusting only its two call relocations. Whole raw COFF inspection confirms typed targets `_malloc` at offset 57 and `_free` at offset 261, corresponding to original CRT thunks 00BFEA0E and 00BFEA14. These are malloc/free, not array-new/delete; early driver metadata's generic array-allocation wording is superseded by this explicit target audit. No body/profile tuning or provider recompilation occurred.

The bounded fixture compares actual retail and compiled code across 19520 cases / 39040 full 8192-byte snapshots, including empty/nonempty vectors, every insertion position up to 16 existing records, fill counts 0/1/2/4, both tail flags, external versus old-buffer value aliases, two owner placements and allocator/free callback mutation. Every event and arena byte agrees; independent allocation-byte and final owner-pointer invariants pass. The exact decorated provider omission also fails as required.

The first fixture printed comparison PASS but exited with access violation `0xC0000005` (3221225477). That failure remains preserved and is not counted as a successful terminal run. A separate fixture amendment scopes allocator mocks to the two tested call boundaries instead of replacing process-wide CRT malloc/free. It executes a 294-byte copy of the actual linked compiled provider with only those two call displacements patched, alongside the similarly bound retail body. The original provider object is reused unchanged and remains required by the link. The amended fixture exits zero. No engine allocation algorithm or production source was changed to accommodate the fixture.

Stage: `work/parity_addtext/native_render_vertex_overflow_FIRST_20261012`.

Closed evidence: `work/parity_addtext/native_render_vertex_overflow_WORK_ONLY_validated_20261012.json`.

Scope remains work-only recovery, not canonical coverage. The canonical headers, symbol index, grades and dashboards remain frozen. No Lua, real GPU test or native game/frontend launch occurred. Actual CRT behavior and live ownership/lifetime are fixture boundaries; the comparison establishes the original bounded overflow behavior and complete compiler-emitted code/typed relocation match.

The next direct parent is `CRenderManager2D::Add2DTri` 009DC030. Its other children are already recovered: current-window getters, wide-string construction/destruction, texture assignment/uninitialization, quick-draw record copying and quick-draw info overflow. `Draw2DScaledGraphic` and the frame-loading branch remain subsequent dependencies; capped `LoadGraphicFrame` work remains excluded from retries. Shared-find acceptance still ends at 980, and promotion of these new providers waits for that header audit to close.
