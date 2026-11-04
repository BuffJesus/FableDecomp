# Native recreation continuation - 2026-10-06

The user requested continuing recreation, excluding Lua, until asked to stop.
Resumed the native renderer dependencies from the October 5 handoff. The real
visually verified recreation frame remains outstanding.

## Native shader enables and ownership

Integrated the native CShaderRenderManager interface for EnableVertexShaders
(00987FE0,61 bytes), DisableVertexShaders (009880E0,44), EnablePixelShaders
(00988110,36), and DisablePixelShaders (00988190,85). Recovered the native
CDisplayManager::GetPixelShaderVersion (009BEE00,29) and the PC shipping no-op
CRenderManagerCore::EnableXBoxShaderConstants (00A0AA20,3). All six genuine
VC7.1 bodies match retail exactly, including resolved relocation identities.
Four replace local View-class definitions; the no-op replaces a misleading
generic E2 provider identity. The capability query was incorrectly catalogued
as texture-memory calculation. PDB methods and retail callsites corroborate
the repaired identities.

The shared shader header now exposes confirmed retail fields at 1D0,1D4,1D8,
1E4,390..392 and retains size3DC. CPixelShaderData is shared and typed; the
CurrentPixelShader subobject's unrecovered construction remains opaque to
avoid adding implicit static initialization to the singleton. Pixel shutdown
clears the device binding before clearing enable/update state, rereads the
current data after the device callback, releases its reference and clears the
slot after the deleting-destructor callback. Keeping the inline uninitialise
helper reproduces the original instruction ordering; flattening it produced
an unaccepted77-byte/87.5% candidate; it was not promoted.

Permanent gate: tools/decomp_pipeline/gate_native_shader_enable.py, registered
in gate_all. Passes252 cases/1656 complete24KiB snapshots and six provider
omission checks. Includes192 enable/disable sequences and60 pixel shutdown
cases: capability version byte boundaries, high version bits, existing enable
bytes, repeated transitions, null/live/wrapping reference counts, COM failures,
and pointer/flag/refcount mutation during COM and deleting-destructor calls.
Enable helpers execute real native callees. Pixel COM and deletion are recorded
boundaries; no GPU rasterization or real shader-data destruction is claimed.

Native render frame regression180 cases and text queue852/3444 snapshots pass
after shared header changes. A fresh native closure build completed at
work/codex-native-startup/native-gfmain-shader-enable-clean-20261006, seeded with
the previous1329 sources plus these six providers:1335 sources,zero duplicates,
414 unresolved symbols. These explicit shader seeds are ready for the renderer
parents; this step does not reduce the frontier before those parents land.

Next: integrate native shader constant layout/state setup and complete renderer
ClearRenderTarget, DrawBufferedPolys and constructor. Their previously exported
evidence remains in work/parity_addtext/renderer_next_decomp.c. No renderer
parent has been promoted by this continuation so far. Preserve the unrelated
dirty workspace and independent Lua work; no commit or retail launch made.

## Shader constant uploads, binding and world reset

Integrated seven native upload/reset methods: SetVSConstants0098B930 (37bytes
exact), UpdatePixelShader00988A20 (41exact), UpdateAmbient00989760 (85bytes,
91.667% near match), UpdateFogColour009897C0 (99exact), UpdateLightGlobals0098A760
(54exact), UpdatePixelShaderConstants00989BF0 (186exact) and
ResetWorldTransform00988290 (131bytes,95.833% near match). Stop byte tuning the
two accepted near matches. Pixel uploads retain the PDB-named static
C4DVector ps_constants[8], with exact emitted C4DVector constructor (3bytes)
and compiler vector-constructor iterator (48bytes) separately audited.

ResetWorldTransform is genuine C++, replacing the previous naked assembly
definition. The original was preserved byte-for-byte under
rebuild/src/asm_bake/00/98/CShaderRenderManager_ResetWorldTransform_00988290.cpp;
the active asm-bake backlog entry was removed. The native method has its proper
class symbol, rather than the old free fastcall oracle symbol. The new shared
CMatrix4x4 union matches PDB field names and Data[4][4]. Its containing object's
alignment/lifetime is not inferred from this partial value interface.

Permanent upload/reset gate passes128 sequences/3120 full12KiB snapshots and
seven omissions. It compares upload payloads, zero/nonzero ranges, pixel
constant index reordering and static initialization, pointer/flag/colour/matrix
changes during callbacks, repeated resets, ignored HRESULT failures, four x87
rounding modes, signed zeros, infinities and quiet NaNs. Device COM calls remain
recorded boundaries; there is no GPU rendering claim.

ApplyVertexShader00988020 is exact176bytes and uses the shared native shader,
vertex-data and render-state interfaces. Its hardware-mode check reads
SoftwareVertexProcessing.CurrentState (+2908), not DesiredState. The PDB's
CurrentVertexShaderVertexDeclairation spelling is retained. The permanent
binding gate passes384 cases/1632 full24KiB snapshots with a required-provider
omission: both force values, all software/fixed modes, cache hits/misses/null
shader handle, repeated binds, and callbacks changing device, cached handles,
input shader data and hardware state. The second COM call rereads the changed
device and declaration; ignored HRESULT behavior matches retail.

Current accepted total for this continuation:14 native bodies,12exact and two
near matches,1067retail bytes. These are local task diagnostics, not dashboard
coverage. Three shader gates pass (252/1656,128/3120,384/1632); native frame180,
frontend Draw512 and text queue852/3444 regressions pass after shared headers.
Fresh combined build launched at
work/codex-native-startup/native-gfmain-shader-pipeline-clean-20261006. Its seed
contains the prior1335 sources plus the eight additional native providers.
The clean build completed:1343 sources,zero duplicates,414 unresolved symbols.
All fourteen providers compile together. The renderer parents remain absent;
no reduction of the frontier or visible frame is claimed.

Remaining immediate renderer dependencies: native shader layout registration
and selection, combined projection update, DrawAsIndexedTris, state capture
restoration, integer viewport endpoint, and then complete renderer parent bodies.
No ClearRenderTarget/DrawBufferedPolys/constructor parent or visible frame has
been promoted. Historical legacy leaf-test files and audit reports are not the
new evidence; dedicated native gates and registered manifest providers are.

## Layout containers, selection, capture restoration and indexed drawing

Registered six layout-container addresses across four sources: RegisterVertexShaderConstantLayout0098B230 (73bytes exact), UnregisterVertexShaderConstantLayouts0098B280 (51exact), pointer-vector resize0098D870 (87exact), fill insert0098D000 (276retail/286built,97.297% near), overflow insert0098CDE0 (216exact), and fastcall fill0098C120 (31exact). Unregister was incorrectly modeled as a list. Retail resets index to-1, resizes its pointer vector to1, stores the embedded null layout and selects it. The generic fill declaration now supports a per-translation-unit calling-convention macro; default behavior of other instantiations is unchanged. The permanent layout-vector gate passes444 cases/1072 full24KiB snapshots and four omissions. Actual production vector operations run, including growth, insertion, aliasing and cleanup; allocator calls use a deterministic fixture arena and production/test object parity is checked.

SetVertexShaderConstantLayout0098B5E0 covers the complete633-byte retail body (the old624-byte span omitted nine bytes),97.714% near match. Its real const range inequality00987D70 is33bytes exact. Selection gate passes1620 cases/1874 full24KiB snapshots and three omissions, with the actual range comparison and CBaseClass destructor. Light allocation, vector clear and vector resize remain recorded boundaries. Cases cover all ranges, index/default selection, light sizes/capacities, no-shrink behavior and callback mutations. Shared range/layout interfaces come from PDB and retail offsets; unrecovered container/subobject lifetimes stay opaque. Both accepted near matches are final without further byte tuning.

The common parity checker now recognizes audited fully relocated byte equality before rejecting a normalized score when the fixed retail image lacks a vtable-literal relocation. The7-byte CBaseClass destructor is exact after relocation, although incomplete retail relocation metadata previously scored it50%. This does not accept an unaudited relocation or a nonmatching body.

Native RestoreCaptureBlock00A05840 (121bytes) and DrawAsIndexedTris00A0AF40 (58bytes) both match exactly. Restore uses shared CRenderStateManager state/stack members; indexed drawing uses the native core and actual RealiseRenderState provider. Permanent gate passes324 cases/3720 full24KiB snapshots and three omissions: two nested capture restores followed by realization and drawing, zero/eight saved states, existing dirty entries, equal hardware values, all state kinds, high bookmark bits, COM errors and callbacks mutating desired states and both device pointers. COM is recorded; no rasterization claim.

All24 addresses accepted this continuation:20 exact/four near matches. New gates are in gate_all and manifest/artifact/correction/symbol registrations are updated. Legacy leaf catalogs were retired for these providers. Layout-vector444 and render-frame180 regressions pass after shared-header changes.

Fresh build work/codex-native-startup/native-gfmain-capture-layout-clean-20261006 completed with1351 sources,zero duplicates,417 unresolved. The extra three unresolved symbols are now exposed by layout selection (AllocateLights, GFClear4DVectors and vector<C4DVector>::resize), not a claimed frontier reduction. Prior1343-source build remains historical. Full renderer parents and a visually verified native frame remain outstanding.

## Native C4DVector containers and composed selection

Eight more exact bodies,884retail bytes, are registered across three sources: resize0098DA90 (68), erase0098CF70 (81), fill insert0098D360 (285), overflow0098D240 (207), uninitialized copy0098BF70 (62), uninitialized fill0098BFB0 (58), backward copy0098C240 (68) and fill0098C140 (55). Class-element insertion uses the actual non-POD STLport paths, not the earlier pointer vector's trivial-copy operations. Existing shared algorithms produce the exact helpers. The range eraser calls the shared counted __copy loop; a manually flattened loop failed similarity and was not used.

Permanent gate_native_vector4.py passes516 cases/950 full24KiB snapshots and three source omissions. Tests cover erase ranges, resize shrink/growth, insertion begin/middle/end, alias values, spare capacity and at-end overflow. malloc/free are deterministic arena boundaries, with renamed replay objects checked against production code and relocations. All eight complete bodies match after audited relocation.

A second composition gate, gate_native_shader_layout_vector_selection.py, runs the actual new vector providers under native shader layout selection. It passes1620 cases/1782 snapshots and six source omissions, including capacities forcing actual allocation. Light allocation and vector clearing remain recorded. The previous boundary-focused selection gate still passes1620/1874. Both gates are registered in gate_all.

Fresh build native-gfmain-vector4-clean-20261006 completed:1354 sources,zero duplicates,416 unresolved. Closure automatically found the three vector sources and removed the resize frontier entry. The continuation now has32 accepted addresses,28exact/four near matches. No full renderer parent or visible frame yet.

Rejected scratch probes: GFClear4DVectors00A5C300 is89retail bytes; intrinsic candidate95bytes/87.671% similarity fails. Its fastcall scalar fill_n00A5C510 is54bytes exact, but not separately promoted or behavior-gated. Combined projection00988A50 full span978bytes; simply lifting the old View candidate to native matrix fields produces1048bytes/30.213% similarity and is not accepted. Candidates and their temporary header live only in work/parity_addtext (native_Clear4Vectors.cpp, native_CombinedProjection.cpp, native_projection_candidate.h); temporary native header declarations were removed again. The actual shared shader layout stayed unchanged. SIMD instruction scheduling needs evidence-based reconstruction, not further polishing of accepted methods.

Next evidence export: simple_memory_pool_decomp.c contains constructor, FreeAll, Allocate and capacity/accounting leaves. Ego_r confirms CSimpleMemoryPool size1C, CBaseClassNonCopyable base, FirstMemoryBlock+4, Capacity+8, GrowthGranularity+C and Root(CArea)+10. This supports the remaining native AllocateLights dependency.

## Simple memory pool and native light allocation

Four pool bodies accepted: FreeAll00A5C720 (101bytes exact), Allocate00A5C910 (134retail/136built,93.333% near), SetCapacity00A5C5C0 (19exact), CalculateMemoryAllocatedByPool00A5C5E0 (29exact). Shared fable_simple_memory_pool.h models real CBaseClassNonCopyable and nested CArea/CMemoryBlock types; PDB confirms all fields/status enum values. It replaces the two old local/generated-layout leaf providers. Alignment uses original division/multiplication form. The allocation near match is final, not a byte-tuning target.

gate_native_simple_pool.py passes249 cases/866 full24KiB snapshots and four omissions. Empty/nonempty free lists, exact/split fits, first/later areas, repeated FreeAll, owned/borrowed chains, wrapping allocation sizes/capacity/accounting and callback mutations are covered. Block growth/destruction are boundaries in this focused gate.

Four further exact block-lifetime bodies share one source: CMemoryBlock constructor00A5C670 (53), destructor00A5C6E0 (32), compiler scalar deleting destructor00A5C9B0 (52), CreateNewMemoryBlock00A5C890 (121). gate_native_simple_pool_lifetime.py runs actual block creation/lifetimes beneath actual pool allocation and FreeAll:240 cases/6216 full24KiB snapshots, five omissions. Only operator new/delete and aligned heap APIs are recorded. Known vtable addresses are normalized; repeated growth, single-block retention, owned/borrowed destruction and flags0/1 are checked. Clean explicit pool build completed1359 sources/zero duplicates/416 unresolved.

Native CShaderRenderManager::AllocateLights0098B420 is446retail/456built bytes,97.544% near. Its actual vector<CLightInfo> constructor0098CCC0 (119bytes) and counted-copy helper0098C2F0 (complete72bytes, including its second return path) are exact. Typed CShaderGenericLight has PDB parameters/types/attenuation/size fields and only initializes Size. CLightInfo initializes enable=false/update=true. Shared shader storage now names LightAttenuations at1A0 without inventing its lifetime.

gate_native_shader_lights.py passes672 cases/3920 full48KiB snapshots and seven omissions. It composes actual pool/block allocation and attenuation-vector erase/growth; heap APIs alone are recorded, with allocator-renamed object parity audited. Existing light records are compared completely. Only newly default-constructed records mask their unspecified first48 bytes and final two padding bytes; Size and both flags remain checked. Input Lights reside outside the initially empty pool, so pointer publication establishes when default-record masks apply. Covers zero/count shrink/growth, retained capacities, pool splitting/new blocks, attenuation shrink/growth, checked multiplication overflow and 32-bit product wrap. No claim of meaningful values in uninitialized parameters.

Current accepted continuation total43 addresses:37exact/six near. Latest clean build native-gfmain-shader-lights-clean-20261006:1360 sources,zero duplicates,415 unresolved. AllocateLights is resolved. Shader upload/reset128/3120 and actual-vector selection1620/1782 regressions pass after light-header changes. Clear4D remains the layout-selection frontier. Bounded SIMD clear compiler/loop variants still fail (best87.671%); they remain scratch. No ClearRenderTarget, DrawBufferedPolys or renderer constructor parent has been promoted, and no visible frame is claimed.


## Complete native clear parent and viewport draft

ClearRenderTarget009D8CF0 is now registered as genuine C++: full3385-byte retail body versus3363 compiler bytes,815/821 instructions,94.8655% normalized similarity. The initial draft failed similarity. Ordinary structured branches and a reference to the capture-stack size recover the compiler layout; the passing body is final and should not be tuned further. Correct state selections are StencilMask, TextureArgument1[0] and TextureAlphaArgument1[0]. The renderer vertex format and shader constant/preset storage now have shared typed interfaces without inventing unrecovered lifetimes.

Permanent gate_native_clear_render_target.py passes4096 native-process cases and34304 snapshots,883122176 bytes compared with retail execution. Both clear paths, flags0..15, all shader-enable pairs, pending upload flags, identity reset, empty/nonempty preset arrays, clean/already-dirty states and callback mutations are covered. Every engine call and return compares the entire24KiB fixture arena plus the988-byte shader manager. Viewport boxes, four vertices and six indices are captured by value. Only four uninitialized specular-colour words are masked. The parent provider omission fails linking. Engine dependencies are recorded boundaries in this gate; previously accepted leaf gates remain separate evidence. No GPU rendering is claimed.

The fixture uses a data-only COFF object containing absolute external symbols to map shader/table globals at their retail addresses; it contains no executable code. The production C++ object is unmodified. The gate audits all72 production relocations and removes its large input/output replay files after success. Earlier scratch emulation independently passed the same4096 cases. No retail code is linked into the native fixture.

Three additional lookup entries (01396E14=8/D3DCMP_ALWAYS,01396E74=2/D3DTOP_SELECTARG1,01396F40=3/D3DSTENCILOP_REPLACE) are included in the existing static data provider. The data gate checks all26 symbols/140 bytes against retail and SDK names and passes486 linked default-state scenarios. This adds data, not three function claims.

Integer viewport009BEF80 now has a genuine C++ scratch candidate at work/parity_addtext/native_IntegerViewport.cpp. Native-process behavior matches384 cases/2563 full4KiB snapshots, including inclusive-edge overflow, degenerate bounds, device failure, repeated calls, callback mutation and aliased input. Current456-byte candidate has73.05% similarity and remains unregistered. Its479-byte assembly original is still only an oracle. D3D viewport storage is typed at1DC, and the shared COM declaration now reaches slot47. The float viewport remains unrecovered.

After header changes, shader uploads128/3120, actual-vector layout selection1620/1782, shader light allocation672/3920 and capture/draw324/3720 regressions passed. A fresh interface-only build was1360 sources/zero duplicates/415 unresolved. After accepting the clear parent and refreshing the lookup data index, the latest clean build is work/codex-native-startup/native-gfmain-clear-parent-clean-20261006:1361 sources,zero duplicates,416 unresolved. ClearRenderTarget is resolved; its previously hidden global2D shader layout and combined-projection method are now exposed. No frontier reduction is claimed.

Accepted continuation total44 addresses:37exact/seven near. DrawBufferedPolys009D9C80 and renderer constructor009D9A30 remain open. The global2D shader layout has only a declaration; its initializer/lifetime must be recovered, not replaced with a zero-filled stand-in. The visually verified native frame remains outstanding. No commit was created.


## Layout lifetimes and global initialization in progress

Four further exact bodies are registered in CVertexShaderConstantLayout_Lifetime_0098D490.cpp: constructor0098D490603 bytes, compiler-generated destructor0098D710270 bytes, scalar deleting destructor0098D99030 bytes, and range scalar deleting destructor0098C39030 bytes. gate_native_shader_layout_lifetime.py passes312 cases/884 full4KiB snapshots with two provider omissions. Actual CBaseClass ctor/dtor run beneath the layout and all26 ranges; free/delete are boundaries, known vtable addresses alone are normalized. Empty/nonempty preset buffers, flags0/1 and mutating free callbacks are covered. The initial destructor probe wrongly measured through the first ret in the next function; the real destructor ends with a tail jump at0098D819. Implicit destructors are required: explicit empty bodies introduce vptr stores absent from retail. Accepted continuation total48 addresses:41exact/seven near. Latest clean closure remains the preceding1361/416 build pending global integration.

The shared layout now names all PDB ranges through ScreenSpaceTransformOffset and owns the actual CArrayVector<CVertexShaderConstant> at140. Both layout-selection gates and the clear-parent gate passed after this lifetime change. Then the global declaration was refined to its actual derived type, CVertexShaderConstantLayoutEmpty, confirmed by RTTI at0129B9B0 and CRT initializer01224140. This final derived-type header change still needs its clear-gate rerun and a fresh closure build.

Global initialization work is NOT registered yet: Global_2DShaderConstantLayout_01224140.cpp contains the real static derived object and Empty::PeekName; the base PeekName source was corrected from the legacy wrong literal/signature to the shared virtual method returning an empty string. Scratch LayoutGlobal.obj emits the expected32-byte _$E1 initializer,10-byte _$E2 atexit thunk,25-byte Empty::PeekName and30-byte Empty scalar deleting destructor. Their parity/behavior gate and registration remain to finish. Retail targets: initializer01224140 calls base ctor0098D490, writes derived vtable0129B9B0, registers0122A820 with atexit004012BC; atexit thunk tail-jumps base dtor0098D710. Empty::PeekName009E16B0 passes literal LayoutEmpty at0129B9B8 to CCharString(char*,long)0099EBF0; base PeekName0098D6F0 passes the empty string at0122D70E. Empty deleting destructor009E1850 is30 bytes. Do not substitute a zero-filled global or change the accepted clear algorithm. Evidence: layout2d_xrefs.log and layout2d_lifetime_decomp.c. Native global lifetime/PeekName fixture is the next action.


## Global shader layout completed

The global initializer/lifetime work above is now registered and verified. Five more exact bodies: CRT initializer01224140 (32bytes), atexit thunk0122A820 (10), Empty::PeekName009E16B0 (25), Empty scalar deleting destructor009E1850 (30), and base PeekName0098D6F0 (25). The base name returns an empty string; the previous local-class provider incorrectly returned PeekName and used the wrong constructor/method ABI. Both now use the shared virtual class and real CCharString(char*,long) signature.

Global_2DShaderConstantLayout_01224140.cpp defines an actual CVertexShaderConstantLayoutEmpty object. Compiler-generated CRT initialization calls the actual base constructor and registers the actual implicit destruction thunk; no manually zero-filled substitute is used. gate_native_shader_layout_global.py compares nine complete332-byte global snapshots with retail, verifies real process startup and CRT atexit teardown, invokes both virtual name slots and tests derived scalar deletion with flags0/1. Four omitted providers each fail linking. CCharString construction and heap deallocation remain recorded boundaries. Vtable addresses are normalized at known polymorphic fields. The compiler CRT slot, derived vtable bindings and zero-initialized BSS allocation are audited. BSS must be read from its COFF flags/size rather than the section's absent raw-data pointer.

Clear-parent4096/34304, layout lifetime312/884 and actual-vector selection1620/1782 regressions pass with the final derived-type header. Clean closure work/codex-native-startup/native-gfmain-layout-global-clean-20261006 finishes1364 sources,zero duplicates,415 unresolved. The global layout dependency is resolved. Accepted continuation total53 addresses:46exact/seven near. No visually verified native frame or commit.

Next renderer evidence: Ego_r CRenderManager2D fields identify retail QuickDrawVertices+3E58, QuickDrawTriInfo+3E64, QuickDrawAlphaVertices+3E70, QuickDrawAlphaTriInfo+3E7C, QuickDraw2DVertices+3E88 and existing QuickDraw2DTriInfo+3E94 (12-byte STLport vectors, unlike donor16-byte vectors). VertexShaderBank occupies3D20..3E48 (CShaderDataBank), counted VertexShaderResource3E48, VertexShader2D3E50. Current shared header leaves the unrecovered members opaque. DrawBufferedPolys native body and renderer constructor remain next major parents; constructor/body decompiles are already in renderer_next_decomp.c. SIMD clear, combined projection and viewport similarity still need work.


## Buffered-draw helpers and texture identity correction

Four buffered-draw prerequisites are registered: depth comparators009D8C30 (32bytes) and009D8CB0 (58), plus CRenderManagerCore::SetAWindow float00A0AA80 and integer00A0AA90 (11each). All are exact. Depth callbacks pass4800 native cases/9600 comparisons across all four x87 rounding modes and24/53/64-bit precision, including signed zeros, infinities, quiet/signaling NaNs, denormals and extreme/adjacent finite values. Equal and unordered comparisons intentionally return1, not0; a <= ternary was incorrect for unordered inputs. These are depth comparisons, not the legacy Y-position labels. Window overloads are PDB-identified and pass512 receiver/reference forwarding cases with a required-provider omission; endpoint behavior remains a boundary.

Further viewport probes remain rejected. Correctly using boost::scoped_ptr safe-bool (rather than get()) yields an integer candidate with74.20% similarity, still passing384 cases/2563 snapshots. Reference-clamp and early-store variants were discarded; native_IntegerViewport.cpp retains the safe-bool candidate. A float viewport cast probe using /QIfist emitted64-bit fistp stores, whereas retail uses32-bit fistp; it is not a faithful overflow/NaN replacement and was not promoted. No compiler flags were changed in the production pipeline.

A real closure identity error was found: 00CAA6E0/00CACD60 had been labeled CTexture::Uninitialise, but those bodies only Release the pointer and leave the object unchanged. Their old tests redefine the tested implementation. The engine method009F9F70 additionally clears the pointer and ownership nibble. The previous closure selected00CAA6E0 under the genuine engine symbol. Both wrong identities were retired from active catalogs/index, their source/tests and original manifest rows preserved under rebuild/backlog/mislabels/<address>, and their naming/proof fields reset pending original identity recovery. They are not substituted into engine calls anymore.

Three typed engine bodies are now registered with PDB signatures/shared headers: CTexture::Uninitialise009F9F70 (34bytes), const CTexture& CTexture::operator=(const CTexture&)009FA1C0 (98), and CRenderManagerCore::AttachTextureToStage009A0CF0 (79). They replace legacy View-class exports. gate_native_texture_bindings.py passes384 cases/3360 full20KiB snapshots with three omissions, composing assignment, repeated cached attachment, destination cleanup, null rebinding and source cleanup. COM AddRef/Release/SetTexture are boundaries; self assignment, null/live pointers, cache hits, stages0/1/3/7, stage counts, callback field mutations and ignored HRESULT failures are covered. Do not replace self assignment with an invented early-out.

Fresh closure work/codex-native-startup/native-gfmain-texture-bindings-clean-20261006 compiles1364 sources,zero duplicates,415 unresolved. It selects00/9f/CTexture_Uninitialise_009f9f70.cpp and contains neither retired00/ca source. The clean seed was copied from the previous closure with the one bad selected provider removed. AddText852/3444, render-frame180 and clear-parent4096/34304 regressions pass after the shared texture declarations. Accepted continuation total60 addresses:53exact/seven near. Two prior false identities were separately retired; these are session diagnostics, not a manually edited project coverage total.

Next helper evidence: quick_draw_erase_decomp.c identifies009E15E0 as vector<CQuickDrawTriInfo>::erase(first,last), not the old TextureCache name. It calls counted copy009DE940, then real CTexture::Uninitialise and CWideString destructor0099B510 over the trailing records. DrawBufferedPolys clears both info vectors through erase(begin,end), and clears vertex vectors through inlined class-element erase. Recovering the typed eraser/copy is next before the full draw parent. No native rasterized frame or commit.


## Native buffered-record erase completed

Three exact bodies now share the typed vector erase provider: compiler-generated CQuickDrawTriInfo::operator=009DE6F0 (146bytes), counted copy009DE940 (71), and vector<CQuickDrawTriInfo>::erase009E15E0 (99). The first was incorrectly labeled a copy constructor; its calls to string/texture assignment and shared-class compiler output establish assignment. Both counted copy and erase have multiple returns; stopping at the first return truncates their bodies. The complete relocated bodies are exact. The old local View-class erase source is archived under rebuild/backlog/mislabels/009e15e0.

gate_native_quick_draw_erase.py passes990 native cases/7570 full4KiB snapshots with three omitted-provider link failures. It composes all three bodies with the actual accepted CTexture assignment/Uninitialise providers. Every valid erase interval at lengths0..8 is exercised with null/live/mixed textures and callbacks that mutate all record texture flag words. Empty erases execute self-assignment when trailing records exist, retaining retail behavior. String assignment/destruction and COM AddRef/Release remain recorded boundaries, not claims of complete string ownership execution.

Fresh explicit closure work/codex-native-startup/native-gfmain-quick-draw-erase-explicit-clean-20261006 compiles1366 sources,zero duplicates,415 unresolved. The first rebuild with the prior seed did not select the new erase provider because the DrawBufferedPolys parent remains unresolved; the explicit seed includes it and resolves its string-assignment dependency. Session total63 accepted addresses:56exact/seven near. No frame or commit. Next is the main DrawBufferedPolys parent and its seven-entry state-block library.


## DrawBufferedPolys native parent accepted

The full3435-byte DrawBufferedPolys009D9C80 now has a genuine VC7.1 C++ provider:3451bytes,961/964 instructions,92.0519% similarity,93 audited relocations. Stop tuning this accepted near match. It uses real shared renderer/state/shader/texture/record types. Five previously opaque renderer vectors are named with PDB order and retail offsets3E58,3E64,3E70,3E7C,3E88; static assertions preserve total3EA4. CStateBlockFunctionBase is the PDB four-byte interface with one pure virtual Apply slot and no virtual destructor. LibraryStateBlockFunctions[7] is declared but its actual data provider remains next.

gate_native_draw_buffered.py passes576 cases/19288 full36KiB+988-byte shader snapshots, six required-provider omissions. The fixture links the actual accepted texture assignment/cleanup/cache attachment and draw-record eraser beneath the parent. Both record counts0/1/3, eight combinations of line/triangle, texture/window/blend variation, four prior shader enable combinations and callback mutations are checked. Mutating state-realization callbacks swap device and vertex-buffer pointers; window callbacks flip signed-zero bits; block/shader callbacks change update flags and layout indices. The parent correctly reloads these values at the retail call boundaries. Both queue erases execute real native cleanup, including class-element vertex erase loops.

Shader updates, state-block Apply, state realization/restoration, viewport, string destruction, COM and sorting remain recorded boundaries. The qsort response reverses complete records deterministically on both sides, validating the parent's use of both returned sequences without claiming CRT sorting equivalence. The test-only qsort-renamed object has identical code and all relocations audited. Only data symbols are mapped at retail addresses; no executable retail code is linked into the native fixture. Actual depth-comparator providers are linked as the passed function pointers, with their separate4800-case gate providing their behavior evidence.

Static data adds01396E58=1/D3DTEXF_POINT with retail+SDK proof; mipmap mode uses the existing native D3DTextureMipMapModeTable at01396E64 rather than creating a second independent alias. Static-data gate now27 symbols/144bytes/486 linked scenarios. AddText852/3444 and Clear4096/34304 regressions pass with the final shared header. Fresh closure work/codex-native-startup/native-gfmain-draw-buffered-clean-20261006 compiles1370 sources,zero duplicates,415 unresolved. Accepted continuation total64 addresses:56exact/eight near. No rasterized frame, Lua changes, retail launch or commit.

Next: actual state-block library data at01396510. Retail's seven pointers are null,01396530,01396534,01396538,0139653C,01396540,null; the five instances already contain vtable pointers in the image (not BSS). PDB types are CStateBlockFunctionSold, AlphaAdditive, AlphaMultiply, AlphaBoolean and AlphaText. The constructor009D9A30, Render2DDrawList009DA9F0 and rejected SIMD/viewport candidates remain unresolved.


## State-block library and composed buffered drawing

Five typed Apply providers accepted: Sold009DF060 (1736bytes,99.7738% near), AlphaAdditive009DF730 (1633exact), AlphaMultiply009DFDA0 (1633exact), AlphaBoolean009E0410 (1738bytes,99.7732% near), AlphaText009E0AE0 (1635exact). The old Sold View-class source/test is archived; this corrects its callable shared-type identity, not a request to retune its previously accepted algorithm. Each now resolves the actual shared CSystemManager->CDisplayManager->CRenderManager chain and queues states through the shared SetRenderState implementation.

CRenderManager2D_StateBlockLibrary_01396510.cpp defines the actual seven-entry table and five static polymorphic objects. The image has null slots0/6 and five valid slots1..5. Eleven complete data spans (table, five objects and five Apply vtable slots;68bytes) match retail after audited relocation. The compiler emits initialized data, without a CRT initializer or substituted zero-filled table. Three additional render defaults are SDK/retail verified:01396DE8 D3DCMP_GREATER,01396F78 D3DBLEND_SRCALPHA,01396F7C D3DBLEND_INVSRCALPHA. Static-data gate30symbols/156bytes/486scenarios.

gate_native_state_blocks.py passes480cases/9600full40KiB snapshots with two omissions. Actual library virtual dispatch and all five methods execute repeatedly in forward/reverse order; two renderer objects, zero/high/all bookmark masks, prior captures/dirty queues and root callbacks that switch the selected renderer are checked. Only GFGetSystemManager is a recorded boundary.

The composed parent gate `gate_native_draw_buffered.py --state-blocks` passes576cases/19136full36KiB+shader snapshots with eight omissions, executing the actual library and Apply bodies beneath DrawBufferedPolys and its accepted texture/erase dependencies. Composed cases use only valid library slots1..5. The original boundary-focused mode still passes576/19288. Shader/window/state-realization/restoration/string/COM/sort boundaries remain; this is not GPU rendering.

Fresh closure work/codex-native-startup/native-gfmain-state-block-library-clean-20261006 compiles1372sources,zero duplicates,414unresolved. Session total69 addresses:59exact/10near (includes replacement of the prior Sold View ABI). Renderer constructor and Render2DDrawList remain the next parents; no native frame, retail launch or commit.


## Render2D storage dependencies and font ABI

Three more exact bodies are registered: CTexture::InitialiseFromPreallocatedTexture009FA230 (74bytes), CTexture::CalcByteLength009F9EE0 (complete143bytes), and vector<CTVertexRHWColSpecTex1>::erase009E1440 (complete79bytes). They replace legacy View-class providers, archived with tests under backlog/mislabels/<address>. Texture bodies share CTexture_PreallocatedStorage_009fa230.cpp and actual D3DSURFACE_DESC/COM types. A first ByteLength += expression failed behavioral replay: retail snapshots the whole size/ownership word before GetColourDepth, then overwrites callback mutations with that saved word. CTexture now exposes the physical union word LengthAndAllocation alongside its original bitfields, with unchanged8-byte size. The corrected source is exact and passes288cases/1584full4KiB snapshots, including pointer/packed-word mutations, negative/zero/multiple mip levels, HRESULT failures and wrapping size multiplication. Pixel-format and COM operations remain boundaries.

Vertex erase passes969cases/969full4KiB snapshots with all valid intervals at lengths0..16 and no callback boundaries. Its standalone specialization must be inline with explicit instantiation: a strong specialization collided with the inline copy already emitted by DrawBufferedPolys. The final COMDAT form keeps exact79-byte code and removes the duplicate. Fresh final closure work/codex-native-startup/native-gfmain-render2d-storage-final-clean-20261006 compiles1374sources,zero duplicates,414unresolved. Session total72 addresses:62exact/10near.

Shared CFontBank slots2/3/5 now have PDB signatures GetFontMaxHeight()const, PrepareToRender(bool)const, and Render(CWideString const&,C3DVector const&,float,CRGBColour const&,unsigned long,CTexture const*)const. Two legacy fixture subclasses were adapted. The counted-pointer fixture compiles; the old frontend Run fixture still fails at its unrelated CTBaseSingleton<CFileInstaller>::Release specialization (C2910,line370), not the font methods. Modern composed font gate2208/29920 passes; texture binding384/3360, quick-record erase990/7570 and composed drawing576/19136 regressions also pass.

Current next action: `work/parity_addtext/render2d-list-probe/native_Render2DDrawList.cpp` compiles3296bytes/880instructions versus complete retail3344/901 at91.1847% similarity. It is **not accepted or registered** until behavior replay passes. No byte tuning needed. It uses the final shared headers and exact texture/vertex-erase prerequisites. Text position is (StringX+Window.left,StringY+Window.top,StringZ), calculated before viewport/font callbacks; the six-argument font Render passes flags0 and null texture. Evidence disassembly009DB3B7 onward corrects the decompiler's misleading stack aliases. Gate still needs mixed text/geometry, font callback changes, empty input, depth boolean,2001-primitive flush boundary and cleanup comparisons.


## Render2DDrawList accepted

The complete3344-byte Render2DDrawList009DA9F0 now has a production native body:3296bytes,880/901instructions,91.1847% similarity,127 audited relocations. Stop tuning it. It keeps retail's captured record pointer across shader/font callbacks, bitwise window comparisons, cached text position before viewport/font calls, shader suspension/reapplication around text, texture transfers and the exact count>2000 flush condition. This replaces no parent with an authored planner; the actual native manager method executes.

gate_native_render2d_list.py passes520cases,14996full-state snapshots and20012ordered compact string-destruction events, with nine required-provider omissions. It links actual state-block library/Apply, texture adoption/accounting/assignment/cleanup/cache binding, quick-info erase and typed vertex erase. Small queues0/1/3/6 cover empty, normal, text-only, alternating text/geometry, consecutive text, line/triangle switches, four prior shader-enable combinations, depth boolean and mutating callbacks. Font preparation changes later scale/font pointers and shader flags; text rendering changes flags again; realization swaps device/vertex buffers; viewport callbacks flip signed-zero bits. The position argument is captured by value.

Large stable batches of2000/2001/2002/4003 primitives verify that the threshold flushes2001 primitives before handling the next record. Small cases compare the full36KiB arena+988-byte shader object at every event. Large cases compare768KiB at engine/draw/return boundaries; string cleanup emits compact ordered events, and the final full arena checks every destroyed record and vector endpoint. This avoids masking state or writing multi-gigabyte snapshots for each trivial destructor. Shader, state realization/restoration, viewport, font virtuals, string destruction and COM remain boundaries. Pixel-format callbacks abort if unexpectedly reached: the parent only adopts null temporary textures, whose actual native accounting returns without querying a format. No GPU/frame claim.

Fresh closure work/codex-native-startup/native-gfmain-render2d-list-clean-20261006 compiles1375sources,zero duplicates,413unresolved. Session total73 addresses:62exact/11near. Native visual verification is still outstanding; no retail launch or commit.

Next renderer-constructor evidence: CShaderDataBank derives the existing typed CBankFile (retail110bytes, donor174), then Shaders array12bytes at110 and the already-typed CShaderBankStateBlock12bytes at11C, total128. Donor development-only ShaderPreParser/EntryParserStates/IntermediateFileFolder members are absent in retail. ConstructorA62020 calls9CD480 then sets vtable129D644, zeros array110/114/118 and sets state vtable129D630. Shared fable_shader_bank_state.h and fable_game_text_bank.h already cover those bases. CShaderResource OpenShaderBank and setters are already typed/gated; A5D5F0 is incorrectly labeled GetPixelShader and must be recovered as GetVertexShader(name)const. CVertexShader's assignment must release old Data, set new Data, then increment new ReferenceCount (PDB returns const CVertexShader&). Renderer constructor577bytes remains next. Evidence: shader_bank_startup_decomp.c and shader_bank_pdb.py; no new bank header/body has yet been written.


## Shader bank lifetime and vertex lookup accepted

Five more addresses accepted; session total78 (66exact/12near). CShaderDataBank constructorA62020 (48bytes) and destructorA620D0 (51bytes) are exact. Shared fable_shader_data_bank.h models the retail128-byte bank, including inherited CBankFile110bytes, shader pointer array at110 and CShaderBankStateBlock at11C. Donor-only AddEntryMultipleFiles/GetEnumerationType overrides are deliberately absent: the retail20-slot table inherits them. gate_shader_bank_lifetime.py now has its previously absent production source/fixture and passes1536compiled scenarios plus60retail comparisons, including CloseShaders changing/clearing buffer ownership, untouched state payload, destructor ordering and vtable targets.

Vertex lookup now uses the shared CVertexShader return ABI. A5D510 indexed lookup is208/212bytes,72/73instructions,95.1724% near; A5D5F0 name lookup37bytes and A61FA0 bank indexing16bytes are exact. The false GetPixelShader wrapper and inventory GetAbilityLevel identity are archived under rebuild/src/backlog/mislabels. CShaderBankEntry retail24bytes contains two12-byte vectors; VSConstantLayout is at20. The inline CVertexShader copy constructor increments nonnull Data reference count, and the resource cache is mutable for the const lookup method. Name lookup, indexing and bank access execute together in gate_native_shader_lookup.py:288compiled cases/1440full4KiB state snapshots, three provider omission checks. Cached/new handles, null Data, reference counts0/1/2/FFFFFFFF and callback mutations of bank/vector/declaration are covered. String operations, allocation and shader creation remain recorded boundaries. Three separate retail-versus-relocated-candidate emulation cases retain the native allocation failure fault at address4 after storing a null cache entry; no invented graceful fallback. Three vtable words are normalized; the emitted table and returned native table pointer are checked independently. No executable retail code is linked into compiled fixtures.

Shared vertex lifetime regression passes5632compiled/3300comparative scenarios; bank lifetime regression passes. Fresh closure native-gfmain-shader-lookup-clean-20261006 compiles1379sources with zero duplicates and421unresolved. This explicitly seeds the newly recovered bank lifetime and lookup; the higher frontier exposes bank virtual dependencies. Renderer constructor remains next. No frame, retail launch or commit.


## Renderer constructor accepted

CRenderManager2D::CRenderManager2D009D9A30 is now native:569built/577retail bytes,161/162instructions,92.8793% similarity,33audited relocations. Session total79 (66exact/13near). Stop tuning the passing match. fable_display_viewport.h now embeds the actual128-byte ShaderDataBank at3D20, counted shader resource at3E48 and CVertexShader at3E50; six draw vectors remain at3E58..3E94. The existing ClearRenderTarget/DrawBufferedPolys/Render2DDrawList sources now directly access the typed shader member. All offsets and total3EA4 size are asserted. CVertexShader assignment releases prior Data, assigns the new pointer and unconditionally increments its reference count, matching retail.

The native constructor opens shaderPath + CWideString(L"pc\\") + L"shaders.big" when the bank manager enables retail banks. It tests SHADERS_BBBLIB, opens the bank with48hex flags, creates/assigns its shader resource, opens/configures it and obtains VSHADER_BBBLIB_2D. The shader-name local must be constructed before loading the current resource receiver, so mutations in that constructor remain visible. The temporary resource's counted-pointer cleanup and temporary shader release execute natively. The low flag bit controls AlternateRenderTargetClearEnabled.

gate_native_renderer_ctor.py passes1296compiled scenarios/19992full snapshots (24KiB arena plus56-byte bank manager and988-byte shader manager), two required-provider omissions. Cases cover both retail-bank/exists states, flags0/1/2/FFFFFFFF, control-block null/ref1/ref2, prior shader null/ref1/ref2, returned shader reference0/2/FFFFFFFF and callback mutations. Boundaries record core/bank/vertex construction, strings, bank open, resource creation/assignment/configuration and lookup; control and virtual-delete callbacks are observed. Four arena vtable words and the native control callback pointer are normalized; the emitted renderer table and relocation identities are audited. No engine-boundary or GPU-frame claim.

The constructor links the actual40-byte Fable2DShaderDeclaration_0129D708: FLOAT4 at0, D3DCOLOR at16/20, FLOAT2 at24, TEXCOORD usage indices0..3 and the end marker. Data is byte-exact against retail, separately catalogued without function coverage, and required by the compiled fixture. Fresh closure native-gfmain-renderer-ctor-clean-20261006 compiles1381sources,zero duplicates,421unresolved. Typed-header buffered-draw regression passes576cases/19136snapshots; vertex lookup288/1440. Clear regression passes4096cases/34304snapshots; mixed-list regression passes520cases/14996full snapshots plus20012compact events after these field changes. Native frame remains outstanding.

Next: shader-bank CloseShadersA62050, CloseA62110, NotifyEntryRemovedA61FC0, GetDataFormatVersionA61F90 and PrepareDataForInclusionA62000 all have exact scratch candidates in native_ShaderBankCleanup.cpp, awaiting behavior gate. CloseShaders includes its full122-byte body, including the statically unreachable STLport memmove arm beyond the first ret. CBankFile::GetEntrySymbol9CC1D0 has a57-byte typed scratch candidate, returning (Unknown) for an empty symbol array; it too awaits gating. Evidence shader_bank_cleanup_decomp.c and shader_bank_cleanup_disasm.py. No promotion for these scratch candidates yet.


## Shader bank cleanup, symbol access and defaults accepted

Six exact typed methods are registered: GetDataFormatVersionA61F90 (6bytes), CloseShadersA62050 (full122bytes), CloseA62110 (16), NotifyEntryRemovedA61FC0 (full63), PrepareDataForInclusionA62000 (32), and CBankFile::GetEntrySymbol9CC1D0 (full57). The old local-struct/fastcall wrappers for version, preparation and close are archived with their tests. Full-body validation retains the unreachable memmove branch beyond CloseShaders' first ret and both removal/symbol return branches. Cleanup reloads vector start/size after virtual deletion; NotifyEntryRemoved zeros the current array after a callback changes its storage. The empty preparation body performs actual by-value CCountedPointer destruction with ret1Chex. Empty symbol arrays return (Unknown); populated arrays copy the selected CCharString.

gate_native_shader_bank_cleanup.py passes732compiled cases/1250full4KiB snapshots, two omissions. Null/live/mixed entries, arrays0..6, all removal indices and callbacks moving/shrinking/growing storage are covered; ownership null/ref0/ref1/ref2/FFFFFFFF and release mutation are covered. The unused array/data arguments remain untouched. String constructors are recorded ABI boundaries. The shader lookup gate's --bank-symbols mode links the real accessor:288cases/1440snapshots and four omissions pass, including bank changes between layout-string copy and symbol lookup. The original lookup and lifetime gates still pass.

CShaderBankStateBlock::GetDefaultA61F20 is also exact27bytes. It zeros the entire8-byte state, including padding, enables shader optimisation and sets target0; it operates on output, not necessarily this. gate_shader_bank_default.py compares1024compiled/native full128-byte states across all256fills and separate, identical and overlapping receiver/output positions. Output vtable and unrelated bytes are preserved. One omission check. Session total86addresses:73exact/13near.

Fresh closure native-gfmain-shader-bank-default-clean-20261006 compiles1384sources with zero duplicates and414unresolved. Native frame remains outstanding. No retail launch or commit.

Next unaccepted candidate: NotifyEntryAddedA623D0 compiled174/174bytes,68/68instructions,97.0588% near, with actual generic shader-pointer overflowA62740 emitted216bytes exact. The production shader-bank header now declares the evidenced CShaderBankEntry(type,index,bank) constructor; its bodyA62120 remains open. Work files native_ShaderBankNotifyAdded.cpp, probe_shader_notify_added.py, probe_shader_bank_overflow.py. Behavior-gate notification and overflow before promotion; do not tune the passing instruction match.


## Shader bank entry addition accepted

NotifyEntryAddedA623D0 is registered at174/174bytes,68/68instructions,97.0588% near; its emitted typed pointer-vector _M_insert_overflowA62740 is exact216bytes. gate_native_shader_bank_add.py passes2216compiled cases/9120full16KiB snapshots and one source-provider omission. It executes both methods: parent cases cover existing/spare/reallocated arrays, zero-filled gaps, shader types0/1/2/FFFFFFFF, failed entry allocation and constructor callbacks replacing the pointer array before the final store. Independent overflow cases cover all insertion positions for lengths0..6, fill counts0/1/3, at-end flags, aliased fill values and malloc/free callbacks. Allocator mutation changes capacity or values; constructor mutation changes the parent array; free mutation is overwritten by the actual overflow commit. The fixture's renamed malloc/memmove/free relocations are audited against unchanged production instruction bytes; entry construction and base notification remain boundaries. No graceful fallback is invented for vector-allocation failure.

Fresh closure native-gfmain-shader-bank-add-clean-20261006 compiles1385sources,zero duplicates,414unresolved; the missing notification is replaced by its entry-constructor dependency. Session88addresses:74exact/14near. Shared header now also declares CBankFile::ReadEntryData(index,offset,data,size), evidenced by PDB and retail9CC2A0.

Next: CShaderBankEntry constructorA62120 candidate native_ShaderBankEntry.cpp compiles675/678bytes,237/238instructions,97.2632% near,20relocations. Stop byte tuning; it awaits behavior replay. It constructs type, code/constant vectors and layout string, reads bank bytes into a zero-filled CArrayVector<char>, uses CMemoryDataInputStream to read a bytecode byte length (floor /4), resizes code with zero, reads words, reads a constant count and null-terminated names, then a final VSConstantLayout string. It preserves index as the initial read destination and zeros that same variable before resize/constant-count read, important when the stream position guard suppresses reads. Existing typed memory-stream/string-vector providers cover several dependencies; typed unsigned-long resize464931 and bank ReadEntryData9CC2A0 remain unresolved wrappers/dependencies. Candidate files/probe: native_ShaderBankEntry.cpp, probe_shader_bank_entry.py, shader_bank_entry_decomp.c. No constructor promotion yet.


## Shader entry construction, bank reads and bytecode vectors accepted

CShaderBankEntry constructorA62120 is registered at675/678bytes,237/238instructions,97.2632% near. gate_native_shader_bank_entry.py passes486compiled cases/7428full snapshots (16KiB arena plus28-byte stream), one source omission. It covers empty and nonempty banks, code lengths with remainders, constant lists, stream chunk limits, signed-position suppression/wraparound and mutating string/vector callbacks. Bank, stream construction/overflow, strings and code resize are recorded boundaries; no shader execution is claimed.

CBankFile::ReadEntryData9CC2A0 is exact95bytes. gate_native_bank_read.py passes768cases/3072full4KiB snapshots, one omission, no normalized bytes. Both packed metadata modes, index underflow,32-bit offsets/lengths and file-pointer mutations pass. The unused packed-size lookup is retained; the file receiver is reloaded after SetPosition before Read. Packed decoding and file virtuals remain boundaries; no filesystem I/O.

Five unsigned-long vector providers are registered: resize464931 (62bytes exact), erase46496F (52exact), fill insertion4649A3 (252/264bytes,108/112instructions,98.1818% near), uninitialized copy464AAB (35exact), overflow464ACE (188exact). gate_native_ulong_vector.py passes4664compiled cases/12554full16KiB snapshots with three source omissions and no normalized bytes. All five actual providers compose together; cases cover empty/full/spare capacity, resize growth/shrink, insertion/erase ranges, aliased fill, overlapping copy, and callbacks mutating values/vector fields during malloc/memmove/free. Renamed CRT relocations are audited against unchanged production code. Allocation failure is not modeled. The old mislabeled pair-array push_back wrapper at464931 and its test are archived. Retail uses size optimization in this unit, preserved locally. Erase uses an inline specialization plus explicit instantiation so existing COMDAT emissions can coexist. Session95addresses:79exact/16near.

The first closure after these additions reports1389sources/413unresolved but one erase duplicate; corrected COMDAT emission is being rechecked. Do not cite it as a clean build yet. Next renderer lifetime address was verified through vtable129B9EC -> deleting destructor9E16E0 -> body9C2FD0; prior landscape/occlusion labels are wrong. Retail body destroys six draw vectors, vertex shader, resource ownership, shader bank and renderer core in reverse member order. Evidence renderer_destructor_decomp.c. No native frame, retail launch or commit.

Fresh post-correction closure native-gfmain-ulong-vector-comdat-clean-20261006 compiles1389sources with zero duplicates and413unresolved. Both production and composed vector gates retain4664cases/12554snapshots after COMDAT correction.


## Renderer lifetime accepted

Retail vtable129B9EC selects deleting destructor9E16E0, which calls9C2FD0. Corrected the false landscape/occlusion labels and archived the local-T deleting wrapper/test. The shared CRenderManager2D header now uses an implicit virtual destructor; an explicit empty body adds a vtable store absent from retail. Existing constructor production emits the actual217-byte destructor,30-byte scalar deleting destructor and58-byte vector<CQuickDrawTriInfo> destructor, all exact. The existing correctly typed vector provider remains available; its manifest now links the stronger composed gate.

gate_native_renderer_dtor.py passes1204compiled cases/16876full24KiB snapshots and a source omission. It executes the renderer and record-vector teardown together, preserves shader then base vtable identities by mapping compiled pointers to their retail counterparts, and normalizes only two recorded control callback pointers. Cases cover empty/nonempty vectors, allocation masks, shader/control refs null/0/1/2, flags0/1/2/FFFFFFFF, and callbacks changing vector storage or control ownership before subsequent reads/deletion. Texture/string cleanup, shader-data virtual deletion, bank/core lifetimes and allocator release remain recorded boundaries. Resource pointer reload after its release callback is observed. No GPU teardown/frame claim.

Constructor regression1296/19992, buffered drawing576/19136, mixed list520/14996full plus20012compact and clear4096/34304 pass after the header change. Legacy display fixtures needed explicit recorded nested bank/vertex lifetime boundaries now that fields are typed; updated fixtures independently pass2304initialization and6144destructor cases. Full display-lifetime comparative job remains running; its already-compiled fixtures predate this boundary correction and may fail at final link. Do not rerun unchanged failures or claim its full result prematurely.

Fresh closure native-gfmain-renderer-dtor-clean-20261006 compiles1389sources,zero duplicates,412unresolved. Session98addresses:82exact/16near (includes renewed verification of the existing record-vector destructor). No retail launch or commit. Next: CRenderManager derived lifetime. Its vtable129B400 points to scalar deleting destructor9C3100, also calling9C2FD0: retail folds the empty derived destructor with the 2D body. Shared derived header still has an explicit unresolved destructor declaration; use the native implicit lifetime rather than add an explicit-body vtable store. Current scratch fable_display_viewport.h copy is only an earlier probe and must not become production.


## Derived renderer lifetime accepted

CRenderManager now also has an implicit virtual destructor. Its emitted scalar deleting destructor9C3100 is exact30bytes and calls the same217-byte2D body9C2FD0, as the actual129B400 vtable does; no invented derived vtable write. The emitted provider belongs to the existing CDisplayManager::Init production unit. Its old mislabeled landscape wrapper/test is archived. gate_native_renderer_dtor.py --derived passes1204cases/16876snapshots through the actual derived deleting entry, with the same exact217/58-byte teardown helpers. Fresh compiled display fixtures pass8448cases, with renderer-deletion observations in the recorded core teardown boundary. The slow original display-lifetime run completed all6144comparative scenarios then failed linking its stale fixture objects; the corrected gate now runs compiled fixtures before the expensive comparisons, and a fresh full run is active. Session99addresses:83exact/16near. Clean closure native-gfmain-renderer-derived-dtor-clean-20261006 compiles1389sources/zero duplicates/411unresolved.

Next shader-manager shutdown98A7A0 is79bytes exact in scratch. Its resource list is the PDB ResourceManagers at retail3A8 (12-byte vector of8-byte counted pointers), accessed through a typed view without inventing unrecovered outer construction. Existing resource UninitialiseA5D7A0 and CloseShaderBankA5D750 are native; the new composed gate is in progress. Evidence shader_manager_shutdown_decomp.c and shader_resource_lifecycle_decomp.c. Shader-manager Initialise98ACF0 evidence was also exported: matrices, resource initialization and pixel-constant name/table setup need typed recovery before promotion.


## Shader-manager shutdown accepted; initialization candidate

Uninitialise98A7A0 is79bytes exact and registered. gate_native_shader_shutdown.py passes420cases/1296full16KiB snapshots, two source omissions, no normalized bytes. Actual manager shutdown composes with existing resource Uninitialise17bytes and CloseShaderBank69bytes. Shader virtual deletion is the only recorded boundary; callbacks replace/shrink/grow manager resources or replace a resource shader array. Null/live/mixed shader entries, resource counts0..6 and shader counts0..3 pass; manager D3DDevice is cleared last and counted owners remain untouched. Clean closure native-gfmain-shader-shutdown-clean-20261006 compiles1390sources/zero duplicates/410unresolved. Session100addresses:84exact/16near.

Full corrected display-lifetime gate now passes8448compiled/6144comparative scenarios. No job from earlier renderer checks remains running.

Shader-manager initialization98ACF0 now has a1333/1333-byte,302/302-instruction,96.0265% near-match candidate native_ShaderManagerInit.cpp. Stop byte tuning. It awaits behavior replay. Shared manager fields now identify retail View/Projection/ViewProjection/CombinedProjection/Texture matrices, FogTransform, CameraPos, TextureTransformCount, TextureFactor and MultisampleViewPortScale; offsets are asserted. ResourceManagers and PSConstNames retain opaque lifetime storage with typed views. New fable_indexed_array.h models PDB Array plus IndexMap (retail12+12bytes); its push_back method is only declared. Added the PDB-evidenced four-float C4DVector constructor. Three pixel-constant table entries use temporary C4DVector assignments, preserving retail local stores/reloads. Nine named local CCharString scopes preserve caller stack-address passing rather than relying on the constructor return value. Evidence shader_manager_init_decomp.c, shader_pdb.txt, probe_shader_manager_init.py. Initialization remains unpromoted, and these newest header refinements require affected regression gates and a fresh closure before claiming current clean state.


## Shader-manager initialization accepted

Initialise98ACF0 is registered at1333/1333bytes,302/302instructions,96.0265% similarity with40audited relocations and nine validated literal strings. gate_native_shader_init.py passes540compiled cases/16920full16KiB snapshots and one source omission. No arena bytes are normalized; literal arguments are mapped to validated name indices and local string values are recorded. Cases cover resource counts0/1/3, existing layout counts0/1/3, table counts0/3/9/10/12, existing names0/2 and six mutation modes. Resource-list replacement/shrink/grow and table replacement during name insertion or resize pass. The parent appends nine constant names, resizes table to nine, sets first three vectors to ones, establishes matrix/state defaults and leaves unused matrices/fog/camera state untouched. Layout resize, resource binding, string/indexed-array operations and table insertion/erase are recorded boundaries. No GPU initialization claim.

After the header refinements, shader updates128/3120, enables252/1656, composed layout selection1620/1782 and lights672/3920 all pass. Fresh closure native-gfmain-shader-init-clean-20261006 compiles1391sources/zero duplicates/410unresolved; the missing initialization is replaced by CIndexedArray<CCharString>::push_back98DB20. Session101addresses:84exact/17near. All launched jobs finished. Earlier scratch shadow fable_display_viewport.h was renamed renderer_implicit_probe_header.txt to avoid hiding the shared header in later probes. Next: indexed name-array append, then the real shader-manager global lifetime. No frame, retail launch or commit.


## Indexed constant-name append accepted

CIndexedArray<CCharString>::push_back98DB20 is124bytes exact, including both returns and the overflow tail. gate_native_indexed_char_append.py passes220cases/1100full4KiB snapshots, one omission, no normalized arena bytes. It caches size before copying the map key, inserts pair<const CCharString,long> through unique-map insertion, destroys that temporary, then appends using live vector fields. Map duplicate success/failure is ignored by the parent; array append still occurs. Cases cover full/spare storage, aliases and copy/map/destructor callbacks changing vector storage or finish. String operations, unique-map insertion and overflow are boundaries; no claim about the map implementation itself. Source is registered, native shader-init closure now1392sources/zero duplicates/409unresolved at native-gfmain-indexed-char-append-clean-20261006. Session102addresses:85exact/17near.

Next actual shader-manager global lifetime: retail ctor98AB00, dtor98ABF0, vtable129A080, CRT thunk around1223D00 and atexit122A4C0. Evidence shader_manager_constructor_decomp.c/.log and shader_manager_lifetime_decomp.c. Constructor proves the full retail size is3E8, including a final12-byte vector<CPreTransposedBoneMatrix> at3DC; the current partial header ends at3DC. Donor CPreTransposedBoneMatrix is48bytes and CIndexedArray is Array plus std::map (retail12+12). Need genuine typed member lifetimes before defining global13BC470. Only three legacy fixtures directly construct global shader managers; their global construction must remain a recorded boundary when the full class gets native ctor/dtor declarations. Existing permanent native gates use mapped/raw state and do not construct it. No global provider or ctor/dtor promotion yet.


## Full shader-manager and pool lifetime accepted

The shared shader-manager class now has genuine typed member lifetimes and full retail size3E8, including vector<CPreTransposedBoneMatrix> at3DC. The48-byte bone matrix is the PDB three-by-four float array. CPixelShader is an8-byte polymorphic value. Existing shader updates128/3120, initialization540/16920, shutdown420/1296 and lights672/3920 pass with the completed class. Legacy display fixtures retain explicit manager-construction boundaries; resource-create/vector fixtures use raw aligned storage for the parent they do not construct.

Manager constructor98AB00 passes237/232bytes,52/52instructions,94.2308% near; pixel constructorA5EC00 is16exact. gate_native_shader_manager_ctor.py passes1792cases/7936full4KiB snapshots, one omission, all256 fill bytes plus nested constructor and allocator mutations. It verifies the map sentinel and bone-vector tail without initializing untouched state.

Pool lifetimeA5C570 constructor74,987F10 destructor22,A5C600 ClearMemoryBlocks54 and987F30 scalar deletion43 are all exact. gate_native_pool_owner.py passes912cases/2528full4KiB snapshots and one omission. Wrapping alignment, untouched root status, empty/nonempty chains, initial ownership, cached next links and callback mutation pass. The source uses inline_depth(0) after construction to preserve the emitted cleanup helper. The old false input-manager destructor wrapper/test is archived.

Manager destructor98ABF0 passes229/246bytes,67/73instructions,91.4286% near. Its scalar deleting98DE90 is30exact, indexed-name-array destructor98D9F0 is113exact and pixel scalar deleting98C3D0 is55exact. gate_native_shader_manager_dtor.py passes1296cases/9538full16KiB snapshots, three provider omissions. Actual parent/member, pool, layout/range and base destruction compose; heap, map erase, string destructor, Uninitialise and virtual delete callbacks remain recorded boundaries. Counts0/1/3, references0/1/2, ownership, storage and callback mutations, and flags0/1/2/FFFFFFFF pass. Vtable and callback addresses map by identity; no state bytes are masked. The false landscape deleting wrapper/test is archived. Stop tuning the passing near match.

The genuine global013BC470 is now a1000-byte BSS CShaderRenderManager object. Compiler-generated CRT initializer1223D00 is22exact and registered atexit122A4C0 is10exact; the old generic thunk/test is archived. gate_native_shader_manager_global.py passes eleven full-object-plus-map-header snapshots and seven source omissions. It executes actual constructor, destructor, empty Uninitialise, pool, layout and base lifetimes, observing before allocation, before main, before exit, each free and after the real CRT-registered destruction. CRT registration and zero storage are audited. Only heap APIs are invoked as boundaries on this empty-resource path; the populated resource shutdown has its own gate. No fake zero-only global or manual replacement startup body.

Fresh closure native-gfmain-shader-manager-global-clean-20261006 compiles1396sources with zero duplicates and409unresolved. An obsolete raw unknown000 access in CreateResource was exposed by the clean build and replaced with the actual ResourceManagers member; its refreshed gate passes289compiled/144comparative cases at97.4684%. Resource-vector and full display-lifetime regressions are still running. Session114addresses:95exact/19near. No frame, retail launch or commit. Next unresolved renderer dependency: CPixelShader::CountReferences; combined projection and viewport candidates remain unrecovered.


## Pixel reference count and display-engine constructor accepted

CPixelShader::CountReferences987EE0 is now the typed7-byte exact provider. gate_native_pixel_references.py passes2048cases with full64-byte state, all byte fills, references0/1/FFFFFFFF/12345678 and aliased receiver/data storage. The previous local-T placeholder and print-only test were replaced. Clean closure native-gfmain-pixel-references-clean-20261006 compiles1397sources/zero duplicates/408unresolved. Resource-create/vector regressions pass289compiled/144comparative and638comparative respectively; full refreshed display-lifetime passes8448compiled/6144comparative. Session115addresses:96exact/19near.

Bounded follow-up intrinsic probes for GFClear4DVectors still fail90% similarity. No SIMD candidate was promoted. Do not add custom-alignment scratch declarations to production. Focus moved to the unresolved display-engine startup chain.

CDisplayEngine constructor434E10 is334bytes exact. The shared header now models its complete256-byte typed camera, fade/letterbox, timing, view-owner and world fields. New CFadeInFadeOutBase constructor preserves close/open timer and flag assignment order and uses a temporary black CRGBColour assignment, which reproduces the retail stores; fade durations and letterbox ratio remain untouched. CCamera gained its PDB four-argument constructor declaration and an empty default constructor preserving prior default behavior. gate_native_display_engine_ctor.py passes4096cases/12288full4KiB snapshots, one omission, camera arguments and base/camera mutations. Shared Init values can change through callbacks, and untouched fields remain observed. Constructor alone does not initialize the engine or produce a frame.

Fresh closure native-gfmain-display-engine-ctor-clean-20261006 compiles1399sources/zero duplicates/414unresolved: the real constructor exposes its camera and virtual-table dependencies, including older local-type display-destructor helper names. Display-engine leaves regression passes768compiled/1536comparative; camera-view regression remains running. Session116addresses:97exact/19near.

Next scratch Init4350D0 is180bytes with13relocations,53/53instructions,96.2264% conservative similarity before literal-data mapping (likely exact after relocation audit). Typed CDisplayViewManager is8bytes, owns CCountedPointer<NDisplayView::CViewBase>. Its scratch constructor640040 is16bytes; owner Reset436CC0 is103bytes; emitted DeleteData436BA0 is66bytes. These are not promoted yet; compose actual view construction and ownership helpers in initialization replay. Evidence display_engine_lifetime_decomp.c. Source candidates native_DisplayEngineInit.cpp/native_DisplayViewOwner.cpp; no global/header-byte shortcut. No frame, retail launch or commit.


## Display-engine initialization and typed teardown accepted

Init4350D0 is180bytes exact after auditing both strings and the double-zero literal. Actual CDisplayViewManager constructor640040 is16exact; CCountedPointer<CDisplayViewManager>::Reset436CC0 is103exact; DeleteData436BA0 is66exact. gate_native_display_engine_init.py passes1536cases/16744full4KiB snapshots, two omissions. All four providers compose: null/live ownership, refs0/1/2/FFFFFFFF, nested owners, new failure at8/12bytes, live engine virtual replacement and callback mutation pass. Strings/progress, engine initialization, heap and the innermost view delete remain recorded boundaries. The old640040 local-T constructor/test was archived. Session120addresses:101exact/19near.

UninitEngine4354A0 (29bytes), destructor4354C0 (105) and scalar deleting437070 (30) now all match exactly using the actual CDisplayEngine header. The prior destructor used invented local CDisplayEngineBase, GetSubsystem, Flush and free names; those cannot link to the genuine engine. PDB and callers identify CTBaseSingleton<CEngineManager>::Get436FB0 and CEngineManager::RemoveEngine6404B0; the argument is evaluated before singleton retrieval. The new header specializes the polymorphic singleton and models20-byte retail manager (list4/vector12 versus donor32bytes). The old local-type destructor, naked UninitEngine and mislabeled landscape scalar wrappers/tests are archived.

gate_native_display_engine_dtor.py passes2880cases/13080full4KiB snapshots, three omissions. It executes nested view-owner DeleteData and base teardown; refs, null ownership, flags0/1/2/FFFFFFFF and callback replacements test engine reload after Release, cached RemoveEngine argument across Get, and ownership pointer reloads. Camera-view regression finished14760compiled/88560comparative; all earlier jobs finished. Fresh display-engine-lifetime closure is running; seed migration replaces the retired destructor filename and stale symbol-index rows from this continuation were removed. Session123addresses:104exact/19near. No frame, retail launch or commit.

Next engine-manager Get and typed AddEngine/RemoveEngine list paths. Retail singleton013BA854 owns20-byte object with12-byte sentinel, list at+4/vector at+8; vtable1231584 selects deleting437020. PDB identifies AddEngine6404D0, RemoveEngine6404B0 and list remove6405B0 despite false Ghidra names. Evidence engine_manager_list_decomp.c. No provider for these new identities is promoted yet.


## Engine registration list accepted; singleton lifetime candidates

Fresh display-engine-lifetime closure completed1401sources/zero duplicates/410unresolved. Engine AddEngine6404D0 (44bytes) and RemoveEngine6404B0 (24) are exact; typed list<CIEngine*>::remove6405B0 (54) passes92.3077%. gate_native_engine_manager_list.py passes1176cases/2373full4KiB snapshots and two omissions, with0..6 nodes, repeated/missing/null values, aliases, cached iteration links/end and heap mutation. No bytes normalized. The old CTCInventoryAbilities-named wrapper/test for the shared6404B0 code is archived; caller/PDB evidence establishes the engine-manager use, without excluding identical folded implementations. New list::remove declaration is additive. Session126addresses:106exact/20near. These newest header/provider changes require a fresh closure.

Scratch singleton Get436FB0 is105exact; its implicit manager destructor437040 emits35bytes and scalar437020 emits30. Manager must use the implicit destructor: retail writes only the singleton-base vtable123156C after vector/list cleanup. Separate list destructor436900 candidate is62bytes. Owner Reset4366B0 candidate103 and DeleteData4361E0 candidate11. These lifetime functions await behavior gates. Actual global CCountedPointer definition emits12-byte CRT initializer (retail121DF10) and57-byte atexit destructor1229190, with8-byte BSS owner at013BA854. Raw reference scan found initializer address; Ghidra xrefs omitted it because that CRT range is not fully analyzed. Scratch files native_EngineManagerGet.cpp/native_EngineManagerOwner.cpp/native_EnginePointerListLifetime.cpp/native_EngineManagerGlobal.cpp. Evidence engine_manager_lifetime_decomp.c/.log. No active verification jobs, frame, retail launch or commit.


## Engine-manager ownership, real global and display WorldUpdate accepted

Engine-manager Get436FB0 (105bytes), implicit destructor437040 (35), scalar437020 (30), counted Reset4366B0 (103), DeleteData4361E0 (11), and list destructor436900 (62) all match exactly. gate_native_engine_manager_owner.py passes3384cases/9039full4KiB-plus-owner snapshots and three source omissions. Allocation failures, existing/null ownership, reference limits, deleting flags, list lengths and callbacks replacing owner/head/links are covered. Old local-type/mislabeled providers and tests were archived and their stale index rows removed.

The actual8-byte BSS singleton013BA854 now has native compiler-emitted CRT121DF10 (12bytes) and atexit1229190 (57), both exact. gate_native_engine_manager_global.py passes19 snapshots and six source omissions, executing real startup, repeated Get, two adds, removal and CRT-registered destruction with a populated vector. Only heap APIs are executed boundaries. Fresh engine-manager-global closure:1407sources,zero duplicates,408unresolved. This build predates the final display header refinements below.

WorldUpdate434F60 is104bytes exact,31instructions. gate_native_display_engine_update.py passes2240cases/5320full4KiB-plus-three-global snapshots and one omission. Signed counter extremes, engine replacement, progress/string/about-to-run callbacks, and finite/zero/infinity/NaN/subnormal values at53/64-bit precision are covered. The fixture translates x87 control words to the distinct CRT _control87 flag encoding; writing raw x87 flags to that API incorrectly changed rounding. The oracle float callback uses immutable executable code and a separate value slot to avoid stale emulator translation. No state bytes are masked. StopProgress, wide assignment, SetAboutToRun and the virtual getter remain boundaries.

PDB donor evidence identifies GResetProgressTipText013B89D8, GResetProgressTipTitleText013B89DC and CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio013B8774. InitialFadeReadynessCount is signed long. Camera constructor parameter names are world_position,target_position,fov,is_2d. Camera candidate remains unaccepted at74% similarity. Display constructor regression4096cases/12288snapshots passes after these header changes. Session135addresses:115exact/20near. Fresh display-world-update closure is running. No frame, retail launch or commit. Next inspect SetAboutToRun434CD0 and its genuine global/renderer dependencies.

Fresh display-world-update closure finished1408sources/zero duplicates/412unresolved. Four new unresolved dependencies come from the actual update provider. SetAboutToRun434CD0 scratch is141bytes exact; donor PDB identifies GInitialFadeAllowedToBeActive01375CDC and final NProfileTimer::EndProfile(1). Added fade Update declaration and global declaration after that closure; behavior gate pending.


## Initial fade setup and timer accepted

SetAboutToRun434CD0 (141bytes,40instructions) and CFadeInFadeOutBase::Update434870 (95bytes,33instructions) are exact. Setup gate5120cases/11520full4KiB-plus-global snapshots covers every fill, lock/enable combination and update/profile callback mutations. Composed replay1024cases/2048snapshots executes the actual fade Update provider, with a separate omission proving it is required. Timer gate21296cases/full28-byte snapshots covers all flags and Cartesian timer/duration/elapsed edge cases at53/64-bit precision, including NaNs, infinities, subnormals and signed zero. No normalized bytes. The old naked byte-emitter source and print-only test for434870 were archived under work/parity_addtext/retired_fade_update_20261006 before replacing the production source. No assembly is counted.

Donor SetAboutToRun identifies GInitialFadeAllowedToBeActive at retail01375CDC and EndProfile(1) at009D8250. The retail enable byte is1; primitive fade-range ratio013B8774 is zero storage. These data dependencies are next. Session137addresses:117exact/20near. Fresh display-about closure finished1409sources/zero duplicates/413unresolved before timer promotion; another closure is required. No active verification jobs, frame, retail launch or commit.


## Fade settings and real progress-tip CRT globals

Native data definitions now supply GInitialFadeAllowedToBeActive01375CDC=true and CDisplayEngine::GamePrimitiveDefaultFadeRangeRatio013B8774=0. COFF storage and retail bytes are audited; no synthetic CRT worker. Both display-update and fade-setup gates now link the actual settings source, verify defaults before mutation, and fail link if it is omitted. Their full comparisons and actual-fade composition still pass. No additional function coverage is claimed for data. Fresh display-fade closure1411sources/zero duplicates/410unresolved.

Real CWideString GResetProgressTipText(L"") and GResetProgressTipTitleText(L"") now have separate native CRT initializers121A3E0/121A400, each27bytes exact, and registered destructors1228C40/1228C50, each10bytes exact. Both4-byte BSS globals at013B89D8/013B89DC and empty literals are audited. gate_native_progress_tip_global.py passes22full4KiB-plus-globals snapshots across four actual processes: empty, unique, shared and zero-reference storage. Actual string literal constructors, destructors and instance counter execute; CRT-registered reverse destruction and free/delete snapshots agree. Unexpected allocation during empty construction fails; five source omissions fail link. The two false generic tail-thunk providers/tests are archived. Session141addresses:121exact/20near. No frame, retail launch or commit. Latest closure predates these two globals.

Progress-stop forwarder9E9F20 is21bytes exact. gate_native_progress_stop.py passes6144cases/9216full256-byte-plus-owner snapshots, one omission. Null/live owner, all fills, both flags and callback owner mutation are covered. Replaced the unrelated Prog/g_prog placeholder with NProgressDisplay and actual shared interface; archived old source/test and stale symbol row. Session142addresses:122exact/20near. Progress-tip globals clean closure1413sources/zero duplicates/408unresolved predates this forwarder. Next unresolved PProgressDisplay013CAA38 duplicates the older boot g_FableProgressDisplay identity; recover its actual CRT lifetime and unify storage, preserving existing boot ownership tests.


## Progress owner storage unified with boot ABI

NProgressDisplay::PProgressDisplay013CAA38 is now the actual8-byte CCountedPointer<CAProgressDisplay> global. CRT initializer12241B0 is12bytes exact; atexit122A890 is57exact. The old SetProgressDisplay source no longer defines a competing legacy global. fable_boot.h gives the old two-pointer CCountedProgressDisplay view a linker alias to the genuine symbol; the concrete CAProgressDisplay base is at offset zero. No extra pointer storage or second owner. Native global gate checks both addresses are equal.

gate_native_progress_display_global.py passes33full4KiB-plus-owner snapshots across six actual CRT processes: null, references0/1/2/FFFFFFFF and destruction callback replacing both owner fields. Native StopProgress dispatch executes; actual CRT atexit cleanup rereads Info after the callback and zeros both fields. One callback address is identity-mapped; no state bytes masked. Two source omissions fail. Existing boot ownership composition32scenarios/32deletions passes with the actual global as a required fifth source. Legacy Set133bytes and Get28bytes remain exact, and both leaf behavior fixtures pass. Set leaf fixture now owns its isolated boundary storage, as Get already did. Bootstrap source/link lists include the actual global for composed runtimes; PowerShell parsing passes. Full authored bootstrap runtime was not launched or used for visual claims.

Refreshed Set symbol-index rows remove the old global definition. Fresh progress-display-global closure1415sources/zero duplicates/406unresolved. Session144addresses:124exact/20near. No active verification jobs, frame, retail launch or commit. Next native display engine initialization and camera construction; camera candidate remains below similarity threshold.


## Display InitEngine accepted

CDisplayEngine::InitEngine435190 is783/774bytes,218/212instructions,92.5581% similarity; stop tuning. gate_native_display_init_engine.py passes2304cases/62400full snapshots and one omission. Every snapshot contains4KiB object arena,100-byte local CEngineInit, seven logical globals,64-byte config,40-byte memory settings and debug-font slot. All128 options, allocation failure, initialise returns0/1/2, world null/live, editor on/off, reference0/1/2/FFFFFFFF and16 callback mutation profiles pass. Resource path ordering, captured registration argument across singleton retrieval, live reload for activation, definition lookup/ref release and all five path destructions agree. Engine allocation/initialization, engine-init constructor, paths/strings, progress, manager and definition calls remain recorded boundaries; no rendered frame claim.

Donor evidence is display_init_engine_donor.txt; retail and candidate instruction exports are alongside it. CEngineDef now has a shared typed0x80-byte layout derived from CParentDefClassBase, with retail fade ratio+50 independently checked againstB8FD40. Donor globals identify GStaticMapQuality, GEnableMinimumStaticMapRebuild, GAllowDataGeneration and GBuildRetailStaticMaps; declarations are in fable_engine_init.h. The local const-engine-def pointer destructor preserves retail release without the generic pointer's extra local clearing store; this specialization is confined to the new translation unit. The engine-init destructor inlines five actual member destructor calls. Definition-lookup fixture uses a proper C++ template specialization for its recorded boundary.

Session145addresses:124exact/21near. Fresh display-init-engine closure and combat-def helper regression are running after shared-header changes. Next recover CEngineInit constructor42D830 and remaining newly exposed startup providers. Camera constructor remains unaccepted. No frame, retail launch or commit.


## Engine-init object lifetime and native defaults

CEngineInit ctor42D830 is122bytes/40instructions at95% similarity; destructor42F7C1 is44bytes exact. gate_native_engine_init_lifetime.py passes3072cases/24576full256-byte-plus-count snapshots, plus256composed cases/512snapshots executing the real five wide constructors/destructors. All fills, ctor/dtor/combined paths and callback mutations of earlier/later members are covered. Constructor and counter are required; composed wide ctor/dtor have separate omission failures. Initial pointer stores precede strings; memory defaults and flags follow strings; StaticMapQuality isINT_MAX for the local object. These providers are registered, with no tuning of the passing constructor.

Native startup globals now define static map quality256, allow-data-generation=true, minimum-rebuild=false, retail-static-maps=false, and40-byte engine memory defaults (all integer-1, both float ratios-1). These differ intentionally from local CEngineInit's INT_MAX static-map default. Parent InitEngine gate links the actual47bytes of data, checks process-start defaults and retains2304/62400 passing comparisons; parent and settings omissions fail. Data has no CRT worker and adds no function coverage.

Added pe_oracle.read_virtual_data for loader-visible initialized data and BSS tails. va_to_off maps virtual addresses to arithmetic file offsets and must not be used as a BSS value reader:013B8649 otherwise appears as unrelated09/04bytes beyond .data raw storage, but the loader zeros it. Fade-range and progress-tip global audits now use virtual reads; their2240/5320 and22snapshot regressions pass. Existing code-byte offset behavior is unchanged.

Display-init-engine closure completed1416sources/zero duplicates/415unresolved; combat definition-helper regression passes24576comparative/12288compiled. Engine-init-defaults fresh closure is running after lifetime/data integration. Session147addresses:125exact/22near. No frame, retail launch or commit. Next typed resource-directory providers and GFAllocEngine, then camera construction.


## Engine resource paths and allocation

GetMeshBankDir41A1C0 and GetEngineCacheDir41A200 are17bytes exact; GetWriteableEngineCacheDir41A220 is99bytes exact, compiled with /GS. The320-case gate compares1504full256-byte plus UTF16 snapshots, including Unicode,259-code-unit paths, initialized buffers returned with failure status, concatenation and temporary cleanup. One source omission fails. The two false CUserProfileManager providers/tests were archived and index rows removed. Shared shell-path declaration leaves existing MyDocuments code and relocations unchanged.

CEngineInit destructor now emits a weak COMDAT consistently with its inline parent use, resolving the duplicate from the1418/409 closure. Lifetime and actual-string composition gates still pass. Fresh engine-directories closure1419sources/zero duplicates/406unresolved.

GFAllocEngineB26340 is27bytes/9instructions exact with two relocations. Its typed CEngine allocation is0x178bytes; donor size0x188 differs in vector storage. gate_native_allocate_engine.py passes3072cases/7680full1024-byte snapshots and one omission: all256fills, success/failure, two allocation offsets and constructor object/whole-arena mutation. Constructor remains a recorded boundary. The old false primitive-factory provider/test is archived. Shared CEngine fields follow donor types with retail offsets, but its virtual override declarations must be expanded and audited before constructor promotion. Session151addresses:129exact/22near. Fresh engine-allocation closure is running; next complete virtual-table audit and CEngine constructor. No frame, retail launch or commit.


## Engine constructor and complete virtual-table audit

CEngine ctorB260B0 is280bytes/60instructions exact, seven relocations. Full shared type preserves retail0x178 layout and member initialization order. Its86-slot primary table and2-slot device interface are audited by matching each donor target with its PDB method; the compiler also emits85-slot CIEngine base table, now audited too. Five inherited entries remain at slots1,17,18,50,51. The checked-in native_engine_vtables.json records173 symbol/target/signature entries; constructor gate verifies COFF table contents and every retail target. CIEngine constructor initializes Active=false; preload stats and video options inline constructors reproduce retail defaults.

gate_native_engine_ctor.py passes2048direct cases/6144full4KiB snapshots plus4096cases/12288snapshots through actual GFAllocEngine. All256fills and eight base/pool callback mutation profiles are compared; allocation failure bypasses construction. Three source omissions fail. Only two known vtable pointer identities map to retail; base/pool calls remain recorded boundaries and virtual method bodies are not claimed. Allocator regression3072/7680, display constructor4096/12288 and parent InitEngine2304/62400 pass after shared-header changes. Session152addresses:130exact/22near.

Clean allocation closure1420/0/406; clean engine-constructor closure1421/0/512. The increase is106 newly exposed native engine/interface dependencies from actual virtual tables, not a compile failure. No verification jobs remain. Next unresolved typed CIEngine defaults and CEngine methods; no frame, retail launch or commit.


## Typed engine interface defaults

Twenty CIEngine methods now use the shared actual interface instead of unrelated local classes, stdcall wrappers or ABI-mismatched signatures. All are exact. gate_native_iengine_defaults.py passes20480cases/full1024-byte snapshots, one omission: all256byte fills and four argument patterns, active-state transitions and reads, scalar return widths, hidden16-byte colour result placement, and preservation of surrounding bytes. Empty reference-parameter methods use opaque ABI words without dereferencing them; no callback/mask/normalization. The old20sources/tests and index rows were archived, and the permanent source/spec/gate registered. Clean defaults closure1422sources/zero duplicates/492unresolved, down20. Session172addresses:150exact/22near. CIEngine Initialise,Release,SetSeaBodyToRender and scalar cleanup are exact scratch candidates; next behavior gates. No frame, launch or commit.


## Engine interface lifetime and concrete teardown

CIEngine InitialiseB232D0(8bytes),ReleaseB232E0(11),SetSeaBodyToRenderB23610(12),scalar deleteB23770(36) are exact. gate_native_iengine_lifetime.py compares32768cases/70656full512-byte snapshots, one omission: all byte fills, init return forwarding, nullable release through boundary or actual scalar body, flags0/1/2/FFFFFFFF, by-value string destruction and pointee mutations. Inline CIEngine destructor supplies genuine weak linkage. Base init/destruction, character cleanup and heap boundaries remain recorded. Clean closure1423/0/488.

CEngine destructorB261D0 is223/238bytes,68/72instructions,92.8571percent; scalarB2AB10 is30exact. No further tuning. The destructor calls the genuine out-of-line pool destructor rather than inlining it as retail does; gate composes the actual pool destructor and ClearMemoryBlocks. gate_native_engine_dtor.py passes3200cases/24400full4KiB snapshots, two omissions. Null/independent/shared six definition owners, refs0/1/2/FFFFFFFF, pointer-vector and camera buffers, owned/borrowed pool chains, cached next-block pointers, mutating callbacks and scalar flags agree. Only four known vtable identities map; no state bytes masked. Free boundary renaming is proven code/relocation-identical. Uninitialise, heap operations, base destructors and virtual block/definition delete callbacks remain boundaries. Retired false landscape scalar provider/test. Session178addresses:155exact/23near.

WorldFramesPerSecond corrected from float to unsigned long: PDB baseType14 plus retail unsigned fild/fadd conversion prove it. Constructor2048/6144 plus allocation composition4096/12288 and allocator3072/7680 regressions pass. Clean teardown closure1424sources/zero duplicates/487unresolved.

Next CEngine::UninitialiseB25D80 scratch body653bytes,191/192instructions,99.2167percent already passes similarity; stop tuning and add behavior comparison. Donor namespace identities identify four NEngineInternal globals. New typed component/layer interfaces, internal global declarations, vector pop_back and inline D3D texture binding declaration are pending a subsequent clean closure. Read-only evidence engine_uninitialise_decomp.c and engine_uninitialise_donor.txt. No active tool jobs, frame, retail launch or commit.


## Concrete engine shutdown and world-frame updates

CEngine::UninitialiseB25D80 is653bytes,191/192instructions,99.2167percent. gate_native_engine_uninitialise.py passes512cases/3008snapshots containing the entire36KiB arena (engine, two renderer objects and resources) plus four internal globals. Init on/off, all16texture masks,0/1/3/5 resource entries including nulls, global replacement during GPU callbacks, signed texture-stage limits and mutable lists pass. Reverse component pop/delete, forward surface deletion, layer slot clearing, both global resets and final initialization flag order agree. Recorded boundaries remain device-reset deregistration, GPU methods, virtual resource deletion and base Uninitialise; no state normalization/masking.

NEngineInternal GPRenderManager1436E18,GPDisplayManager1436E1C,GPPrimitiveRendererSharedData1436E78,GPVideoOptions1436EB0 now have actual16-byte BSS storage. Donor PDB namespace/type identities and retail references agree; COFF and loader-visible values plus actual fixture startup verify zero defaults. No CRT worker. Parent and data omissions fail. New CEngineComponent13-slot typed interface, layer type, vector pop_back and inline AttachD3DTextureToStage support genuine engine shutdown. Clean closure1426sources/zero duplicates/486unresolved.

WorldFrameUpdateB24030(143bytes) and PausedWorldFrameUpdateB240C0(140bytes) are exact. gate_native_engine_world_frames.py passes14088cases/22792full2KiB snapshots:12288dispatch and1800arithmetic cases. Initialization/render guards,0..3components, flags, live masks, replacing current component pointers during PeekSceneFilterFlags, callback changes to clocks/counters/rate/list ends all pass. Counters and times advance even when dispatch is disabled. Nine unsigned rates include zero,16777217 and signed-boundary/FFFFFFFF values; double signed-zero/finite/subnormal/infinity/qNaN/sNaN and counter wrapping pass at53/64-bit x87 precision. Component virtuals are boundaries. One source omission fails. Session181addresses:157exact/24near. Fresh world-frame closure is running; next engine component/renderer dependencies. No frame, retail launch or commit.


## Engine scene controls, backend defaults and component ownership

World-frame clean closure1427/0/484 completed. Seven scene controls atB239A0,B23A10,B23A70,B23F90,B23FC0,B23FD0,B24000 are exact. gate_native_engine_scene_controls.py passes3840sequences/38400full512-byte snapshots: ten-step sequences, scene/cutscene guards, both boolean arguments, all float bit patterns including NaNs/infinities/subnormals and independent3D/2D mask bits. Concrete ReleaseB23990 is11bytes exact; gate_native_engine_dtor.py now4480cases/30304full4KiB snapshots and two omissions includes null/live release through actual virtual scalar cleanup. Scene-release clean closure1428/0/476.

Fourteen genuine retail PC backend defaults are exact: conversion/static-map/activation/debug and render-target methods, both device-change callbacks and pass-count return. gate_native_engine_backend_defaults.py passes14336cases/full1024-byte snapshots, one omission; all256fills, four argument patterns and active flag only writes. Old false/ABI-mismatched providers retired. Backend closure1429/0/462.

CEngineComponent ctorB59710,dtorB59730,scalarB59780,SetGlobalComponentPointerB59750,GetConsoleEnableFunctionNameB28CA0 and ten default virtuals are exact.13-slot vtable audited against donor PDB and retail targets. gate_native_engine_component.py passes19968cases/47616full1024-byte snapshots, one omission: all256fills, external/null/self-alias ownership links, setter without pointee dereference, scalar flags0/1/2/FFFFFFFF, padding, hidden empty-string result and mutating boundary callbacks. Only known vtable identity mapped; character construction and heap deletion remain boundaries. Promotion prep accidentally archived the new provider as well as old sources; verified its bytes against scratch SHA256 e7f9efe8cb752eb3903fc402c5ec7d5629c13e17a2b14edcd1ef3858eefcdbe1, restored it and completed registration. Failed missing-source closure is not validation. Fresh native-gfmain-engine-component-restored-clean-20261006 completed1430sources/zero duplicates/462unresolved; component was explicitly seeded pending future derived-component dependencies. Session218addresses:194exact/24near.

## Class-local engine-definition lookup

GetDef<CEngineDef>00430096 is101bytes/43instructions exact with one lookup relocation. Existing class-name/index template and PDB signature correct the manifest's false CEnvironmentDef identity. gate_native_get_engine_def.py executes the compiled provider against retail:18176cases/39912full2048-byte snapshots, one omission. All256fills, negative/zero/positive signed indices, null lookup/output when legal, shared/distinct/null owners, reference counts0/1/2/FFFFFFFF, delete callback mutation of temporary pointer/output/counts. Callback snapshots capture the hidden result contents; no arena normalization/masking. Lookup and virtual pointee deletion remain recorded boundaries. Callee-saved registers and stack cleanup checked in retail execution. Registered source, fixture, gate, corrections and symbol catalog. Session219addresses:195exact/24near. Fresh engine-definition closure running. No recreation frame, retail launch or commit.
