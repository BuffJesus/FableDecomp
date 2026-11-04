# Near-range pop-in, shadows and fade distance — causes and plan (2026-10-19)

User report (video `Radeon ReLive/unknown/unknown_2026.10.19-20.40.mp4`, review in the
texture checkout's `docs/TEXTURES_NEXT_SESSION_20261018.md`): smooth, sometimes very
low-poly rocks turn "speckled" and higher quality while still close and in view;
hero shadow edges are blocky. Dithered far fades are fine, but the user would like
them pushed farther out. Priority = switches on near, in-view objects.

All findings are static analysis (capstone on retail Fable.exe + PDB layouts + asset
reads). Nothing launched or patched. Scratch scripts/dumps from this session:
`%LOCALAPPDATA%/Temp/claude/D--Documents-FableTLC/a4aaefba-.../scratchpad/research_{shading,lod,fades,shadows}/`.

## Causes (CONFIRMED in code unless marked)

### 1. Rock "speckle" = bump map, gated twice
- **Asset:** every bump-mapped Oakvale rock (`MESH_*ROCK_LICHEN_nn`, `MBANK_ALLMESHES`)
  has `MapFlags=3` (diffuse+bump) at LOD0 and `1` (diffuse only) at LOD1+. Bank-wide:
  496 multi-LOD statics have bump on LOD0 only; none keep it on LOD1.
- **Shader:** `CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive` 0xBA1140 tests
  the bounding sphere against the shadow `RecieverClipVolume` (0xBA14A6–0xBA1568 →
  byte esp+0x1B). If inside and shadows enabled (`[0x1436E60]+8`), local render flags
  get `|=0x200` (0xBA179B); 0xBA17C8 then picks type 0xB = DP3 bump + shadow. Outside:
  type 8 = plain `PixelShaderDiffuseFog`; bump bound but never sampled.
  (Not to be confused with primitive flag `[esi+0xC]` bit 0x200 at 0xBA1755, which only
  picks stipple vs alpha-blend while fading.)
- Net: a rock looks speckled only when **LOD0 AND inside the shadow volume**. Either
  boundary crossing is a one-frame swap.

### 2. Mesh LOD switch
- Same formula in RenderPrimitive (0xBA12FD–0xBA135B) and PreloadPrimitive 0xBA4C10:
  `proj = max(d − R·s,1e-4)/(R·s·renderer+0xA8)·LODErrorTolerance`; advance while
  `LODErrors[k+1] <= proj`. No hysteresis, no crossfade.
- `renderer+0xA4/+0xA8` = 640.0 virtual screen (ctor 0xB35C78), times camera
  projection in FrameStart (0xB327F9) — resolution-independent.
- `LODErrorTolerance` Low/Normal/High/Highest = 3/2/1/0; **Highest never leaves LOD0**.
- LOD0 is read on demand: `C3DMeshInfo::GetMesh` 0xA25E70 → `BeginLoading` 0xA256F0
  (async, default priority), falling back to a resident LOD meanwhile. Preload
  (0xB92F80/0xB92E50) only runs from `LoadNewScene` 0xB328C0. LODs unload after
  `UnloadMeshFrameDelay`, so revisits can pop again.
- HYPOTHESIS: mesh radius units (R≈200 for medium rocks) vs world units unresolved, so
  exact switch distances not computed.

### 3. Texture sharpness (prior work: TEXTURE_TRANSITIONS_2026-10-19.md)
Mip requests use `min(screenWidth,640)`; uploads replace surfaces instantly. If no mip
is resident and not blocking, the subprimitive is **skipped that frame** (0xBA50A0
returns false via 0xBA9360/0xBA95D0) — late-appearing pieces.

### 4. Shadows (hero blockiness)
- One scene shadow map (`CEngineShadowRenderer`, `[0x1436E60]`), no hero-specific
  shadow. `MaxShadowDistance`=32 and `ForegroundBufferSpan`=8 hardcoded in ctor
  (0xB50440–0xB504F5); perspective warp fades out when light ∥ view.
- Size = `ShadowBufferSize`: Low 0 (off) / Normal 1024 / High 2048 / Highest 4096;
  `ShadowDistanceScale` 1/1/1/1.5. Default slider likely Normal (HYPOTHESIS).
- Technique 1 (D16 depth texture, LINEAR → 2×2 HW PCF) vs technique 2 (COLBUFF packed
  depth, POINT, single-tap `cmp`) chosen in `InitialiseTextures` 0xB4F8E0. AMD likely
  gets technique 2 (HYPOTHESIS; read `[[0x1436E60]+0x334]` live). Sampler states in
  `SetupShadowFuncRenderStates` 0xB4FC40. No blur/PCF anywhere.

### 5. Far fades (fine as-is, user wants them farther)
Per-primitive alpha/stipple ramp: `SetFadeDistanceWithGamePrimitiveDefaults` 0xB8FD40,
`GetFadeDistance` 0xBEB5C0 (× DrawDistanceMultiplier, clamped Min/Max). Grass: repeated
mesh alpha ramp (0xB99281) × `RepeatedMeshDrawDistanceFactor` (1/1/1.5/2). Thing cull
`CThingPhysical::PrepareForRendering` 0x663C40 does NOT apply the Min/Max clamp —
possible removal before alpha hits 0 when distances are pushed. Terrain bands
(`BuildLODBands`, 64·2ⁿ) are hard switches with no geomorph.

## Plan, ranked by how close the switch is

| # | Fix | Kind | Retail/patch | Modern renderer |
|---|---|---|---|---|
| 1 | Bump always on: retarget static type 8 → DP3 bump (no shadow) shader | code (shader select) | yes, small | normal maps always on |
| 2 | Bump on LOD1+: copy LOD0 bump index into LOD1+ materials at bank build (same UVs; verify tangent basis/vertex format at 0xA246E3) | data (mesh bank) | yes, FableForge bank writer | regenerate LODs + baked normals |
| 3 | Resolution-aware LOD projection (640 → viewport width) + hysteresis | code | small constant/patch | dithered LOD crossfade |
| 4 | Prefetch next-finer LOD before it's selected; never switch to a non-resident LOD | code | medium | streaming with lookahead |
| 5 | Shadows: Highest (4096) now; bilinear/4-tap PCF receiver shader (shaders.big); shorter span or more resolution near hero | data / shader asset / constant | yes | CSM + PCF/PCSS, optional hero map |
| 6 | Textures: lift the 640 cap; MinLOD ramp after upload; draw lowest mip instead of skipping | code | patch (request DLL exists) | sampler MinLOD fade |
| 7 | Farther dither fades: DrawDistanceMultiplier/Max, `GamePrimitiveDefaultFadeRangeRatio`, RepeatedMeshDrawDistanceFactor; clamp thing cull to match | data + tiny code | yes | same |
| 8 | Large Address Aware (opt-in, scratch only, top-down alloc test, explicit second host hash) | header bit | yes | moot on x64 |

## Cheap live tests (user-driven, when the game is free)
- Shadow Detail Low → rocks should never speckle (confirms shadow-volume gate).
- Mesh/Highest (LODErrorTolerance 0) with shadows on → remaining pop = shadow volume only.
- Read `[[0x1436E60]+0x334]` (1 depth / 2 COLBUFF) to confirm the AMD shadow path.
- Record at native resolution, not 1024×768.
