# Fable TLC particle, lighting, sky, and shadow handoff

Start with `docs/PARTICLE_LIGHTING_VIEWER_HANDOFF.md`. It separates confirmed
retail data, decompiled runtime behavior, original GPU instructions, and the
remaining interpretation gaps.

## Immediate viewer fixes

- Candle flame texture 4422 is 32x64, so its quad is 2:1 rather than square.
- Candle `SpriteFlags=0` means centered `2D_FACE_ME`, not no facing behavior.
- Its two sprite angles are 45 and 135 degrees.
- Candle `ADDSMOOTH` is the D3D9 tuple `ONE / INVSRCCOLOR / ADD`.
- The ordinary particle pixel shader doubles RGB after texture/particle colour
  multiplication and multiplies their alpha channels normally.
- Water-vapour texture 4517 uses only 64x64 of a 256x256 allocation, so its UV
  maxima are 0.25.
- Outer sky first blends two textures and then uses gradient alpha as a second
  mask between the texture result and the top/bottom colour gradient.

## Package layout

- `docs/` — readable implementation handoff.
- `reports/` — reports generated from the local retail banks and lighting
  table.
- `shaders/` — exact D3D shader-model 1.1 disassembly with source-payload
  SHA-256 values.
- `decomp/` — supporting recovered engine routines and PDB-backed enum dumps.
- `tools/` — reproducible bank/report/shader extraction scripts.
- `SHA256SUMS.txt` — integrity hashes for every packaged file.

Retail `.big` files and `lighting_colours.tga` are deliberately not included.
Point the tools at a local Fable TLC installation.

## Reproduce the particle report

```powershell
python tools\report_particle_rendering.py `
  "Fable Install\data\Misc\pc\effects.big" `
  "Fable Install\data\graphics\pc\textures.big" `
  --id 137 --id 313 --id 314 --id 130 --id 926 `
  --json particle_viewer_report.json
```

## Reproduce the lighting lookup samples

This requires Pillow (`python -m pip install Pillow`).

```powershell
python tools\report_environment_lookup.py `
  "Fable Install\data\LightingTable\lighting_colours.tga" `
  --column day_dawn=0 --column oakvale_start=36 `
  --column snowspire_start=157 `
  --json lighting_lookup_samples.json
```

## Reproduce the original shader disassembly

On Windows this uses `d3dcompiler_47.dll`.

```powershell
python tools\dump_shader_asm.py `
  "Fable Install\data\shaders\pc\shaders.big" `
  --name SPRITE_GROUP --output particle_shaders.asm

python tools\dump_shader_asm.py `
  "Fable Install\data\shaders\pc\shaders.big" `
  --name OUTER_SKY --name INNER_SKY --output sky_shaders.asm
```

## Remaining gaps

- Semantic names for every particle and inner-sky shader constant register.
- The separate mesh-normal RSA lighting formula.
- Signed blend-op behavior outside the normal render-state selector.
- Full shadow-buffer implementation and scene eligibility rules.

The shader assembly itself is original retail bytecode disassembled directly
from `shaders.big`; it is not an approximation or a rewritten HLSL shader.
