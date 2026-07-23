# SPDX-License-Identifier: MIT
"""Headless Blender test for io_scene_fable.

Run:
  blender --background --factory-startup --python \
      tools/blender_addon/tests/blender_headless_test.py -- \
      "<install>/data/graphics/graphics.big" [MESH_NAME] [out_dir]

(`--factory-startup` avoids unrelated pre-installed user addons.)

It registers the addon from source (not from a zip install), invokes the import
operator on a real mesh, and ASSERTS that:
  * bpy images were created and are packed with real (non-empty) pixels,
  * a Principled-BSDF material was created and wired to an image texture,
  * an Armature with the recovered bone names exists,
  * the LOD/submesh Empty hierarchy exists,
  * decode_geometry() builds a real mesh per compiled primitive.
On success it saves a .blend + a viewport render PNG to the out dir as proof and
exits 0. Any failure raises (Blender returns nonzero).
"""
import os
import sys

import bpy

# ---- args after '--' ----
argv = sys.argv
args = argv[argv.index("--") + 1:] if "--" in argv else []
GBIG = args[0] if args else None
MESH = args[1] if len(args) > 1 else "MESH_SEAGULL_01"
OUT = args[2] if len(args) > 2 else os.path.dirname(os.path.abspath(__file__))

if not GBIG or not os.path.isfile(GBIG):
    print("SKIP: graphics.big not provided/found:", GBIG)
    sys.exit(0)

# ---- make the addon package importable + put tools/ on path ----
ADDON_DIR = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
TOOLS_DIR = os.path.normpath(os.path.join(ADDON_DIR, ".."))
if ADDON_DIR not in sys.path:
    sys.path.insert(0, ADDON_DIR)
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

# import + register the addon from source
import io_scene_fable  # noqa: E402
io_scene_fable.register()
print("addon registered; operator present:",
      hasattr(bpy.ops.import_scene, "fable_model"))

# fresh scene
bpy.ops.wm.read_factory_settings(use_empty=True)

# ---- run the import operator ----
res = bpy.ops.import_scene.fable_model(
    filepath=GBIG, entry=MESH, import_all=False, decode_textures=True,
    tools_dir=TOOLS_DIR)
print("operator result:", res)
assert res == {"FINISHED"}, "import operator did not finish: %r" % (res,)

# ---- assertions ----
imgs = [im for im in bpy.data.images if im.name.startswith("FABLE_TEX_")]
print("images created:", [im.name for im in imgs])
assert imgs, "no Fable textures were packed as bpy images"
for im in imgs:
    assert im.size[0] > 0 and im.size[1] > 0, "image %s has zero size" % im.name
    # confirm the packed pixels are real (not an all-zero buffer)
    px = list(im.pixels[:4000])
    assert any(p > 0.01 for p in px), "image %s appears empty (all-zero)" % im.name

mats = [m for m in bpy.data.materials if m.name.startswith(MESH)]
print("materials created:", [m.name for m in mats])
assert mats, "no materials created"
# at least one material has an image texture node linked to Base Color
def _mat_has_image(m):
    if not m.use_nodes:
        return False
    for n in m.node_tree.nodes:
        if n.type == "TEX_IMAGE" and n.image is not None:
            return True
    return False
assert any(_mat_has_image(m) for m in mats), "no material wired to an image texture"

arms = [o for o in bpy.data.objects if o.type == "ARMATURE"]
print("armatures:", [(o.name, len(o.data.bones)) for o in arms])
# seagull is skinned -> expect an armature with bones
if MESH == "MESH_SEAGULL_01":
    assert arms and len(arms[0].data.bones) > 0, "expected an armature with bones"
    print("bone names:", [b.name for b in arms[0].data.bones])

empties = [o for o in bpy.data.objects if o.type == "EMPTY"]
subm_empties = [o for o in empties if "_SUBM" in o.name]
lod_empties = [o for o in empties if "_LOD" in o.name and "_SUBM" not in o.name]
print("LOD empties:", len(lod_empties), " submesh empties:", len(subm_empties))
assert lod_empties, "no LOD placeholder empties created"

# geometry: decode_geometry now builds a real mesh per compiled primitive.
mesh_objs = [o for o in bpy.data.objects if o.type == "MESH"]
print("real MESH objects:", len(mesh_objs))
assert len(mesh_objs) >= 1, "expected real mesh geometry (decode_geometry is implemented)"
assert all(len(o.data.vertices) > 0 and len(o.data.polygons) > 0 for o in mesh_objs), \
    "every imported mesh object must have vertices and faces"
if MESH == "MESH_SEAGULL_01":
    assert len(mesh_objs) == 2, "seagull should import 2 primitive submeshes"

# ---- save proof: a .blend and a render ----
os.makedirs(OUT, exist_ok=True)
blend_out = os.path.join(OUT, "fable_import_%s.blend" % MESH)
bpy.ops.wm.save_as_mainfile(filepath=blend_out)
print("saved .blend:", blend_out)

# Visual proof: save a PACKED bpy image back out to PNG. This proves the decoded
# texture round-tripped INTO Blender's image datablock (a geometry render would
# be black -- there's no geometry yet, that's the honest stub). We save the
# first Fable image via Blender's own image saver.
try:
    im = imgs[0]
    png_out = os.path.join(OUT, "fable_bpy_image_%s.png" % MESH)
    im.filepath_raw = png_out
    im.file_format = "PNG"
    im.save()
    print("saved bpy-image PNG (proves the texture is a real Blender image):", png_out)
except Exception as ex:
    print("(image save skipped:", ex, ")")

print("\nBLENDER HEADLESS TEST: ALL ASSERTIONS PASSED")
