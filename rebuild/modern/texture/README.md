# Textures (GBANK entries in `.big` banks)

Portable C++23 decoder for retail GBANK textures ([TEXTURE.md](../../../docs/formats/TEXTURE.md)):
the 34-byte info descriptor, mip 0 (LZO1X chunks covering all but the last 3 bytes, then those
3 bytes raw; or all raw when `MipSize0` is 0), and linear A8R8G8B8 (stored BGRA), DXT1 and DXT3,
converted to RGBA8 and cropped to the authored frame size. LZO1X comes from `../codec`; banks
come from `../bank`. A self-contained PNG writer (stored deflate blocks) keeps the module free
of zlib.

## Verified (2026-10-17)

`fable_texture_dump <bank> --check` decodes mip 0 of every GBANK entry:

- `frontend.big`: **394/394** (125 A8R8G8B8, 116 DXT1, 153 DXT3).
- `textures.big` (retail backup): **6,319/6,324**. The other 5 use pixel formats 0x18 (1) and
  0x23 (4), which TEXTURE.md §3 lists as unconfirmed; they are reported as unsupported, not
  guessed.

`fable_texture_tests` covers the descriptor, a DXT1 block, BGRA order, the mip-0 chunk framing
and the PNG writer.

## The title logo through the modern tree

`fable_title_view <frontend.big> [--seconds N] [--capture <png>]` (Windows):
1. decodes `FRONTEND_TITLE_01_SPRITE` and `FRONTEND_TITLE_02_SPRITE`, the two 256×128 halves
   of the logo;
2. composes them over black;
3. presents them in a 64-bit window through D3D11. `--capture` reads the presented back buffer
   back from the GPU and writes it as a PNG.

This is the modern tree's first frame: "FABLE / The Lost Chapters", whole.

The viewer only clears and copies (no shaders). It is a check that the fork's own bank, LZO1X
and texture code produce the right pixels on a 64-bit build, not the fork's renderer
(MODERNIZATION step 4: an RHI with D3D12 and Vulkan backends).

## Not covered

- Mips 1..n-1 (stored raw after mip 0) and multi-frame sprites (`FrameCount` 4/16).
- Pixel formats 0x18 and 0x23.
