# Foliage / local-detail scenery — recovered save grammar (2026-08-22)

## 2026-08-22 stage34 writer checkpoint

The first donor-free writer is now implemented in FableForge. It deliberately
uses primitive type 0 only: one fixed 92-byte `CLocalDetailPrimitiveMesh` per
placement. This avoids guessing any type-1 optional-array semantics.

Recovered container output is now covered by a deterministic unit test:

- one `CObjectCacheGroupCollection` header in the root file block;
- four absent child markers;
- standard framed LZO contents;
- `CObjectTypeCollection` header and palette index;
- fixed type-0 records; and
- runtime-mode 0x40-byte palette records (three resource IDs plus the raw
  0x34-byte settings block).

`ForgeTest64_terrain_v12.chunk.bin` contains 217 placements (grass plus six
trees), is packaged as `stage34_foliage_runtime_package`, and validates CLEAN.
Runtime status: deployed and launched, awaiting the interactive Oakvale-save
trigger. Rollback set: `20260822_164209`.

Fable's grass, bracken and trees are **not** TNG props. They are a procedural,
theme-keyed generator whose baked output lives in the STB chunk's **local-detail**
section: an object-type palette plus per-quadtree-node instance batches. Our
donor-free chunk currently emits a deliberately empty local-detail section, which
is why ForgeTest64 has no foliage.

## Source

Recovered from the symbol-bearing debug build `D:\ForgeBake\gameroot\FableWin.exe`
(image base 0x400000, `.text` at 0x0178F000) against the 80,060-symbol dump
`D:\ftpub\ghidra_out\fablewin_editor_symbols.tsv`. It is an unoptimised build, so
the bodies read almost like source. Retail `Fable.exe` implements the same format
(its palette record loader is `CLocalDetailObjectCollectionType::Load @0x00BE27B0`).

Vtable slots below are reached through incremental-link thunks (`jmp` stubs); the
targets listed are the real bodies.

## Function map

| Function | FableWin VA |
|---|---|
| `CLocalDetailCacheMap::GenerateStaticMapEntry` | `0x02E3BF00` |
| `CObjectTypeCollectionPalette::Save` | `0x02E3BCD0` |
| `CObjectTypeCollectionPalette::Load` | `0x02E36BE0` |
| `CObjectTypeCollection::Save` | `0x02E3BB30` |
| `CObjectTypeCollection::Load` | `0x02E35E70` |
| `CObjectCacheGroupCollection::SaveHeader` | `0x02E3D5B0` |
| `CObjectCacheGroupCollection::SaveContents` | `0x02E3D850` |
| `CObjectCacheGroupCollection::GetSaveSize` | `0x02E3D7A0` |
| `CObjectCacheGroupCollection::LoadHeader` | `0x02E370D0` |
| `CObjectCacheGroupCollection::LoadContents` | `0x02E37430` |
| `CQuadTreeElement::SaveTree` | `0x02E3E890` |
| `CQuadTreeElement::SaveHeader` | `0x02E3E680` |
| `CQuadTreeElement::SaveFileBlock` | `0x02E3EB20` |
| `CQuadTreeElement::SaveSubFileBlocks` | `0x02E3EFE0` |
| `CQuadTreeElement::AssignFileBlocks` | `0x02E3F230` |
| `CObjectTypeCollection::CreatePrimitiveOfType` | `0x02E36420` |

## `CObjectTypeCollection::Save(CDataOutputStream&, CObjectTypeCollectionPalette&)`

```
u8    field_0x18                       // collection flag
s32   paletteIndex                     // palette.GetIndexOfType(this->type)
s32   primitiveCount                   // length of the linked list at this+0x08
repeat primitiveCount:
    s32  primitiveType                 // virtual GetType(), vtable +0x24
    <primitive payload>                // virtual Save(out), vtable +0x28
```

The primitive list is singly linked through `+0x04`.

## `LOCAL_DETAIL_PRIMITIVE_TYPE`

`CreatePrimitiveOfType` switches on 0/1/2 and each class's `GetType()` confirms it:

| value | class | instance size |
|---|---|---|
| 0 | `CLocalDetailPrimitiveMesh` | 0x78 |
| 1 | `CLocalDetailPrimitiveRepeatedMesh` | - |
| 2 | `CLocalDetailPrimitiveMeshZSpriteBatch` | - |

These line up with the `class` column of `forge foliage palette`
(`grass` / `near` / `z-sprite`).

## Type 0 — `CLocalDetailPrimitiveMesh::Save` (`0x02EDF6C0`)

Fixed 92 bytes, no counts:

```
WriteBytes(this+0x08, 0x18)   // 24B  = 6 floats, AABB
WriteBytes(this+0x20, 0x10)   // 16B  = 4 floats, bounding sphere
WriteBytes(this+0x38, 0x30)   // 48B  = 12 floats, 4x3 transform
WriteFloat(this+0x68)         //  4B
```

## Type 1 — `CLocalDetailPrimitiveRepeatedMesh::Save` (`0x02EE1D70`)

This is where the instance batches live. The body first asserts that
`+0x48`, `+0x4C` and `+0x3C` are all non-zero (the assert path builds a message
and is not taken for real data); the live path begins at `0x02EE1E83`:

```
WriteBytes(this+0x08, 0x18)              // 24B AABB
WriteBytes(this+0x20, 0x10)              // 16B bounding sphere
Write(s32  count = [this+0x3C])          // instance count
WriteFloat([this+0x38])
WriteBytes([this+0x48], count * 16)      // instance array A (16B/instance)
WriteBytes([this+0x4C], count * 16)      // instance array B (16B/instance)

EBOOL present = ([this+0x54] != 0)
if present: WriteBytes([this+0x54], ((count + 3) & ~3) * 12)   // count PADDED to 4

EBOOL present = ([this+0x50] != 0)
if present: WriteBytes([this+0x50], count)                     // count BYTES

EBOOL present = ([this+0x44] != 0)
if present: ...                                                // continues at 0x02EE1F63
```

Every optional array is written as `[EBOOL present][payload]`, so a minimal
authored batch can emit `present = 0` for all three and still parse.

Note the `(count + 3) & ~3` padding on the +0x54 array — a writer that packs it
tightly will desynchronise the reader.

## Still to recover

1. The tail of type 1 past `0x02EE1F63` (the `+0x44` optional array).
2. `CLocalDetailPrimitiveMeshZSpriteBatch::Save` (`0x02EE2420`), type 2.
3. `CObjectCacheGroupCollection::SaveHeader` / `SaveContents` — the per-node
   grouping that wraps these collections, and how `GetSaveSize` is used to lay
   out file blocks.
4. The field semantics of the two 16-byte instance arrays. Size and count
   strongly suggest position+scale and orientation, but that is inference, not
   yet proven — decode a retail batch and check the values against known
   in-world foliage before trusting it.

`forge foliage instances` currently reports 5845 instances for retail Darkwood_3
but with `bound to a type: 0` and an implausible world extent
(`X[-3..2563] Y[-4..75]`), so the existing reader is heuristic and must not be
inverted into a writer as-is. Rebuild it on the grammar above first.

## Authoring path once complete

1. Emit a non-empty `CObjectTypeCollectionPalette` (64-byte records; grammar
   already recovered — see `forge::foliage`).
2. For each populated quadtree node, emit `CObjectTypeCollection` batches
   referencing palette indices, with type-1 instance arrays scattered over the
   authored heightfield.
3. Wire the node file blocks through `AssignFileBlocks` / `SaveFileBlock`
   ordering, replacing `buildEmptyLocalDetailSection`.

---

# Continuation notes (2026-08-22, later session)

## RETIRED STRATEGY: relabel grass and author it as type 0

`CLocalDetailPrimitiveMesh` (type 0) is **fully recovered and has no unknowns**:
a fixed 92-byte payload with no counts and no optional arrays, carrying a single
4x3 transform. One primitive = one placed object.

So a first foliage writer does **not** need the type-1 instance arrays at all.
Scatter N plants as N type-0 primitives inside a `CObjectTypeCollection`. It is
less compact than retail's batching, but every byte is specified:

```
per primitive:
  s32  type = 0
  24B  AABB          (6 floats: minX,minY,minZ,maxX,maxY,maxZ)
  16B  bounding sphere (4 floats: centre xyz + radius)
  48B  4x3 transform (12 floats)  <- placement
   4B  float         (this+0x68)
```

This is valid only for palette entries whose native primitive class is `NearMesh`
(type 0). Stages 34-41 incorrectly changed retail grass slot 0's primitive
discriminator from repeated mesh (type 1) to near mesh (type 0). The stream could
then pass the loader's exact-type check, but that does not make the grass renderer
consume a type-0 object. No grass appeared in any of those runs.

The debug type layout also names the formerly unknown arrays:

```
+0x38 float       MaxObjectScale
+0x3c long        ObjectCount
+0x48 C4DVector*  ObjectMatricies
+0x4c C4DVector*  ObjectOffsets
+0x50 uchar*      WindDelayArray
+0x54 float*      LandscapeNormalArray
```

Stage42 uses genuine retail type-0 entries (oak stump and two sapling families)
as a structural proof. Actual grass still requires a real type-1 writer; do not
restore the discriminator override.

## Stage44-45 correction: real repeated-mesh grass writer

The stage44 runtime hook at retail `CObjectTypeCollection::Load @ 0x00BDE680`
proved collection streams parse successfully and expose non-null primitive lists.
The decisive retail batch dump then established the paired type-1 arrays exactly:

```
array A = (cos(rotation)*scale, sin(rotation)*scale, 0, 0)
array B = (worldX, worldY, worldZ, scale)
```

For every sampled retail object, `length(A.xy) == B.w`. Grass scale is around
`0.01`, not the `0.75..1.25` transform scale used by the failed type-0 experiment.

Stage45 emits one native type-1 primitive for palette slot 0 with 300 instances,
uses primitive mask `2`, and writes the three optional-array presence bytes as
zero. The corrected offline scanner reports all 300 instances bound to type 0
(`Grass blades (var 03)`), world extent X 3329..3391 / Y 2306..2367 / Z 72..84,
with zero unbound records.

The scanner itself had a separate alignment defect: after a compressed frame
whose length was not divisible by four, it resumed at that unaligned modulus and
missed every later frame. It now rounds the next scan position to four-byte
alignment; authored local detail is consequently found after the terrain frames.

## `CObjectCacheGroupCollection::SaveHeader` (`0x02E3D5B0`) — partial

Signature `SaveHeader(CDataOutputStream&, bool)`. The second arg (`[ebp+0xC]`)
gates a validation block: when true it asserts

```
[this+0x28] > 0  &&  [this+0x2C] > 0  &&  [this+0x30] >= 0  &&  [this+0x30] < [this+0x2C]
```

and on failure builds an assert message (strings at `0x44709B0` / `0x4470251` /
`0x44709F0`, line `0x6D4`) rather than writing. The real write path begins at
`0x02E3D6B9` and was NOT yet disassembled — that is the next thing to read.

Fields implicated: `+0x28` and `+0x2C` are counts/dimensions, `+0x30` is an index
bounded by `+0x2C`.

## Remaining unknowns (unchanged)

1. `CObjectCacheGroupCollection::SaveHeader` write path from `0x02E3D6B9`.
2. `CObjectCacheGroupCollection::SaveContents` (`0x02E3D850`) and
   `GetSaveSize` (`0x02E3D7A0`) — the latter drives file-block sizing.
3. Type-1 tail past `0x02EE1F63`; type-2 `Save` (`0x02EE2420`).
4. Semantics of the two 16-byte type-1 instance arrays. **Do not guess these.**

## Disassembly recipe

`FableWin.exe` is an unoptimised debug build with an incremental-link thunk table:
vtable slots point at `jmp` stubs, so follow one level to reach the real body.
Scratch tooling used this session:

- `wdis.py <exe> <va> <len>` - capstone disassembler that resolves VA via the PE
  section table (works for any of the three binaries).
- `vt.py <vtableVA>` - prints vtable slots 9/10 (GetType / Save) and follows the
  thunks.

Symbol lookup: `D:\ftpub\ghidra_out\fablewin_editor_symbols.tsv`
(tab-separated: group, hex VA, mangled name).

---

# Session 3 (2026-08-22, late) — subsection table SOLVED, manager RE'd, stages 46-49 recorded

Sources: FableWin.exe debug build + retail Fable.exe + 1,591 retail type-1 records across
Darkwood_3 and StartOakValeWest. Raw agent output:
`work/no_donor_terrain_pack/RE_NOTES_20260822_subsections_manager_palette.md`.
Companion diagnosis: `docs/modding/README.md#forgetest64_dark_terrain_and_foliage`.

## Stage 46-49 history (was undocumented)

| stage | change | outcome |
|---|---|---|
| 46 | group bounding sphere: C2DBounds quad -> real world sphere `(3359.957, 2336.080, 78.981, r=43.851)` | **WORKED — this is what made the collection load. Keep it.** |
| 47, 48 | float `22.0f` (`0x41B00000`) at group-header disk `+0x20` | **HARD CRASH.** `+0x20` is an INTEGER band index (see below). Rolled back to stage46. |
| 49 | int mask restored, 240 instances, 1-lane subsection table, LandscapeNormalArray `(0,0,1)` AoS, WindDelayArray zeros | Loads (`stream=4->10955`), still no visible grass. This is what the user's screenshot shows. |

There is **no FSE log for stages 46-48** — the log is single-attach with no rotation, so each run
destroys the previous one. Their outcomes were reconstructed from WER events and backup hashes.
Preserve the log into `runtime_evidence\<stage>\` before every relaunch.

## The type-1 record, authoritatively (writer `0x02EE1D70`, reader retail `0x00BFA2A0`)

```
WriteBytes(this+0x08, 0x18)      // f32 aabb[6]        C3DBoundingBox
WriteBytes(this+0x20, 0x10)      // f32 sphere[4]      C3DBoundingSphere
WriteSLONG(ObjectCount)          // i32  +0x3C
WriteFloat(MaxObjectScale)       // f32  +0x38
WriteBytes(ObjectMatricies, ObjectCount*16)   // A[]  +0x48
WriteBytes(ObjectOffsets,   ObjectCount*16)   // B[]  +0x4C
if (+0x54) { WriteEBOOL(1); P=(ObjectCount+3)&~3; WriteBytes(ptr, P*12); } else WriteEBOOL(0);
if (+0x50) { WriteEBOOL(1); WriteBytes(ptr, ObjectCount); }               else WriteEBOOL(0);
if (+0x44) { WriteEBOOL(1); WriteSLONG(+0x40); WriteBytes(ptr, n*0x50); } else WriteEBOOL(0);
```

`WriteEBOOL` = 1 byte normalised to 0/1 (`0x03082720`); `WriteSLONG` = 4 bytes LE (`0x03082880`).
Retail Load confirms every size; element size 80 is proven at `0x00BFA807`
(`lea ebp,[eax+eax*4]; shl ebp,4`).

**Hard cap:** assert `"ObjectCount>0 && ObjectCount<256"` (string `0x448C3C8`, checked
`0x02EDF851`/`0x02EDF85D`). But `MAX_BATCH_SIZE = 0x20`, the builder's scratch is
`CSubsectionElement scratch[32]` with `quadList[4][32]`, and **retail's max over 1,591 records is
exactly 32**. These two figures disagree; resolve before relying on either. Our 240-instance batch
violates both.

## `LandscapeNormalArray` is whole-array SoA — proven in BOTH directions

Reader `RenderSubPrimitive 0x02ED079E`: `ObjectCount -> +3 -> &~3 = P`; X base `[ecx+0x94]`,
Y base `lea edx,[ecx+eax*4]`, Z base again `+P*4`.
Writer `BuildFromSourceMeshes`: `normals[i]` `0x02EE1864`, `normals[i+P]` `0x02EE188C`,
`normals[i+2P]` `0x02EE18B4`.

```
f32 X[P] | f32 Y[P] | f32 Z[P]        P = (count+3)&~3
```

`P` exists because the SIMD lighting loop does `movaps xmm0,[nx+esi]`
(`0x02CEF000/0x02CEF00C/0x02CEF018`) — each slice base must be 16-byte aligned. Slots
`[count..P-1]` are never written by the builder, so retail files carry `0xCDCDCDCD` there, exactly
one run per slice. **For byte parity, emit `0xCDCDCDCD` in the pad slots.**

Value = the terrain SURFACE normal at the instance, unit length (retail Darkwood_3: 19,072/19,072
unit-length under this layout). Not the 0.45-damped normal.

**A NULL array is fatal, not benign:** the ctor `0x02EDD2F4` zeroes `+0x54` and
`RenderSubPrimitive` dereferences `prim+0x94` unconditionally. Presence byte 0 => AV. Since
stage45 (presence=0) did not crash, **stage45 never reached RenderSubPrimitive and is void as
evidence.**

## `CSubsectionElement` — fully decoded (the 80-byte guess was correct)

Authority: `ClipSubSections 0x02ECFD30` (reader); element writes `0x02EE05E2` / `0x02EE05F4` /
`0x02EE0B9C`.

```
struct CSubsectionElement {          // sizeof 0x50, 16-byte aligned
    f32 CentreX[4];   // +0x00
    f32 CentreY[4];   // +0x10
    f32 CentreZ[4];   // +0x20
    f32 Radius [4];   // +0x30
    u8  ObjectCount[4]; // +0x40   0 => lane unused
    u8  StartIndex [4]; // +0x44   ABSOLUTE index into the on-disk A/B/Wind/Normal arrays
    u8  ChildIndex [4]; // +0x48   RELATIVE: child = (u8*)thisNode + ChildIndex[q]*0x50; 0 = leaf
    u8  Pad        [4]; // +0x4C   never read, never written
};
struct CClipListElement { u8 StartIndex; u8 Count; };   // emitted 0x02ECFE89 / 0x02ECFE99
```

**The trailing 4 bytes are SOLVED: struct tail padding the engine never initialises.** The builder
writes only `0x00..0x4B`; the outer function allocates `CSubsectionElement scratch[32]` on the
stack UNINITIALISED (`sub esp,0xA4C`; array at `[ebp-0xA0C]`, `0xA00` = 32*0x50) and memcpy's it
verbatim (`0x02EDFA38`), so the tail holds whatever that stack address last held. Over 1,591 retail
records only 16 distinct tail dwords exist and each is a function of the ELEMENT INDEX only, not of
record content (element 0 -> `0x7C80E0C6`, a kernel32 code address, 1102x; element 1 ->
`0x000FC8D0`, a stack address, 252x). A writer must emit 4 bytes; emit `0x00000000`.

Empty quadrant is written as centre/radius `0.0f`, startIndex 0, childIndex 0
(`0x02EE0BB0..0x02EE0C0E`).

## The subdivision algorithm

Outer `BuildSubSectionsAndObjectRemapTable 0x02EDF740`, inner `0x02EDFB20`, caller
`BuildFromSourceMeshes 0x02EE1070` (call at `0x02EE15AB`).

Inputs: `spheres[i]` = per-object WORLD sphere, centre =
`objectMatrix.TransformPoint(mesh.GetBoundingSphere().GetCenter())` (`0x02EE13A8..0x02EE13F9`),
radius = `mesh sphere radius * objectScale` (`0x02EE135A..0x02EE1390`). Leaf threshold
`T = min(4, max(1, 128 / PeekPolyCount()))` (`0x02EE14ED..0x02EE1584`), always in {1,2,3,4}, and
**not stored in the file** — a writer needs the mesh's polygon count.

Inner, recursive, emits nodes PRE-ORDER (self, then child of q0..q3):

1. `if (count <= T) return false` (`0x02EDFB48`).
2. `N = clamp((int)ceilf(sqrtf(count)), 1, 8)` (`0x02EDFB57..0x02EDFB98`).
3. XY bounding box over sphere CENTRES ONLY; asserts `minX != maxX`, `minY != maxY`.
4. Bucket into the NxN grid: `gx = ftol((s.x-minX)/(maxX-minX)*(N-1))` (round-half-EVEN), clamped.
5. Fill 4 quadrants, each sweeping outward from its own corner (q1/q3 mirror in X, q2/q3 in Y)
   along a diagonal-shell walk `(0,0),(1,0),(1,1),(0,1),(2,0)...`, LIFO-popping each cell.
   Per-quadrant target: for `q<3`, `t = remaining/(4-q)`; if `t<=T` target=t, else target=T
   doubled by 4 until `target*4 >= remaining`. q3 takes the remainder.
6. `if (nonEmptyQuadrants <= 1) return false` — degenerate split, no node emitted.
7. Emit one element, then for each non-empty quadrant write the permutation into `remap`, set
   `objCount`/`startIndex`, compute the quadrant sphere (centre of the AABB of
   `centre +/- radius`, then max enclosing radius), and recurse; `childIndex[q]` is the
   pre-recursion element offset, or 0 if the child returned false.

Reproduces the observed counts exactly: T=4 gives 16->1, 26->2, 30->3; T=1 gives 5->2, 8->3,
12->4, 14->5, 17->6, 23->8, 32->11. **Matches 1,591/1,591 retail records field-for-field.**

## The permutation is REAL and mandatory

`remap` is a build-time local (`unsigned char remap[32]` at `[ebp-0x3C4]`), never serialised. It is
consumed immediately to write the instance arrays in permuted order (`0x02EE17F8..0x02EE1991`):

```
for i in [0, ObjectCount):
    src = remap[i]; obj = sourceObjects[src];
    LandscapeNormalArray[i]           = obj->normalX;
    LandscapeNormalArray[padded + i]  = obj->normalY;
    LandscapeNormalArray[padded*2 + i]= obj->normalZ;
    ObjectOffsets[i]   = C4DVector(obj->GetTranslation(), obj->scale);           // B
    ObjectMatricies[i] = C4DVector(cos(rot)*scale, sin(rot)*scale, 0, 0);        // A
```

So the on-disk A / B / WindDelay / LandscapeNormal arrays are **already in subsection order**, and
`startIndex[q]`/`objCount[q]` name a CONTIGUOUS RUN. A correct table over unpermuted arrays draws
the wrong instances. Our writer must apply one permutation consistently to all four arrays.

## How the renderer consumes it

Load-time: `AddObjectsToPrimitiveRenderer 0x02EDD740` -> params builder `0x02EDD4E0` -> ctor
`0x02EDD650` (`params+0x10 = SubSections`; SubSectionsCount is NOT passed) -> `AddPrimitive
0x02ECF2B0` -> `InitialisePrimitive 0x02ECF4D0`, which sets `prim+0xA0 = SubSections`,
`+0x5C = ObjectCount`, `+0x94 = normals`, `+0x98/+0x9C = A/B`, `+0xA4 = WindDelay`, and flag
`0x8000` when `HasLandscapeNormalLighting`.

Per-frame traversal `0x02ECFD30`, called from `0x02ED0839` with the array base (element 0 = root).
Per lane, in order: skip if `objCount==0` (`0x02ECFD68`); distance test
`GFSqr(drawDist + radius) < d2` -> flag `0x40`, skip (`0x02ECFDDD`); `d2 < GFSqr(drawDist - radius)`
-> allInside (`0x02ECFE0C`); `if ((flags & 0x3c) != 0x3c) CEngineCamera::ClipSphere` (`0x02ECFE41`);
skip if `flags & 0x40` (`0x02ECFE49`); then recurse on `childIndex[q]` or emit
`{startIndex[q], objCount[q]}`. **There is a NULL-table fallback that draws all ObjectCount
instances** — which is why our structurally-legal 1-lane table is not itself the blocker.

## Remaining unknowns (updated)

Superseded from the earlier list: items 3 (type-1 tail) and 4 (instance-array semantics) are now
CLOSED. Still open:

1. **CLOSED 2026-08-23:** `GetSaveSize` (`0x02E3D7A0`) serializes each collection into an empty
   temporary stream (without the payload's leading u32 collection count), then returns its length
   +0x28. For an already-built uncompressed group payload the exact size is therefore
   `payloadSize + 0x24`. `SaveContents` (`0x02E3D850`) confirms the leading count and same per-
   collection serializer.
2. **CLOSED 2026-08-23:** type-2 `CLocalDetailPrimitiveMeshZSpriteBatch::Save` at `0x02EE2420`
   writes bbox[6], sphere[4], u32 count, then count records of 0x44 bytes, followed by count
   float4 auxiliary records: `44 + count*84` bytes after the primitive tag. The four-map oracle
   now walks all 527 groups with zero type-2 truncations or unknown primitives.
3. The 32-vs-255 `ObjectCount` cap contradiction.
4. Frustum-flag bit semantics inside the traversal (`0x02ECFDFC..0x02ECFE60`); bit `0x40` =
   rejected is proven, the individual plane bits `0x04/0x08/0x10/0x20` are not. Does not affect
   the writer.
5. `T` per retail record is not uniquely recoverable from the file (many records match several
   `T`); needs mesh triangle counts out of GBANK.
6. Byte-exact subsection centres need the true transformed mesh sphere, not `B[i].xyz`
   (current approximation leaves ~0.67 residual).
7. **CLOSED 2026-08-23:** `CEngineLocalDetailGenerator::BuildThemes` (`0x02D29100`) fills
   `generator+0x38`. It repeatedly finds the highest fade end among collection types whose
   CacheGroup is still -1, sets `cutoff = highest - 16.0`, assigns every unassigned type at or above
   that cutoff, ORs their primitive masks, and appends `CCacheGroup{cutoff, highest, mask}`.
   Consumers use +4 (`highest`) and +8 (`mask`). This descending pass yields stock ids
   0..4 = fades 210, 118, 85, 48, 23.
