# ForgeTest64 — black terrain and invisible foliage (2026-08-22)

Both defects in the stage49 screenshot are diagnosed. They are **separate bugs with separate
root causes**; they share only a theme — in each case FableForge synthesised auxiliary
normal-derived data from its own model instead of porting the engine's algorithm.

**Status 2026-08-23: fix-plan steps 1, 2, 3, 5 and most of 6 are IMPLEMENTED and gated** — see
sections 7 (terrain) and 8 (foliage). Step 4 (porting the subsection builder) is deliberately
deferred: the fabricated table was deleted rather than kept, and a null table is a proven-legal
retail configuration. Open unknown 1 (the 32-vs-255 cap) is RESOLVED in section 8.

Raw agent output backing every claim below:
`work/no_donor_terrain_pack/RE_NOTES_20260822_subsections_manager_palette.md` (92 KB).

---

## 1. Dark terrain — `cliffU`/`cliffV` carry the wrong quantity

`stbbake.cpp:780-781` (`buildLayeredForeground`) writes those two per-vertex bytes as *slope*
and *normalised height*. The engine reads them as the **X and Y of the surface direction-mask
normal**, encoded `int((n * 0.5 + 0.5) * 255)` — truncating, not rounding.

### Evidence chain (disassembled end to end)

| hop | evidence |
|---|---|
| on-disk foreground vertex is 15 B `u16 x, u16 y, f32 z, u32 packedNormal, u8 blend, u8 cliffU, u8 cliffV` | `stbbake.cpp:1300-1306`; `CLandscapeLayerMesh::LoadForeground` FableWin `0x02DDD820` |
| stored at GPU vertex `+0x16`, `+0x15`, `+0x14`, stride `0x18` | `0x02DDDA91` / `0x02DDDAB7` / `0x02DDDACC`; `imul ecx,ecx,0x18` |
| decl stride 0x18, `D3DCOLOR` at offset 20 | `CVertexShaderDeclarations::GetLandscapeForeground` `0x02DD7C30`, decl array `0x0450F3F0` |
| D3DCOLOR is little-endian B,G,R,A so `blend`→RED, `cliffU`→GREEN, `cliffV`→BLUE | `CGenericVertex::SetU1 0x02CE7CE0` (+0x16), `SetV1 0x02CE7D20` (+0x15) |
| `VSHADER_LANDSCAPE_FOREGROUND` does `mov oT0.xy, v3.yzzz` — stage 0 sampled at `(cliffU/255, cliffV/255)`; all lighting comes from `v2` via `dp3 r4, v2, -c19` | shaders.big blob `0x1cf0` |
| stage 0 is bound to `GetForegroundBlendTable(mappingDirection)` | `CEngineLandscapePatch::RenderForeground` `0x02DFA8EC` → `0x02DFA8F8` → `0x02CE5FF0` |
| the five 128x128 tables are procedural: `u=(x-64)/64, v=(y-64)/64, w=(1-|u|)-|v|`, and **`w<=0` forces the texel to 0.0** | `CEngineLandscapeRenderer::BuildBlendingTables` `0x02C85F00`; zero branch `0x02C86162`, `fldz` `0x02C861A5`, `*255.0` `0x02C86233` |
| `PSHADER_LANDSCAPE_FOREGROUND`: `mul_sat r0.w, t0.w, v0.w` — table alpha multiplies layer alpha. The background composite path (`PSHADER_LANDSCAPE_PROC_TEXTURE`, `mul r1, t0.w, v0; mul r0, r1, t1`) goes literally black | `CLandscapeBackgroundPatch::RenderProceduralTexture 0x02CE4AF0` |

**The closing argument.** Substitute retail's encoding into the engine's own mask condition:
`|2u-1| + |2v-1| = |nx| + |ny|`, so the diamond is satisfied exactly when `|nx|+|ny| < 1` —
any surface shallower than ~45 degrees. The blend table's diamond mask *is* the natural domain
of a `(n*0.5+0.5)` normal encoding. Retail's residue outside the diamond (15.3% on Darkwood_3)
is precisely its real cliffs.

### Measured

`w = 1 - |2*cliffU/255 - 1| - |2*cliffV/255 - 1|`:

| | Darkwood_3 (11,843 v) | DemonDoor_LookoutPoint (4,624 v) | **ForgeTest64 v25 (10,404 v)** |
|---|---|---|---|
| cliffU range / mean | [8,249] / 107.5 | [122,130] / 127.0 | **[128,255] / 182.8** |
| cliffV range / mean | [8,251] / 125.9 | [122,129] / 127.0 | **[0,255] / 54.9** |
| corr(cU, nx) | 0.972 | — (flat) | **0.000** |
| corr(cV, ny) | 0.969 | — | **0.005** |
| corr(cV, height) | — | — | **1.000** |
| byte-exact vs `floor((n*.5+.5)*255)` | — | 99.5% / 99.6% | **0.0% / 0.1%** |
| fraction with `w <= 0` (black) | 15.3% | ~0% | **91.6%** |

Modal ours is `(128, 0)` on 4,020 vertices — flat ground, `w = -1`. The surviving 8.4% is the
single lit wedge in the screenshot.

Corroborated independently by `docs/OPENALBION_TERRAIN_CROSSWALK.md:172`
(`CEngineLandscapeMeshBuilder::BuildMapDirMask`, retail `0x009BF540` / FableWin `0x02CAE270`):
"CliffU and CliffV are int((component*0.5+0.5)*255), truncating rather than rounding".

### Regression point

Chunks v2/v10 used `buildSingleMaterialForeground` (`stbbake.cpp:846-848`), which writes
`blend=255, cliffU=128, cliffV=128` — retail's flat-ground value, and correct. **v11** switched
to `buildLayeredForeground` with the slope/height formula. v11 through v25 all carry the defect;
stages 46-49 never touched these bytes.

### Ruled out for the darkness — do not revisit

- **`packNormal` bit layout** — verified four independent ways. Engine pack `0x02CE99D0`,
  unpack `0x02CE9B20` / retail `0x00BFDEC0`; constants 1023.0/511.0 at `0x04443BE8`/`0x04443BF8`.
  Retail's own 11,843 normals decode unit-length under our rule (0.9989..1.0011, nz>0 everywhere);
  an alternative layout gives 10.2% unit. Only delta is `fistp` vs `floor(v+0.5)`, <=1 LSB.
- **Our normals being degenerate** — v25: length 0.9990..1.0010, nz min 0.614, zero negative.
- **The `0.45f` slope damping in `normalAt()`** — it tilts normals *toward vertical*, which
  *brightens*. Our nz mean 0.957 vs Darkwood_3's 0.750. It is a fidelity defect (7.6 deg mean,
  22.3 deg max error) and a purity violation, but it is not the darkness.
- **SelfIllumination / a per-map light or shadow record** — `LoadForeground` reads exactly one
  lighting scalar (`PeekSelfIllumination 0x02DFB460`) and it is 0 on every layer in retail
  Darkwood_3 (105 layers) *and* ours. `CLandscapeBackgroundPatch::Load 0x02CDF860` reads no light
  record. All lighting arrives as global shader constants `c3`/`c19`/`c20`/`c35`.
- **Dark textures** — ours are 2-3x *brighter* than retail's own terrain art (93.6 / 94.1 / 70.5
  vs 30.6 / 41.1). `textures_forgetest_bright_v2.big` was already tried at stage41; no effect.
- **Missing bump map** — DemonDoor_LookoutPoint has `tex[2]=0` on 16/16 layers and renders fine.
- **Time of day / FSE Lua / weather / gamma** — the deployed `FSE_Master.lua` (byte-identical
  stages 45-49) calls none of them, and sky/hero/HUD/minimap all render normally.

The only untraced darkness mechanism left is `VSHADER_LANDSCAPE_FOREGROUND_BLACKOUT_PASS`
(name string `0x04434A80`, referenced `0x02C81AAD`). There is **zero** positive evidence for it;
it is a last resort only if the cliffU/cliffV fix does not resolve the defect.

Confidence **0.90**. The one inferred link is the blend table's runtime *texel content* — the
builder code that writes 0.0 outside the diamond has been read, the live texture has not.

---

## 2. Missing foliage — the blocker is the batch size cap, not the normals

The earlier working hypothesis (degenerate `LandscapeNormalArray` values stop the draw) has been
**REFUTED**. The corrected picture, from `CEnginePrimitiveManagerRepeatedStaticMeshes`:

- The normal array is consumed **only** by `ProcessLightingSW`, which turns each normal into a
  per-instance RGB vertex-shader constant. A `(0,0,0)` normal gives dot=0, so the sun and
  back-light terms both scale to zero and the instance renders **ambient-only (dark), never
  skipped, never NaN**. With our AoS bug 160/240 instances go ambient-only but the other 80
  decode to `(1,1,1)` and get *full* sun — so a third of our grass would be brightly lit if it
  were reaching the rasteriser.
- A **NULL** array is *not* benign, though: the ctor (`0x02EDD2A0`) zeroes `+0x54`, `Load` only
  fills it when the presence byte is set, and `RenderSubPrimitive` dereferences `prim+0x94`
  unconditionally (`movaps xmm0,[nx+esi]` from a NULL base) — guaranteed AV. Since stage45
  (presence=0) did **not** crash, stage45 never reached `RenderSubPrimitive`, which matches the
  collection only beginning to load at stage46. **Stage45 is void as evidence.**

### The actual blocker

`ObjectCount` per type-1 primitive is capped. Two figures came out of the disassembly and they
disagree — **resolve this before implementing**:

- assert string at `0x448C3C8`, checked at `0x02EDF851`/`0x02EDF85D`:
  `ObjectCount>0 && ObjectCount<256`
- `CEnginePrimitiveManagerRepeatedStaticMeshes::MAX_BATCH_SIZE = 0x20`, and the builder's stack
  scratch is `CSubsectionElement scratch[32]` with `quadList[4][32]`; **retail max over 1,591
  records is exactly 32**.

Either way **our stage49 batch of 240 instances is out of contract** and must be split into
per-leaf batches of <=32. Retail batch sizes are 1..32.

### Ranked remaining blockers (from the manager disassembly)

1. **Mesh resource not resolving** — the only clean *silent* no-draw path. `RenderPrimitive`
   returns without drawing when `C3DMeshInfo::GetMesh` fails (`0x02ED1B0F`); `RenderSubPrimitive`
   returns when `GetTexture` fails (`0x02ED0F0B`). Both silent. **But see section 3A — mesh 156
   does resolve**, so this is now unlikely.
2. **Nothing within the draw radius.** The palette's fade pair is 20/22 world units with
   alpha-test ref 130/255, giving vertex alpha `-0.5*d + 11` and a hard cut at **d ~ 21 world
   units**. Our placement covers only y in [2306, 2339] — the writer truncates to the first 240
   grid points. If the teleport lands in the northern half, **zero blades are eligible**. This is
   arithmetic, not a defect, and can fully explain "no foliage" on its own.
3. **Sub-pixel geometry** — scale 0.009..0.015 is retail-plausible, but mesh 156's local extent
   and how the vertex shader applies `B.w` are unverified.

Our stage49 subsection table, contrary to the working assumption, is **structurally legal** and
would emit clip entry `{start=0, count=240}`. It is not parity (retail splits into a
`ceil(sqrt(count))`-clamped grid) but it does not stop the draw.

---

## 3. Newly recovered engine contracts

Full text in `work/no_donor_terrain_pack/RE_NOTES_20260822_subsections_manager_palette.md`;
the subsection grammar is also appended to `docs/FOLIAGE_LOCAL_DETAIL_RE.md`.

### A. Mesh 156 resolves — and the bank is global

`CLocalDetailObjectCollectionType::Load` hardcodes `bank_no 0` (FableWin `0x02E46A61 push 0`), so
the local-detail mesh bank is **not** per-map or per-region: every map resolves against
graphics.big `MBANK_ALLMESHES` (8,113 dense ids). Index 156 = `MESH_GRASSBLADES_02`, bank data
type 2 = REPEATED, exactly what `PrimitiveType==1` requires. All 3,172 retail palette records
across 399 maps resolve with zero misses.

**Side defect:** FableForge's `kMeshes` name table (`foliage.cpp:42-63`) is uniformly **off by
one** — the name/texture/category attached to id N belongs to id N+1 (21/22 sampled confirm). IDs
are right, labels are wrong. Mesh 156's real diffuse hint is `GRASSBLADES_02_32`.

### B. Palette resource slots 2 and 3 are swapped in our writer

On-disk order is **Mesh, ShadowMesh, ZSpriteMesh** (`Load 0x02E46981` -> `+0x14`,
`0x02E46992` -> `+0x1C`, `0x02E469A0` -> `+0x18`; `Save 0x02E49EF9/0x02E49F08/0x02E49F17`; def
ctor asserts literally "object_def.Mesh"/"ShadowMesh"/"ZSpriteMesh"). Retail data agrees: of
3,172 records, 580 have a field-2 mesh whose bank name contains "SHADOW" and **zero** field-3
meshes do.

`stbbake.cpp:648-650` writes `(mesh, zspriteMeshIdx, shadowMeshIdx)` and `foliage.cpp:204-205`
reads `+0x04` as zsprite / `+0x08` as shadow — **both mislabelled the same way**, and the built-in
`kSeeds` table was harvested through the mislabelled reader, so the two errors cancel and all 25
built-in types round-trip byte-identically to retail. It is a no-op for slot 0 (both 0) and bites
the moment anything supplies these IDs semantically. Fix both sides together or neither.

### C. Group header fade/mask are NOT free parameters

`BuildPrimitives 0x02E3C2A8`/`0x02E3C2CA` looks up **both** the group header's fade (`+0x1C`) and
mask (`+0x20`) from the group's CacheGroup id (`+0x24`) via
`CEngineLocalDetailGenerator::GetCacheGroupInfo`. The table was extracted from 15,418 retail
groups with zero exceptions. For ForgeTest64 the needed row is unambiguous: **CacheGroup 4 ->
fade 23.0, mask 3**.

This retroactively explains the stage47/48 crash: `+0x20` is an **integer** band index into a
stride-8 table (`LoadHeader 0x02E370D0` stores it verbatim to obj `+0x1C` at `0x02E3721B`, no FPU;
`StaticUpdate` indexes retail `0x00B597E0`, debug twin asserts index<8). Writing float `22.0f`
gave `0x41B00000*8 mod 2^32 = 0x0D000000` -> AV at `0x00B597E8`. **Never write a float there.**

### D. The palette's 0x34-byte settings block, decoded

`runtimeSettings[k]` maps to memory `0x20 + 4k`:

| idx | mem | field | getter |
|---|---|---|---|
| 0 | 0x20 | f32 FadeStart | `0x02EDD190` |
| 1 | 0x24 | f32 FadeEnd | `0x02D295B0` |
| 2 | 0x28 | i32 AlphaRef (truncated to u8) | `0x02EDD1F0` |
| 3 | 0x2C | f32 AlphaMipBias | `0x02EDD210` |
| 5 | 0x34 | f32 WindSkewRandomFactor | `0x02EDD630` |
| 7 | 0x3C | f32 ZSpriteFadeStart | `0x02EDEC40` |
| 8 | 0x40 | f32 ZSpriteFadeEnd | `0x02E4C030` |
| 9 | 0x44 | u32 PrimitiveType (0 Mesh / 1 Repeated / 2 ZSprite) | `0x02E36200` |
| 10 | 0x48 | i32 CacheGroup | `0x02D29610` |
| 11 | 0x4C | u8 AlphaIsBoolean | `0x02EDD1D0` |
| | 0x4D | u8 HasLandscapeNormalLighting | `0x02EDD610` |
| | 0x4E | u8 HasWindSkew | `0x02EE1C20` |
| 12 | 0x50 | u8 ReceivesShadows | `0x02EDD1B0` |

The engine's own naming is inconsistent (`PeekFadeStart` reads `+0x24`); what matters is that
`SetupPrimitiveDesc` builds `CEngineFadeDistance(start = mem+0x20, end = mem+0x24)` and the
shader fade requires `+0x20 < +0x24`.

---

## 4. Fix plan (ordered; nothing implemented yet)

Governed by **parity and purity**: port the engine's algorithm from disassembly, prove it by
diffing emitted bytes against retail's, and delete invented constants rather than tuning them.
A least-squares fit to retail output is an oracle for checking a port, never a substitute for one.

1. **`cliffU`/`cliffV`** — port `BuildMapDirMask` (FableWin `0x02CAE270` / retail `0x009BF540`)
   faithfully; do **not** implement the fitted `cU = 126.98 + 113.82*nx`. Apply to both
   `buildLayeredForeground` and `buildSingleMaterialForeground`. Delete the `0.45f` damping in both
   `normalAt` lambdas (purity: it is a symptom patch).
   *Gate:* corr(cU,nx) and corr(cV,ny) > 0.95 (from 0.000/0.005); `w<=0` fraction 91.6% -> ~0%;
   byte-diff the port against retail Darkwood_3 and DemonDoor_LookoutPoint and report the exact
   match rate — a clustered residual is a bug in the port.
2. **`LandscapeNormalArray`** — whole-array SoA `X[P] | Y[P] | Z[P]`, `P=(count+3)&~3`, populated
   with the real terrain normal sampled at each instance. For byte parity the `[count..P-1]` pad
   slots should hold `0xCDCDCDCD` (retail's allocator debug fill, one run per slice).
   *Gate:* every instance normal unit-length (currently 0 of 240).
3. **Split batches to <=32 instances** and attach groups to 16x16 leaf nodes. **Resolve the
   32-vs-255 cap contradiction first** (section 2).
4. **Port the subsection builder** (`0x02EDF740` outer / `0x02EDFB20` inner) including the
   permutation — A, B, WindDelay and the normals are stored **pre-permuted** into subsection order,
   and the permutation must be applied consistently to all four or grass scatters to wrong
   positions. Emit `0x00000000` in the element tail padding.
   *Gate:* emit for a retail batch's instance set and byte-diff against that batch's retail bytes,
   across every type-1 batch in two maps.
5. **Coverage and density** — instances currently stop at y=2351.69 leaving the four cellY=3 cells
   empty, at 0.059/u2 vs Darkwood_3's 0.215/u2. Given the ~21-unit draw radius, coverage is not
   cosmetic: fix it before concluding anything from a runtime test.
6. **Container conformance** (batch these): palette slot order (3B, both sides together),
   collection `flag18`, group `+0x26` bit0, group `+0x20`/`+0x1C` from the CacheGroup table (3C),
   group `+0x24` = 4, node header mask, group file-block triple = owning node's triple,
   common-record filler `0x7A..0x7C` = `88 37 02`.

---

## 5. Open unknowns

1. **The 32 vs 255 instance-cap contradiction** (section 2). Blocks step 3. Re-read `0x02EDF851`
   and the `MAX_BATCH_SIZE` reference.
2. **Draw radius vs where the hero lands.** Cheapest possible test and it may moot everything:
   walk to ~(3360, 2320) and look down. If grass appears, the writer was closer than we thought.
3. **CacheGroupInfo provenance** — recovered by exhaustive observation (15,418 groups, zero
   exceptions) and the engine's read proven, but the loader that *fills* `generator+0x38` was not
   disassembled. Almost certainly a `CEngineLocalDetailGeneratorDef` in game.bin. Treat the 5-row
   table as an empirical constant, not a hardcode, until xrefed.
4. **Leaf threshold `T` per record** — the formula is disassembly-proven
   (`T = min(4, max(1, 128 / PeekPolyCount))`, `0x02EE14ED..0x02EE1584`) but `T` is not stored in
   the file and many retail records match more than one `T`. Needs the referenced meshes' triangle
   counts out of GBANK.
5. **Byte-exact subsection sphere centres** need `objectMatrix.TransformPoint(meshSphere.centre)`
   from the real mesh asset, not the translation (current approximation leaves ~0.67 residual).
6. **Type-2 (ZSpriteBatch) on-disk layout** still unknown; the 92-byte guess mis-sizes it, which is
   the sole cause of 2,406 failed group parses. Type-0-only and type-1-only groups parse with zero
   residue, so no number above is contaminated.
7. **`VSHADER_LANDSCAPE_FOREGROUND_BLACKOUT_PASS`** selection condition (last resort only).
8. **Blend-table runtime texel content** — deliberately deferred; step 1's offline gate is cheaper.

## 6. Instrumentation caveats for the next runtime test

- The FSE foreground decode probe (`D:\Code\ForgeFSE\FableScriptExtender\FableAPI.cpp:1713-1810`)
  is **misaligned by +4 bytes** (`vertices = decodedEnd - vertexCount*24` reads one field early),
  so every per-vertex number it has ever logged is shifted one field. Its "z" for retail Darkwood
  was (-0.653, 0.694) — impossible for a world height. Fix before trusting it. Also
  `g_forgeTestLandscapeMap` never matched `layer+0x40` after arrival, so there are **zero**
  ForgeTest decode lines despite a 256 cap.
- The FSE log is **single-attach with no rotation** — each run destroys the previous one. That is
  why stages 46-48 have no runtime evidence and their outcomes had to be reconstructed from WER
  events and backup-set hashes. **Copy the log into `runtime_evidence\<stage>\` before relaunching.**


---

## 7. Step 1 implemented and gated (2026-08-23)

`BuildMapDirMask` and `CMap::PeekMapNormal` are no longer re-derived per call site. Both are now
shared, documented ports in forgecore:

- `forge::stbbake::buildMapDirMask` / `packDirMaskByte` / `packMapNormal` /
  `quantizeEngineHeight` / `clampedHeightSampler`
  (`D:/Code/FableForge/libs/forgecore/{include/forge,src}/stbbake.{hpp,cpp}`).
- `buildLayeredForeground`, `buildSingleMaterialForeground` and `buildBackgroundPatchRect` call
  them. The invented `0.45f` upward damping is DELETED from all three (it was a symptom patch for
  the darkness, and the darkness was cliffU/cliffV all along).
- `apps/forge/main.cpp`'s `--rebuild-direction-mask` lambdas now delegate to the same functions
  instead of carrying a second copy of the algorithm.

### Gate 1 — retail parity of the port

New durable test `forge_dirmask_tests` (`tests/test_dirmask.cpp`, registered in CTest). It reads a
retail `.lev` plus a `forge stb foregroundinfo --vertices` dump of that map's retail baked chunk and
diffs recomputed cliffU/cliffV against the bytes retail actually stores. Fixtures come from
`FORGE_DIRMASK_CASES`; without it the test SKIPs (same policy as `test_bwd.cpp`). Example fixture
list: `tests/fixtures_dirmask_cases.example.tsv`.

| map | unique coords | exact | mean abs err U / V |
|---|---|---|---|
| Darkwood9_Leadout_01 | 1,089 | **1,089 (100%)** | 0 / 0 |
| Darkwood_Filler_15 | 9,409 | **9,409 (100%)** | 0 / 0 |
| Darkwood_9 | 4,225 | 4,205 (99.53%) | 0.0033 / 0.0026 |

The 20 Darkwood_9 misses are +/-1 LSB on interior vertices and are **not** a defect in this port:
the independent Python oracle `tools/compare_retail_direction_mask.py` returns the identical 4,205
and the identical mean errors, and recomputing the whole chain in `long double` does not reduce them
(so retail's x87 80-bit intermediates are not the explanation either). The test carries an explicit
`allow=20` for that one case so any regression still fails the gate.

### Gate 2 — the ForgeTest64 defect measurement, before and after

Re-baked with the v25 inputs (`forge stb create-terrain ForgeTest64_final.lev ... 3328 2304 399
4216 4216 0 --slope-tex 4188 --height-tex 4212 --foliage`) into
`work/no_donor_terrain_pack/ForgeTest64_terrain_v26.{chunk,info}.bin`:

| metric | v25 (before) | v26 (after) | plan gate | retail Darkwood_3 |
|---|---|---|---|---|
| corr(cliffU, nx) | 0.000 | **0.997** | > 0.95 | 0.972 |
| corr(cliffV, ny) | 0.012 | **0.989** | > 0.95 | 0.969 |
| fraction with `w <= 0` (black) | 91.6% | **4.5%** | ~0% | 15.3% |
| cliffU range / mean | [128,255] / 182.8 | [16,238] / 127.0 | — | [8,249] / 107.5 |
| cliffV range / mean | [0,255] / 54.9 | [32,247] / 124.5 | — | [8,251] / 125.9 |
| packed normal nz mean | 0.957 | 0.856 | lower is less damped | 0.750 |
| packed normal length | 0.9990..1.0010 | 0.9989..1.0011 | unit | unit |

Vertex count is unchanged (10,404). The residual 4.5% is genuine steep terrain, below retail
Darkwood_3's own 15.3%, so nothing is being masked.

**Not yet done:** v26 has not been packaged, installed, or run. The runtime check of the dark
terrain is the next action and needs the game, as does the section-5 open unknown #2 (walk to
~(3360, 2320) and look down for grass). Foliage steps 2-6 are untouched.


---

## 8. The instance cap resolved, and the foliage writer fixed (2026-08-23)

### 8.1 The 32-vs-255 contradiction — resolved

Both figures are real; they bound different things, and the disassembly of
`RenderSubPrimitive` (FableWin `0x02ED0610`) settles which one a writer must obey.

- **255 is the format bound.** `ObjectCount > 0 && < 256` (assert at `0x02EDF851`) exists because
  the subsection element's `objCount[4]` / `startIndex[4]` are `u8`.
- **The draw loop does not cap at 32.** At `0x02ED12B9-0x02ED1333` the renderer *chunks*:
  `batch = layout+0x174`, then for `b = 0, 1, ...` it draws
  `n = min(batch, total - b*batch)` instances starting at `b*batch`, stopping when `n <= 0`.
  So a primitive with more than 32 instances is drawn correctly **when nothing is culled**
  (`0x02ED09C3`: `total == ObjectCount` takes the fast path straight off the primitive's own
  arrays). This refutes the earlier claim that our 240-instance batch was "undrawable on that
  alone" — it was out of contract, but it was not the thing stopping the draw.
- **32 is the cull-compaction bound, and it is the operative one.** When `total != ObjectCount`
  the survivors are compacted into the manager's lane arrays at `mgr+0x90`, and the stores at
  `0x02ED0B54` / `0x02ED0B78` / `0x02ED0BA0` are `base[i*4]`, `base[i*4+0x80]`, `base[i*4+0x100]` —
  **the lanes are strided 0x80 bytes = 32 floats apart**. Instance 32 writes into lane array Y.
  In the debug build the assert at `0x02ED08DF` (`cmp total, 0x20; jle`) catches it; retail has no
  assert and silently corrupts.
- **Retail never gets near it.** Independently measured on retail Darkwood_3 with
  `tools/localdetail_verify.py`: 45 type-1 records, `ObjectCount` min 1, **max 30**, mean 11.4.

**Writer rule:** split at <= 32 instances per type-1 primitive. Emitting more is only safe for a
primitive that can never be partially culled (no subsection table), which is a worse trade than
splitting.

### 8.2 Also settled from retail bytes

- **A null subsection table is legal, not a degraded mode.** 8 of Darkwood_3's 45 type-1 records
  carry none, and `RenderSubPrimitive` has an explicit no-table path (`0x02ED0E25`) that draws
  `[0, ObjectCount)`. So deleting our fabricated single-lane element loses per-quadrant culling and
  nothing else.
- **The collection's leading EBOOL is NeedsRenderUpdate, not "enabled".** Every type-1 grass
  collection sampled out of retail (groups `0xb87ee`, `0xb9140`, `0xbbb27`, all CacheGroup 4)
  carries **0** and renders. Our writer had it at 1.

### 8.3 What changed in the writer

In `stbbake.cpp`, all gated by `tools/localdetail_verify.py` below:

1. **Batch splitting** — type-1 instances are emitted as `ceil(n/32)` primitives instead of one
   oversized primitive (`kRepeatedMeshMaxBatch`).
2. **`LandscapeNormalArray` is whole-array SoA** — `X[P] | Y[P] | Z[P]`, `P = (count+3)&~3`, pad
   slots `0xCDCDCDCD`, populated with the real terrain normal under each instance from the engine's
   own `CMap::PeekMapNormal` port (`mapNormal`). It was an interleaved `(0,0,1)` triple per
   instance, which decodes as degenerate under the engine's SoA read.
3. **The fabricated subsection table is deleted** (presence byte 0) rather than kept until
   `BuildSubSectionsAndObjectRemapTable` is ported.
4. **Group header fade/mask/CacheGroup come from the `GetCacheGroupInfo` table**, not from the
   palette's own fade and a synthesised mask; the node's quad header takes the OR/MAX of its
   subtree. For grass that is CacheGroup 4 -> fade 23.0, mask 3 — byte-identical to what retail's
   grass groups carry.
5. **Coverage and density** — the 240-instance truncation is gone (it had confined grass to the
   southern third of a map whose draw radius is ~21 units) and the scatter grid went from step 3 to
   step 2, ~0.25 candidates/u2 before slope rejection against retail Darkwood_3's 0.215/u2.
   Placements are ordered by 8x8-cell tile so each 32-instance batch has a tight bounding sphere
   instead of a map-wide one.
6. The collection EBOOL is now 0 for everything but z-sprite batches.

### 8.4 Gate — `tools/localdetail_verify.py`

New structural decoder and checker for baked local-detail sections. It walks the quadtree from the
root header in the map's STB common record, decompresses each group, parses the collection and
primitive grammar, and enforces the engine's constraints: `0 < ObjectCount <= 32`, normal array
present and unit length, `|A.xy| == B.w`, instances inside their primitive's sphere and bbox, and
contents consumed exactly.

**It validates retail first.** On retail Darkwood_3 (extracted from `FinalAlbion_RT.stb`): 21
quadtree nodes, 41 groups, 394 primitives, 848 instances, 37 subsection tables, zero problems. The
only thing it cannot walk is a type-2 ZSpriteBatch (13 groups stop early), which is a documented
open unknown, reported as a skip rather than silently.

ForgeTest64, same tool, before and after:

| | v25 (before) | v29 (after) | retail Darkwood_3 |
|---|---|---|---|
| type-1 primitives | 1 | 20 | 45 |
| instances | 240 | 610 | 848 |
| ObjectCount max | **240** (violates the cap) | **32** | 30 |
| normals | degenerate (AoS misread as SoA) | unit, real terrain normals | unit |
| fabricated subsection tables | 1 | 0 | n/a (37 real ones) |
| group fade / mask / CacheGroup | 20.0 / 2 / 0 | **23.0 / 3 / 4** | 23.0 / 3 / 4 |
| collection EBOOL | 1 | 0 | 0 |
| verifier verdict | 2 problems | **OK** | OK |

Chunk: `work/no_donor_terrain_pack/ForgeTest64_terrain_v29.{chunk,info,common}.bin`. FableForge's
own `forge_tests` writer tests were updated to the corrected contract (they had encoded the old
assumptions) and pass.

### 8.5 Known remaining gaps (not defects that block a draw)

- **One quadtree node.** Retail attaches groups to 16x16 leaf nodes (Darkwood_3: 21 nodes); we
  still emit a single root group covering the map. Culling granularity only.
- **No subsection tables** — legal, but not parity. This is fix-plan step 4, still open, and it is
  the one place where a byte-diff against a retail batch's bytes is the right gate.
- **Group file-block triple** is our own block rather than the owning node's.
- **Palette slot order (Mesh/ShadowMesh/ZSpriteMesh)** is still swapped on both the writer and the
  reader; they cancel today and must be fixed together.
- The built-in catalog's CacheGroup for near-mesh types looks off by one (entry 14 carries
  CacheGroup 0 while its fadeStart 118 is CacheGroup 1's fade). The writer honours the palette
  field; the catalog itself needs re-harvesting.
