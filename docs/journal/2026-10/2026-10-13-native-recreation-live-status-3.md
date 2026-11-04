# Native recreation live status 3: frame-loading boundaries

Continuation is active; no Lua. The two validated drawing parents remain
work-only and their frozen packets are unchanged.

## GetFrame is functional evidence, not accepted reconstruction

`CGraphicInfo::GetFrame 009FF8D0` first source is 599B/84.382% versus 578B retail.
The separate source amendment follows the original shared BeginLoading/forced
loading/output tail and emits 547B/89.975550%. It still misses the 90% gate;
neither source is accepted. No tuning to cross the remaining tiny score gap.

Bounded x86 original/compiled comparison passes **32,736 cases / 217,938 full
64KiB state snapshots**, callback events, boolean return, callee-saved registers
and RET16 stack balance. Genuine resource-list remove/insert and graphic-frame
uninitialise bodies execute. Allocation, frame construction, BeginLoading,
forced loading, virtual deletion and control-block deletion are bounded
interfaces. This is emulation, not native execution or actual asset loading.

The nominal independent branch model includes sign extension of signed mip
bytes before unsigned32 comparison. Cases cover absent/unloaded/loaded frame
data, high and negative mip values, drawing/loading flags, output null/owned/
source-member aliases, resource lists, forced-loading responses, null allocation,
and callbacks changing the current data owner, output frame, pending file data
and cleanup targets. Source fitting stopped with the gate still false.

Packet: `work/parity_addtext/native_graphic_info_getframe_WORK_ONLY_functional_NOT_ACCEPTED_20261013.json`,
SHA `9ad694dcdfff2ce62b2915be16610363c92e769196737a401ebcaf283c0d9f6f`, 1725 controls.

## Exact coordinator and correctly named unloadable provider

`CGraphicInfoFrame::InternalGetFrameForcedLoading 009FF6B0` first genuine C++
compile emits exactly 82 bytes, matching the full retail body after its five
typed call relocations: IsFinished, WaitUntilFinished, RemoveFromResourceList,
UploadFrame and SetAsUnloadable. The routine checks file presence, optionally
waits (ignoring the wait's boolean), removes the current resource, uploads with
true, and optionally marks the *then-current* owner unloadable. It rereads the
frame's data pointer after each callback.

The new correctly named `CResource::SetAsUnloadable 009FBF50` emits all 17 original
bytes without relocations. Both donors corroborate the real owner/name. It
subtracts the resource-list unload delay from its current frame with original
unsigned32 wrap. The old `CResource_Methods::M` facade and its index row remain
unchanged; this work confers no identity credit or canonical publication to it.

A coherent work-only copy of the include graph adds just the required original
method declarations/friend access. Canonical headers and their hashes remain
unchanged. No claim that the pending shared-header audit has closed.

Native comparison passes **73,728 cases / 147,456 full 8192-byte snapshots**, all
callback events, return value and nominal control/last-used-frame invariants.
The genuine coordinator, 17B unloadable provider and original 16B resource
removal wrapper execute. IsFinished, WaitUntilFinished, resource-list removal
and UploadFrame are fixture interfaces; no actual thread wait, loader or GPU.
Both exact original-root omission links fail and the full control links.

Cases include two data-owner placements, file/list nullability, both completion
responses, ignored false waits, upload success/failure, all sixteen callback
owner-swap combinations, high drawing flags and unsigned frame/delay wrapping.
The initial original invariant stop at case 18432 is preserved. A diagnostic
fixture amendment retains the same stop and exposes its cause: the mock's
global `Finished` control was shadowed by the actual class member. A separate
fixture-only rename to `FixtureFinished` passes. Neither provider was recompiled
or changed for this correction.

Packet: `work/parity_addtext/native_forced_frame_loading_WORK_ONLY_validated_20261013.json`,
SHA `f7db875b9ccc3f93389e2eb31e1e8338201683320d1c93116829fa01ef0892c2`, 3408 controls.

All recovery remains work-only. GetFrame's near gate is explicitly false, and
UploadFrame's capped LoadGraphicFrame child remains unrecovered. No authored
production loader substitute, canonical promotion, dashboard refresh, game/GPU
launch, commit, push or Lua work occurred. Shared-find acceptance remains 980.
