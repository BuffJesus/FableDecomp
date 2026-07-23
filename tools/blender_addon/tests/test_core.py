#!/usr/bin/env python3
"""Standalone (bpy-FREE) test of fable_core on REAL retail data.

Exercises the full data path the Blender addon depends on:
  open graphics.big -> pick a real compiled mesh -> build MeshModel ->
  resolve material -> LZO1X-decompress + DXT/ARGB-decode its base texture to
  a real RGBA image -> assert the image is non-trivial (real dims, not noise).

This proves the texture->material path on real bytes WITHOUT Blender. Run:
  python tools/blender_addon/tests/test_core.py \
      "<install>/data/graphics/graphics.big"
(the texture banks are auto-located next to it).

Exit code 0 = pass. Also writes a PNG of the decoded texture to scratchpad as
visual evidence if PIL is available.
"""
import os
import sys

_THIS = os.path.dirname(os.path.abspath(__file__))
# import fable_core from the addon package dir
sys.path.insert(0, os.path.join(_THIS, "..", "io_scene_fable"))
import fable_core as fc  # noqa: E402


def _find_install_graphics_big():
    for base in (
        r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters",
        r"C:\Program Files (x86)\Steam\steamapps\common\Fable The Lost Chapters",
    ):
        p = os.path.join(base, "data", "graphics", "graphics.big")
        if os.path.isfile(p):
            return p
    return None


def _image_stats(img):
    """Return (min, max, distinct_count) over a sample of RGB channels to prove
    the decoded image is real content, not a flat/constant/noise buffer."""
    px = img.pixels
    step = max(1, len(px) // 40000)  # sample up to ~10k pixels
    vals = px[::step]
    lo = min(vals)
    hi = max(vals)
    # quantize to 0..255 buckets to count distinct tones cheaply
    distinct = len({int(v * 255) for v in vals})
    return lo, hi, distinct


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else _find_install_graphics_big()
    if not path or not os.path.isfile(path):
        print("SKIP: graphics.big not found (pass a path as argv[1])")
        return 0

    banks = fc.guess_texture_banks(path)
    print("texture banks located:", banks)
    assert banks, "no texture banks (textures.big/frontend.big) found next to graphics.big"

    tex_bank = fc.TextureBank(banks)
    print("indexed texture entries:", tex_bank.count())
    assert tex_bank.count() > 1000, "too few texture entries indexed"

    buf, meshes = fc.open_graphics_big(path)
    print("compiled meshes in graphics.big:", len(meshes))
    assert len(meshes) > 100, "too few compiled meshes"

    # --- Walk meshes until we build one that yields a REAL decoded texture ---
    proven = None
    checked = 0
    skeletal_seen = None
    for e in meshes:
        checked += 1
        model = fc.build_model(buf, e, tex_bank=tex_bank, decode_textures=True)
        if model.has_skeleton and model.bones and skeletal_seen is None:
            skeletal_seen = model
        # find a material whose base texture decoded to a real image
        for md in model.materials:
            if md.image is not None and md.image.pixel_count >= 64 * 64:
                lo, hi, distinct = _image_stats(md.image)
                if distinct >= 16 and (hi - lo) > 0.15:
                    proven = (model, md)
                    break
        if proven:
            break
        if checked > 400:
            break

    assert proven is not None, "no mesh produced a real decoded base texture"
    model, md = proven
    img = md.image
    print("\n=== PROOF: real texture decoded into a material ===")
    print("  mesh          :", model.name, "(type", model.entry_type, ")")
    print("  material slot :", md.slot, "->", md.name)
    print("  base_tex_id   :", md.base_tex_id, "resolved:", md.base_tex_name)
    print("  texture       : %dx%d (alloc %dx%d) fmt=%s"
          % (img.width, img.height, img.alloc_width, img.alloc_height, img.fmt_name))
    print("  source .tga   :", img.source_tga)
    lo, hi, distinct = _image_stats(img)
    print("  pixel stats   : min=%.3f max=%.3f distinct_tones=%d pixels=%d"
          % (lo, hi, distinct, img.pixel_count))
    assert len(img.pixels) == img.alloc_width * img.alloc_height * 4, \
        "pixel buffer length mismatch"

    # --- report the submesh/LOD tree + skeleton for that model ---
    print("\n=== structural import (LOD/submesh tree) ===")
    print("  materials:", len(model.materials), " bones:", len(model.bones),
          " LODs:", len(model.lods))
    for lod in model.lods:
        print("   LOD%d @0x%x dist=%.4f submeshes=%d"
              % (lod.index, lod.offset, lod.switch_distance, len(lod.submeshes)))
        for sm in lod.submeshes[:4]:
            print("     submesh%d verts=%d faces=%d sVert=%d matSlot=%d geom=%s"
                  % (sm.index, sm.n_verts, sm.n_faces, sm.svert, sm.material_slot,
                     "STUB(None)" if sm.geometry is None else "present"))

    if skeletal_seen is not None:
        print("\n=== skeleton bone-name recovery (a skinned mesh) ===")
        print("  mesh:", skeletal_seen.name, " bones:", len(skeletal_seen.bones))
        print("  first bones:", [b.name for b in skeletal_seen.bones[:12]])

    # --- geometry honesty check: decode_geometry must currently return None ---
    if model.lods and model.lods[0].submeshes:
        g = fc.decode_geometry(buf, model.lods[0].submeshes[0], model.lods[0].offset)
        assert g is None, "decode_geometry should be a STUB returning None today"
        print("\ngeometry stub: decode_geometry() -> None (as expected, pending "
              "docs/BIG_MESH_GEOMETRY.md)")

    # --- visual evidence: save the decoded texture to scratchpad as PNG ---
    try:
        import numpy as np
        from PIL import Image
        arr = np.asarray(img.pixels, dtype=np.float32).reshape(
            img.alloc_height, img.alloc_width, 4)
        arr = arr[::-1]  # undo the bottom-up flip for a normal PNG
        rgba8 = (arr * 255.0 + 0.5).astype(np.uint8)
        out_dir = os.environ.get("FABLE_SCRATCH", _THIS)
        out = os.path.join(out_dir, "core_decoded_%d.png" % md.base_tex_id)
        Image.fromarray(rgba8, "RGBA").save(out)
        print("\nsaved visual evidence PNG:", out)
    except Exception as ex:
        print("(PNG evidence skipped:", ex, ")")

    print("\nALL CORE ASSERTIONS PASSED")
    return 0


if __name__ == "__main__":
    sys.exit(main())
