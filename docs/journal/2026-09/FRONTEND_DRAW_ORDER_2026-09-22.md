# Frontend draw-order recovery, 2026-09-22

Worktree: `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
This increment ran entirely offline. No screen/game launch or shared-checkout edits.

## Readable native primitive insertion

Recovered `CEngineInternalPrimitiveBase::AddToList` at 00B8FDF0 as ordinary C++ in
`rebuild/src/compiled/00/b8/CEngineInternalPrimitiveBase_AddToList_00b8fdf0.cpp`.
It uses the existing generated engine header and original member names:
RenderLayerMask, NextPrimitive, RefPrimitive, NextLayerMask, RefLayerMask.
The public adapter uses ECX for the receiver, ignores EDX, and pops one stack
argument, matching the native thiscall boundary without editing generated headers.

The first primitive with a new mask becomes the head/group representative.
A later member of that group is inserted immediately after its representative.
Both intrusive chains and their pointer-to-link backlinks are maintained.
No allocation, ownership transfer or draw calls occur in this routine.

`python tools/decomp_pipeline/check_primitive_list.py` passes 1,221 cases against
the complete retail routine without dependency doubles. The grid exhausts mask
sequences of lengths 0..6 over three masks, plus 128 longer cases up to 64 nodes.
It compares the head and all four link fields on every node, including dirty
initial link storage. Compiled size is 106 bytes, retail size 106 bytes; register
allocation/instruction scheduling differ. Status is functional `DIFFER`, not a
byte-parity claim. `--require-byte-match` correctly returns failure while behavior
still passes. Reports/assembly are in `work/primitive_list_check/`.

The manifest records PASS/PASS/DIFFER and bootstrap invokes the behavior gate.
The presenter does not yet submit its flat quads through this native list, so
this increment does not claim a visible change or full renderer integration.

## The normal 2D list requests stable sorting

The previous equal-key instability probe covered SortList's false-flag path.
It must not be interpreted as the frontend's default sorting behavior.

Retail constructor 00B84470 belongs to CEngineSubPrimitiveRenderer (its primary
and secondary vtables are 012A3A60/012A3A28). The manifest's propagated
NHeroInformationScreens::CBase owner at that address was wrong; the manifest and
function override now record the evidence-backed owner. It initializes
15 lists at +18, stride 18 hex. Donor CSubPrimitiveList names resolve the fields:
Elements, RenderPass, PreRenderPass, BoundingVolumePass, RequiresStableSort,
Sorted, Enable. Retail omits the donor vector's extra allocator word.

All lists default to sorted/enabled, with RequiresStableSort false. Constructor
stores at +5D and +75 explicitly enable stable sorting for types 2 and 3.
Normal 2D rendering calls RenderType(2,true), whose per-list setup forwards
RequiresStableSort to SortList. Thus the default normal 2D path uses stable sort.
Runtime overrides remain to be audited.

`python tools/decomp_pipeline/probe_frontend_draw_order.py` now validates:

- Three primitive insertion/traversal examples with no doubles.
- Ten false-flag small-list examples with no doubles (retained as other-path evidence).
- Eighteen true-flag stable-sort examples, equal and mixed keys, sizes 1..512.
  Only malloc/free are doubled; native sort/merge instructions execute. Results
  preserve the order of equal-key elements and match stable ascending key order.
- All 15 constructor list defaults. Two unrelated base constructors are explicit
  no-op doubles; engine global storage is supplied. Native list setup executes.

This probe is evidence, not a reconstructed-sort parity gate. False-flag radix
sorting and allocation-failure behavior are not tested.

## Frontend submission path and next work

The native draw coordinator 0042DF9E supplies a null parent primitive handle to
CFrontEndManager::Draw 00595222. That routine traverses a tree using 004292C0
and calls component slot +8 with engine, handle, layer 0, null index, null parent.
The tree uses parent +4, left +8, right +C; 004292C0 is an in-order successor.
Do not mistake +8 for a rightmost pointer or reverse the draw sequence.

For ordinary CSprite::Draw submissions, a null handle/index takes engine slot
+5C. Engine vtable 012A0F3C resolves it to SetPersistentPrimitive 00B23BC0,
which calls 00B324A0. The alternate supplied-handle/index path uses slot +70,
SetChildPrimitive 00B23C50 -> 00B31F70 -> internal primitive virtual slot +20.
The latter is not automatically the path for all frontend artwork.

Persistent collection is now traced: 00B324A0's new-object branch tests flags
bit 0x40 and calls 00B4A670 -> 00B8FDF0 for a 2D primitive. Its same-type reuse
branch calls primitive Update and updates status flags; it does not reinsert
the primitive. The 0xC0 supplied by CSprite::Draw is a flag word, not a descriptor
byte size. This distinction matters when recovering the submission interface.

The offline probe now creates three persistent sprites in order 0,1,2 and
confirms their real native list is 0,2,1. Updating them in order 1,0,2 leaves
every list link unchanged. It executes SetPersistentPrimitive, AddPrimitive
and AddToList; manager factory and primitive Update are explicit doubles.
The fixture uses type 0x23, flags 0xC0 and grid level -1 (no spatial update).
Assertions check distinct handles and the expected initial chain, so an empty
or broken insertion cannot pass merely by remaining unchanged. Changing the
factory stub requires invalidating Unicorn's translated code cache.

Equal-depth ordering can therefore depend on primitive creation history. The
presenter's outgoing-then-incoming quads do not establish that history. Next
recover sprite creation/culling and release across page/state changes, plus
the frontend manager's layer setup. CManager::SetMetaLayer 0041E1CD maps UI
layers -3..13 to engine layers 64..81 when meta-layer is zero, skipping engine
layer 72 between UI layers 4 and 5. The other meta-layer mapping and actual
selected meta-layer still need confirmation. Keep current presenter compositing
until submission order and depth mapping are both established.

Bootstrap PowerShell syntax and `git diff --check` pass. No full bootstrap rebuild
was attempted for this offline dependency increment.
