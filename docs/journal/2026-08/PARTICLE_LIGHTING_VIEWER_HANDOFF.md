# Particle, RSA-lighting, sky, and shadow viewer handoff

This is a recovered retail contract for implementing Fable TLC effects in an
independent viewer. It distinguishes authored asset data, decompiled runtime
behavior, and the remaining shader-level gaps.

## Candle flame: immediate fix

`CANDLE_FLAME` (effect 137), `CANDLE_FLAME_ROUND_01` (313), and
`CANDLE_FLAME_TRIO_01` (314) all use:

- `CPSCRenderSprite`
- texture 4422, `SPRITE_CANDLE_FLAME_01`
- DXT1, real and allocated dimensions **32x64**
- `BlendMode=4`, `BlendOp=0`, `SpriteFlags=0`
- `NoCrossedSprites=2`

The recovered engine enums name those values `ADDSMOOTH` and `ADD`. In the
ordinary particle colour pass that resolves to the exact D3D9 tuple:

```text
SRCBLEND  = ONE
DESTBLEND = INVSRCCOLOR
BLENDOP   = ADD
```

This matters for the flame edge: `ADDSMOOTH` is not conventional
`SRCALPHA/ONE` additive blending.

`CPSCRenderSprite::PreparePrimitives @ 0x00ABE360` does not build a square.
For each frame:

```text
quad width  = render size
quad height = render size * frameHeight / frameWidth
```

The candle is therefore two times as tall as it is wide. The routine emits two
different crossed planes. Their angle is:

```text
baseAngle + (i + 0.5) * 0.5 turns / count
```

For two planes, the offsets are `0.125` and `0.375` turns: 45 and 135 degrees,
90 degrees apart. They are geometry, not two animation frames. A square quad
or a renderer which handles those planes as separate flame frames reproduces
the reported square/cut-double-flame bug.

`SpriteFlags=0` is not "no orientation." The PDB-backed
`EEngineSpriteFlag` defines zero as `ENGINE_SPRITE_2D_FACE_ME` and
`ENGINE_SPRITE_ALIGN_CENTER`. Bit `0x01` selects `3D_FACE_ME`; alignment bits
are left `0x02`, right `0x04`, top `0x08`, and bottom `0x10`. The remaining
recovered bits are rotate-around-centre `0x20`, rotate-around-Z `0x40`, lit
`0x80`, modulate-2x `0x100`, and disable-Z-buffer `0x200`. Thus the candle uses
the centered 2D camera-facing path before the two 45/135-degree crossed-plane
offsets are applied.

There are two distinct engine paths:

| Authored component | Runtime primitive | Camera/orientation controls |
|---|---|---|
| `CPSCRenderSprite` | `CEnginePrimitiveParticleSprite`, batched in groups of ten | packed `SpriteFlags`, crossed count, update orientation |
| `CPSCSingleSprite` | `CEnginePrimitiveSprite` | separate `FaceMe2D`, `FaceMe3D`, `CrossedSprites`, `SpriteAlignment`, and rotation switches |

Do not collapse them into one fixed-quad implementation.

Run the joined asset report:

```powershell
python tools\report_particle_rendering.py `
  "Path to Your Fable Install\data\Misc\pc\effects.big" `
  "Path to Your Fable Install\data\graphics\pc\textures.big" `
  --id 137 --id 313 --id 314 --id 130 --id 926 `
  --json work\particle_viewer_report.json
```

## Particle UVs, alpha, colour, and batching

The GPU path sends frame UV scale as:

```text
uScale = realWidth  / allocatedWidth
vScale = realHeight / allocatedHeight
```

This is not optional padding metadata. For example,
`SPRITE_WATER_VAPOUR_01` (4517) is a real 64x64 image in a 256x256 allocation,
so both UV maxima are **0.25**. `LARGEWATERFALL` (130) uses this texture.

The particle group owns a static ten-quad buffer and submits at most ten
sprites per draw (40 vertices / 20 triangles). Per-particle position, size,
angle, colour, and frame constants are uploaded for each batch.

The original shader-model 1.1 programs are present in `shaders.big`; they do
not need to be approximated or inferred from screenshots. This command
extracts and disassembles the relevant retail token streams:

```powershell
python tools\dump_shader_asm.py `
  "Path to Your Fable Install\data\shaders\pc\shaders.big" `
  --name VSHADER_SPRITE_GROUP_BILLBOARD_DIRLIGHT_FOG `
  --name VSHADER_SPRITE_GROUP_CROSSED_DIRLIGHT_FOG `
  --name VSHADER_SPRITE_GROUP_3D_FACE_ME_DIRLIGHT_FOG `
  --name PSHADER_SPRITE_GROUP `
  --output work\particle_shaders.asm
```

The ordinary particle pixel shader is only three shader-model 1.1 slots:

```text
tex t0
mul    r0.w,   t0, v0
mul_x2 r0.xyz, t0, v0
```

Thus output alpha is texture alpha times interpolated particle alpha, while
output RGB is exactly twice texture RGB times interpolated particle RGB. The
bank payload SHA-256 is recorded beside every disassembly so a viewer author
can verify that they are using the same retail shader.

The vertex programs also settle the three facing families. The 3D-face-me
shader derives and normalizes a horizontal camera-to-particle perpendicular;
the crossed shader uses the authored XY direction with the world-Z axis; and
the billboard shader transforms its authored two-component basis through the
camera basis constants. Their exact instruction sequences are in
`work/particle_shaders.asm`; the remaining task is attaching human names to
every constant register, not recovering the GPU math itself.

Authored colour bytes are BGRA on disk; the parser reports RGBA. Runtime colour
is interpolated Start -> Mid -> End over life, then modified by emitter/system
fade, smooth fade-in/out, and optional flicker. Preserve the texture format:
DXT1 can use its one-bit transparent mode, while DXT3 supplies explicit
four-bit alpha.

`CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive` selects
these normal-pass blend states:

| Engine mode | `D3DRS_SRCBLEND` | `D3DRS_DESTBLEND` |
|---|---|---|
| `ADDITIVE` (3) | `ONE` | `ONE` |
| `ADDSMOOTH` (4) | `ONE` | `INVSRCCOLOR` |
| other modes | `SRCALPHA` | `INVSRCALPHA` |

The authored blend op maps `ADD` (0) to `D3DBLENDOP_ADD`, `SUB` (1) to
`D3DBLENDOP_SUBTRACT`, and `REVSUB` (2) to
`D3DBLENDOP_REVSUBTRACT`; the remaining signed enum values fall through to
ordinary `ADD` in this state selector. Whether those modes also modify shader
operands is still an open shader-level target. These tuples are for
`CPSCRenderSprite`'s batched normal colour pass; depth and special render
passes deliberately override them.

## RSA/environment lighting: exact authored source

The community name "RSA light" corresponds to retail's orientation-dependent
diffuse/backlight/ambient model. The authored ramp is real:

```text
data\LightingTable\lighting_colours.tga
```

The retail file is **190x21 RGB**. `ENVIRONMENT` in `game.bin` names the file
as `lighting_colours.tga` and supplies these exact rows:

| Row | Channel |
|---:|---|
| 0 | diffuse |
| 1 | ambient |
| 2 | cloud |
| 3 | backlight |
| 4 | reflection |
| 5 | mist effect |
| 6 | fog colour |
| 7 | fog alpha |
| 8 | sun and sunlight attenuator |
| 9 | moon |
| 10 | stars |
| 11 | sun flare |
| 12 | lens flare |
| 13 | sky gradient top |
| 14 | sky gradient top alpha |
| 15 | sky gradient bottom |
| 16 | sky gradient bottom alpha |
| 17 | water |
| 18 | sea |
| 19 | glow threshold |
| 20 | glow bloom |

Each `ENVIRONMENT_THEME_DAY` selects a starting `ColourLookupColumn`. Each
element in its `Time` vector uses `startingColumn + elementIndex`. The ordinary
day theme starts at column 0 with eight time keys; some other useful spans are:

| Theme | Start | Keys |
|---|---:|---:|
| `ENVIRONMENT_THEME1` | 0 | 8 |
| `ENVIRONMENT_CAVE` | 14 | 2 |
| `ENVIRONMENT_HAUNTED` | 18 | 4 |
| `ENVIRONMENT_OAKVALE` | 36 | 7 |
| `ENVIRONMENT_WITCHWOOD` | 55 | 8 |
| `ENVIRONMENT_INTERNALS` | 79 | 8 |
| `ENVIRONMENT_UNDERTREES` | 109 | 8 |
| `ENVIRONMENT_GUILD_INTERNALS` | 129 | 8 |
| `ENVIRONMENT_HOOKCOAST` | 147 | 8 |
| `ENVIRONMENT_SNOWSPIRE_CLEAR` | 157 | 8 |
| `ENVIRONMENT_SNOWSPIRE_SNOWING` | 167 | 8 |
| `ENVIRONMENT_PRISON` | 186 | 4 |

`CEnvironmentLookupTexture::LookupFloatColour @ 0x008C5650` samples one exact
texel—no filtering inside that function—and decodes BGRA memory as:

```text
floatChannel = (byteChannel + 0.5) / 255.0
```

Notice that byte 255 becomes `255.5/255`, slightly above 1. Theme construction
samples the lookup first. `CBlendedEnvironmentTheme::Blend` then interpolates
time/theme values, and `CEnvironment::RenderUpdate @ 0x006EFF00` performs the
render-frame interpolation. Applying bilinear filtering to the TGA before
those steps changes the colours.

To dump exact channel values at chosen columns:

```powershell
python tools\report_environment_lookup.py `
  "Path To Your Fable Installation\data\LightingTable\lighting_colours.tga" `
  --column day_dawn=0 --column oakvale_start=36 --column snowspire_start=157 `
  --json work\lighting_lookup_samples.json
```

The CPU SIMD block sent toward mesh/particle lighting is 272 bytes:

```text
0x020/0x030/0x040  diffuse direction X/Y/Z, each replicated float[4]
0x050/0x060/0x070  diffuse R/G/B, each replicated float[4]
0x080/0x090/0x0A0  backlight R/G/B, each replicated float[4]
0x0B0/0x0C0/0x0D0  ambient R/G/B, each replicated float[4]
0x0E0/0x0F0/0x100  object position X/Y/Z, each replicated float[4]
```

The remaining parity item is instruction-level recovery of the vertex-shader
normal/orientation formula. The constant identities and authored inputs above
are verified; a generic hemispheric/RSA approximation should not be labelled
exact until that shader formula is decoded.

## Why a skybox can have the right texture but wrong colour

The sky is not a single unlit cubemap:

- `CEnvironmentThemeDef` independently selects `SkyTexture0`,
  `SkyTexture1`, and `SkyTexture1Blend`.
- The outer-sky vertex shader receives both the lookup-table top and bottom
  gradient colours.
- Lower and upper clouds each have two textures, a blend, and a speed.
- Cloud colour is lookup row 2; top/bottom sky gradients are rows 13/15, with
  separate alpha rows 14/16.
- Sun, moon, stars, flares, fog, and glow use their own rows.
- Theme changes and render-frame interpolation affect all of the above.

Recovered mesh constants are a 6,500-unit radius, 36 radial segments, outer
heights `-500` and `7000`, and a cloud grid whose vertex alpha is authored by
the mesh builder. Using the sky images without the current theme columns,
gradient colours/alpha, and texture blends explains a close shape with wrong
colours.

The exact retail shader order is now available in `work/sky_shaders.asm`.
`VSHADER_OUTER_SKY` computes the vertex gradient as:

```text
gradient = vertexWeight * colour1 + (1 - vertexWeight) * colour0
```

`PSHADER_OUTER_SKY` samples both sky textures and then performs:

```text
textureBlend = saturate(c0.w) * texture1
             + (1 - saturate(c0.w)) * texture0
output       = saturate(gradient.a) * saturate(gradient)
             + (1 - saturate(gradient.a)) * textureBlend
```

`RenderOuterSky` uploads `c0` as the same scalar in all four lanes, with the
value `1 -` the current authored sky blend, and uploads two independent
lookup-derived gradient colours. This confirms that the gradient alpha is a
second blend mask over the two-texture result, rather than merely the output
alpha.

The inner-sky/cloud shader is also exact: it samples four textures, blends
0/1 by `c0.w` and 2/3 by `c1.w`, premultiplies the second pair by its alpha,
combines the two layers using the first pair's alpha, applies vertex colour
and vertex alpha, then multiplies RGB by `2 * c2` with saturation.

Reproduce both families directly from the retail bank:

```powershell
python tools\dump_shader_asm.py `
  "Path to Your Fable Install\data\shaders\pc\shaders.big" `
  --name OUTER_SKY --name INNER_SKY `
  --output work\sky_shaders.asm
```

## Shadow path

Retail uses `CEngineShadowRenderer`, not only a dark material:

1. Build/update world-to-shadow transforms and clip volumes.
2. Render eligible scene primitives into the shadow buffer.
3. Pack depth through the renderer's depth-packing lookup texture/pass.
4. Bind shadow transforms and fade constants in lit scene shaders.

Each environment time key authors `ShadowFactor` and `FadedShadowFactor`.
After time-key interpolation the shader fade triple is exactly:

```text
(1 - ShadowFactor,
 ShadowFactor - FadedShadowFactor,
 FadedShadowFactor)
```

Spotlight shadows have a separate path and falloff parameters. Particle sprite
groups also select different shaders for self illumination and local point
lights; do not assume every translucent particle receives the world shadow.

## Evidence and remaining work

Raw recovered routines:

- `work/particle_renderer_runtime_egor.c`
- `work/particle_primitive_contract_egor.c`
- `work/particle_sprite_group_render_egor.c`
- `work/particle_sprite_group_ctor_egor.c`
- `work/particle_shaders.asm`
- `work/sky_shaders.asm`
- `work/rsa_lighting_contract_egor.c`
- `work/environment_lookup_contract_egor.c`
- `work/sky_shadow_contract_egor.c`

The PDB-backed blend enums are:

```text
mode 0 SOLID, 1 BOOLEAN_ALPHA, 2 ALPHA, 3 ADDITIVE,
     4 ADDSMOOTH, 5 CONST_COLOUR
op   0 ADD, 1 SUB, 2 REVSUB, 3 ADDSIGNED, 4 REVSUBSIGNED
```

Next exact targets are names for every particle shader constant register, the
signed blend-op behavior, the RSA mesh-normal formula, and semantic names for
the remaining inner-sky/cloud constant registers.
