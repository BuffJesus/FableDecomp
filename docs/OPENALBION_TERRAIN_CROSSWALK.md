# OpenAlbion terrain crosswalk

Source reviewed: `jamen/openalbion` at commit
`bc19d8c91423bd7e3990ccbe111a86438c165cf4` (2026-08-15).

## Highest-value finding

OpenAlbion's `fable-data/src/landscape/mesh.rs` independently transcribes
`CMap::PeekMapNormal` as two separately normalized two-cell slopes followed by
one final 3-vector normalization:

1. `horizontal = normalize2(h(x-1,y)-h(x+1,y), 2)`
2. `vertical = normalize2(h(x,y-1)-h(x,y+1), 2)`
3. `normal = normalize3(horizontal.x, vertical.x,
   horizontal.y*vertical.y)`

FableForge's `forge stb bake-heightfield` instead used a plain height gradient
`normalize3(-dzdx,-dzdy,1)`. The two are close on shallow terrain but diverge
strongly on authored slopes.

`tools/compare_terrain_normal_models.py` measures the difference without
editing data. Results:

| LEV | changed packed normals | mean angle | maximum angle |
|---|---:|---:|---:|
| 32x32 ForgeTest | 707/1089 (64.92%) | 3.31 degrees | 18.74 degrees |
| 32x32 sculpted48 | 707/1089 (64.92%) | 4.74 degrees | 31.71 degrees |
| 64x64 ForgeTest | 886/4225 (20.97%) | 0.03 degrees | 3.98 degrees |

The exact model was ported into FableForge's `authoredNormal` calculation. It
still writes the proven signed 11/11/10 packed normal and preserves donor
topology plus disk bytes `Blend`, `CliffU`, and `CliffV` unchanged.

Offline rebake of the 32x32 corrected ForgeTest chunk succeeded:

- output size unchanged: 132,632 bytes;
- 4 composed patches and 4 foreground frames re-encoded;
- foreground compressed spans decreased from `2386,5945,6781,7335` to
  `2350,5754,6701,7176`, remaining inside the donor allocation; full precision
  exceeded the fixed aggregate budget, so the first `clearBits=0x0f` fallback
  was selected;
- `stbvalidate`: S1 PASS, S2 26/26 PASS, S7 4/4 PASS, overall `RESULT: OK`;
- S3 retains the existing nine-frame alignment warning and was not introduced
  by this normal change.

Generated proof artifact (not deployed):

- `tmp/ForgeTest_chunk_peekmapnormal.bin`, SHA-256
  `4674867A9BA35D495928626B800E3208A1E43898FC39B3758B9C61B8FEE07ABC`;
- `tmp/FinalAlbion_RT_peekmapnormal.stb`, SHA-256
  `D74FD5F27F899D4959A7807A98841D6C648303B20D591CF779C10F5DB1076C63`.

The container copy reports 425 entries. Extracting
`Data\Levels\FinalAlbion\ForgeTest.lev` from it reproduces the chunk SHA-256
exactly. Neither artifact was deployed.

The 64x64 path also passes: 17 composed patches and 16 foreground frames were
re-encoded in an unchanged 811,136-byte chunk; S1 and S2 (129/129 frames) pass,
S7 resolves 16/16 quadtree entries, and the result is `OK`. Its fixed donor
budget selected the existing `clearBits=0x7f` fallback (`99,933 <= 100,582`
bytes); this is normal quantization inside the signed 11/11/10 fields, not a
different packed-normal format. S3 retains the source chunk's existing
36-frame alignment warning.

## Retail donor cross-check

`forge stb foregroundinfo` now accepts `--vertices <out.tsv>` so decoded retail
foreground words can be compared directly. The paired retail
`Darkwood9_Leadout_01` chunk and LEV contain 2,251 foreground vertex records
covering all 1,089 coordinates of a 32x32 map. Repeated coordinates across
layers have zero stored-normal conflicts.

`tools/compare_retail_terrain_normals.py` produced the important split:

| model / sample | exact packed words | mean angle | maximum angle |
|---|---:|---:|---:|
| plain gradient, 961 unique interior vertices | 447/961 | 1.093 degrees | 10.777 degrees |
| PeekMapNormal, 961 unique interior vertices | 486/961 | 0.053 degrees | 0.277 degrees |
| PeekMapNormal, 128 unique border vertices | 9/128 | 24.413 degrees | 59.676 degrees |

Thus PeekMapNormal is strongly validated as the interior lighting-normal model,
although it is not a byte-for-byte writer oracle. The remaining sub-degree
interior differences are consistent with writer arithmetic/quantization or a
closely related smoothing path. The border discrepancy has a clearer cause:
retail bakes sample neighbouring maps. OpenAlbion exposes the same distinction
with `vertex_normal_stitched`; a standalone LEV necessarily clamps outside its
own heightfield and cannot reproduce retail seam normals.

FableForge now exposes that path directly:

```text
forge stb bake-heightfield <chunk> <target.lev> <x> <y> <out>
  [--neighbor <neighbor.lev> <x> <y>]...
  [--world <world.wld> --levels-root <data/Levels> --level <level-name>]
```

The WLD form discovers edge and diagonal maps from the target's owning
region(s), opens only available candidate LEVs, verifies the command placement
against the WLD, and de-duplicates any explicit neighbours. On the retail
cross-check it automatically found the same five maps as the manual command;
both outputs are byte-identical with SHA-256
`F7F5649E8F0E84F226B20D3150AFDF4E1C9EC58D57EA4CD85C095C879A36C067`.

The final retail cross-check exposed two additional engine rules: the
serialized extra row/column is not map-owned (`x == width` resolves through a
neighbour), and `PeekLandscapeHeight` rounds every sampled height to 1/128 of a
world unit. With the four edge neighbours plus the southwest diagonal
`Darkwood10_Leadout_01`, all 128 border normals match retail exactly as packed
11/11/10 words. Across the complete map, 2,225/2,251 layer-expanded records
(98.84%) and 1,081/1,089 unique coordinates are byte-exact; the remaining
angular error is at most 0.067 degrees. Border mean error is 0.022 degrees and
its maximum is 0.057 degrees.

The 26 non-exact records are duplicates of only eight interior coordinates.
Each differs by exactly one signed quantization unit in one component: six
records are X `-1`, fifteen are Y `+/-1`, and five are Z `-1` relative to the
reconstruction. Float32 division, reciprocal-multiply, and extended final
normalization variants all leave the same eight-coordinate set. There is no
global rounding adjustment that improves them without breaking exact vertices;
they are treated as localized retail cache/history variance unless another
retail donor demonstrates a repeatable rule.

The baker now applies the same ownership and 1/128 quantization to emitted
foreground heights, composed-patch heights, normals, and quadtree Z bounds. On
the retail donor all 2,251 emitted foreground heights and auxiliary bytes match,
and 2,225 normals match. Full normal precision is attempted first; the existing
low-bit coarsening ladder is only used when an edited map exceeds its fixed
donor allocation.

Evidence artifacts (not deployed):

- `tmp/Darkwood9_Leadout_01_retail_vertices.tsv`;
- `tmp/Darkwood9_Leadout_01_peekmap_rebake.bin` (unchanged 132,632-byte
  container, S1/S2/S7 pass);
- `tmp/Darkwood9_Leadout_01_peekmap_vertices.tsv`.
- `tmp/Darkwood9_Leadout_01_stitched_rebake.bin` and
  `tmp/Darkwood9_Leadout_01_stitched_vertices.tsv`; final chunk SHA-256
  `F7F5649E8F0E84F226B20D3150AFDF4E1C9EC58D57EA4CD85C095C879A36C067`.

## What OpenAlbion can and cannot settle

Useful as a secondary oracle:

- patch size and alternating triangle topology;
- theme-weight normalization and the `blend > 0x10` layer threshold;
- merging layers by `(mapping direction, texture set)`;
- neighbouring-map height and palette ownership at seams;
- five mapping directions and foreground UV transforms;
- deterministic local-detail generation and PRNG.

Not authoritative for full STB generation:

- it regenerates terrain from LEV instead of reading/writing
  `FinalAlbion_RT.stb`;
- its `DirectionMask::build` explicitly substitutes `PeekMapNormal` for the
  retail weighted face smoothing used by `BuildMapDirMask`;
- several citations refer to private `fable-reimpl` and shader-disassembly
  inputs absent from the repository, so claims still require our own
  Ghidra/PDB/retail cross-check.

## BuildMapDirMask recovery

The local `ego_r.exe` Ghidra project and PDB symbol identify retail
`CEngineLandscapeMeshBuilder::BuildMapDirMask` at `0x009bf540`. Its exact
structure is now recovered:

- vertices whose world `(x ^ y) & 1` is zero accumulate eight surrounding
  triangle normals; odd vertices accumulate four;
- each face normal is normalized, then weighted by the product over XYZ of
  `0.5 / (abs(component) + 0.0625) + 0.5294118`;
- the weighted sum is normalized;
- `CliffU` and `CliffV` are `int((component * 0.5 + 0.5) * 255)`, truncating
  rather than rounding.

`tools/compare_retail_direction_mask.py` matches all 1,089 unique coordinates
and all 2,251 layer-expanded `CliffU`/`CliffV` records in the stitched retail
donor exactly. FableForge exposes this as the opt-in
`--rebuild-direction-mask` bake flag. A native retail rebake reproduces all
2,251 U bytes and all 2,251 V bytes exactly and retains the original compressed
foreground spans.

The ForgeTest experiment changes 738 unique lookup coordinates (1,536 U and
1,560 V records). It remains structurally valid but increases foreground
entropy enough to select the `clearBits=0x1f` normal fallback. Its proof chunk
is `tmp/ForgeTest_chunk_wld_directionmask.bin`, SHA-256
`AE9CDAE8795E6F90C8E2E4038F08DD0A4C91C512FC8CE3427D456CAEAD6BF040`.
This is not promoted as the runtime candidate yet: the same recovered normal
also controls five direction-face booleans used during layer topology creation,
and the current donor topology has not been rebuilt.

## Retail layer-topology rule

Direct decompilation of retail `ReadThemesAndCreateLayers` (`0x009c0c30`) and
`AddPolysSurroundingPointWithMask` (`0x009bf060`) confirms the topology sequence:

1. Read three theme IDs and the first two blend bytes from the map owner.
2. Zero unavailable theme levels and renormalize the surviving three weights to
   integer values summing to 255.
3. Merge identical base texture tuples and identical cliff texture tuples in
   separate passes, adding their weights.
4. Ignore a merged contribution unless its weight is strictly greater than
   `0x10`.
5. Base textures request mapping direction 0. Each cliff contribution requests
   directions 1 through 4 independently.
6. For each contributing vertex, visit its four surrounding cells. Under the
   parity-selected triangle split, add a triangle only if the contributing
   point is one of its three vertices and at least one of those vertices has
   the requested direction's `BuildMapDirMask` face boolean set.

Consequently, simple layer-vertex presence is not an oracle for the face mask:
a vertex with zero direction blend can still be emitted as a supporting corner
of a triangle activated by either of its neighbours. A valid topology comparator
must decode per-layer index buffers and reconstruct triangles, not compare the
TSV vertex set directly.

## Next terrain step

The corrected ForgeTest WLD at `(2784,2560)` automatically resolves
`Darkwood_Filler_18` `(2816,2496)` and `Darkwood_Filler_19` `(2816,2592)`.
Compared with the clamped bake, neighbour ownership changes 95/2,251 emitted
heights and 198/2,251 packed normals across 66 unique coordinates, confined to
the two easternmost columns (`x=2815..2816`). `Blend`, `CliffU`, and `CliffV`
remain donor-exact. The 132,632-byte chunk passes S1/S2/S7 and has SHA-256
`DFDED201042A0EBC38FB8FF636255E54CF938649A4C774202471BBAA57F19999`.
The bake reports 68 unique samples resolved through those neighbours and 129
locally clamped samples on world edges without an owning map; this coverage is
now printed for every bake rather than silently falling back.
The offline 425-entry container copy is
`tmp/FinalAlbion_RT_wldnormal.stb`, SHA-256
`9EEECE655DABFADD2E100ED19F5CAC99842437CBD787974CC98807D7F6246766`;
re-extracting its ForgeTest entry reproduces the chunk hash exactly.

Use the donor-mask WLD-driven container for the staged ForgeTest runtime probe.
Next, feed the recovered direction normal through the already-understood five
`GetMappingDirectionBlend` tests and compare the resulting face booleans and
layer membership against retail. Only then promote regenerated `CliffU`/
`CliffV` into a full-topology authored candidate.
