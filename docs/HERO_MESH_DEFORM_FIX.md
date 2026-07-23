# Hero-mesh (Banjo) deform bug — root cause + fix

*From engine decomp analysis 2026-07-21 (`hero_mesh_decomp.txt`, 96 fns). The byte
format is NOT the bug.*

## Ruled out (byte-identical to EgoCore's proven meshes — do NOT touch)
- **Packed-normal bit layout** — engine never CPU-decodes normals; it builds a D3D9
  vertex declaration (`CVertexShaderInput{Static,Animated}::Initialise` 0x00a90560/
  0x00a90630) and the GPU decodes the packed dword. Engine picks packed normal (typeCode
  0xc) exactly when InitFlags&4 (Banjo's 0x14 sets it). `mesh_rw.pack_normal` == EgoCore
  PackNormal byte-for-byte. Match.
- **Weight/bone-index bytes** — 4 joint bytes + 4 weight bytes after position, joint =
  paletteIndex*3, weights /255 renormalized. Matches `mesh_rw._skin_bytes`. Match.
- **Palette local-vs-global** — undecidable from decomp BUT can't be the cause: the build
  **clones the donor hero skeleton byte-exact** (`build_banjo_animated.py:292/298`,
  indices re-emitted verbatim), so the local->global map is identical to retail regardless.
- Engine only validates that the swapped mesh is a **type-5 entry** (`ReplaceMeshByIndex`
  @0x007a69f0 + CreatePPhysicsMeshInfo on graphic[0xd]==5). Nothing else is checked.

## ROOT CAUSE (authoring-space weighting error in `build_banjo_animated.py`)
1. **(high) Head-only + single-uniform-scale fit ignores bind orientation.** The Banjo geo
   is fit using only bone HEAD positions + one global scale (`:79-151`), then ARMATURE_AUTO
   bone-heat weights it in that rest pose (`:202`). Each bone's bind *orientation* and
   per-bone scale are never consulted, so vertices weight to bones whose bind-frame axes
   don't match Banjo's local geometry -> any animated rotation twists/collapses the mesh
   even though the file round-trips perfectly.
2. **(high) WEAPON_FOCUS/WEAPON_SCABBARD deform disabled before bone-heat** (`:184-193`) ->
   no Banjo vertices weight to the weapon bone -> **"sword doesn't track the hand"** (the
   attached weapon expects deform on a zero-weight bone).

## Fix (edit ONLY the build script, never mesh_rw.py)
1. Remove `WEAPON_FOCUS`, `WEAPON_SCABBARD` from the NONDEFORM tuple (`:184-193`); re-run;
   confirm via the bone_use histogram (`:242-252`) that WEAPON_FOCUS now carries weight.
2. Replace the head-only/uniform-scale fit (`:79-151`) with a **bind-aware fit**: place/
   orient geometry using each dominant bone's FULL rest matrix (head AND tail direction),
   not just `b.head_local`; drive ARMATURE_AUTO from a mesh whose limbs align to bone axes.
3. Harden the ANATOMY spot-check (`:254-263`) into a hard assert (upper-outer verts -> ARM
   bones, lower verts -> LEG bones above a threshold) so an orientation-scrambled fit fails
   the build instead of shipping.

## Validation (no in-game run)
One-bone skin test: build a trivial mesh 100% weighted to one non-root deform bone (e.g.
`Bip01 R Hand`), compose with the cloned hero skel, apply the base pose in a Python harness
(`CalcLocalBoneTransformationRelative` semantics: base TRS from C3DMesh2+0xa0 stride 0x30),
and confirm the mesh follows THAT bone. Validates the slot mapping end-to-end before a full run.

## Still open
Is the in-game symptom a whole-body scramble (-> fit/orientation, fix #2) or specifically the
weapon (-> WEAPON_FOCUS, fix #1)? The exact symptom picks the primary fix. Draw-time palette
convention (CalcMeshTransformations / CVertexBufferWin32 skin path) not in this closure; only
matters if you ever author a NON-cloned skeleton.
