# SPDX-License-Identifier: MIT
"""Headless Blender proof for NEW-MESH COMPOSITION (docs/MESH_COMPOSE.md §5).

Run:
  blender --background --factory-startup --python \
      tools/blender_addon/tests/blender_compose_test.py -- \
      "<COPY of graphics.big>" [out_dir]

NEVER point this at the Steam install (a modified .big is written next to the
copy). Steps:
  1. compose the synthetic textured cube + grass cluster (compose_test.py)
     PLUS a NEW SKINNED boot (donor boot geometry scaled + re-skinned by bone
     NAME to the same hero-rig bones, skeleton cloned from the donor),
  2. add them as NEW entries to the graphics.big copy (big_write adds=;
     the boot goes in as a type-5 entry),
  3. import ALL through the EXISTING import operator from the modified copy
     (textures come from the retail textures.big, read-only),
  4. assert a real textured mesh appears; for the skinned boot assert the
     armature exists with the donor's bones, the mesh has an ARMATURE modifier
     + vertex groups, and per-vertex weights MATCH the authored skin;
     save .blend + render PNG as proof.
"""
import os
import sys

import bpy

argv = sys.argv
args = argv[argv.index("--") + 1:] if "--" in argv else []
GBIG = args[0] if args else None
OUT = os.path.abspath(args[1] if len(args) > 1 else os.path.join(
    os.path.dirname(os.path.abspath(__file__)), "proof"))

if not GBIG or not os.path.isfile(GBIG):
    print("SKIP: graphics.big copy not provided/found:", GBIG)
    sys.exit(0)
assert 'steamapps' not in GBIG.lower(), "refusing to modify the Steam install"

_THIS = os.path.dirname(os.path.abspath(__file__))
ADDON_DIR = os.path.normpath(os.path.join(_THIS, ".."))
TOOLS_DIR = os.path.normpath(os.path.join(ADDON_DIR, ".."))
for p in (ADDON_DIR, TOOLS_DIR, os.path.join(ADDON_DIR, "io_scene_fable"), _THIS):
    if p not in sys.path:
        sys.path.insert(0, p)

import fable_core as fc          # noqa: E402
import big_write                 # noqa: E402
from compose_test import make_cube, make_grass, make_skinned_boot   # noqa: E402
import mesh_rw                   # noqa: E402

TEXTURES_BIG = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\graphics\pc\textures.big"

# ---- 1+2: compose and add to the copy ----
MOD_BIG = os.path.join(os.path.dirname(GBIG), "graphics_blender_compose.big")
buf, _meshes = fc.open_graphics_big(GBIG, tools_dir=TOOLS_DIR)
adds = []
names = []
for name, prims, mats in (make_cube(), make_grass()):
    pay, info = mesh_rw.compose_mesh(name, prims, mats)
    adds.append(dict(sub="MBANK_ALLMESHES", name=name, payload=pay,
                     type=1, info=info))
    names.append(name)
# NEW SKINNED mesh: modified boot geometry re-skinned to the same bones
BOOT_NAME, boot_prims, boot_mats, boot_skel, boot_skin = \
    make_skinned_boot(buf, _meshes)
boot_pay, boot_info = mesh_rw.compose_mesh(BOOT_NAME, boot_prims, boot_mats,
                                           skeleton=boot_skel)
adds.append(dict(sub="MBANK_ALLMESHES", name=BOOT_NAME, payload=boot_pay,
                 type=5, info=boot_info))
names.append(BOOT_NAME)
rebuilt = big_write.rebuild(buf, adds=adds)
with open(MOD_BIG, "wb") as f:
    f.write(rebuilt)
print("wrote modified copy:", MOD_BIG)
del buf, rebuilt

# ---- 3: import through the existing operator ----
import io_scene_fable            # noqa: E402
io_scene_fable.register()
bpy.ops.wm.read_factory_settings(use_empty=True)

for name in names:
    res = bpy.ops.import_scene.fable_model(
        filepath=MOD_BIG, entry=name, import_all=False, decode_textures=True,
        tools_dir=TOOLS_DIR,
        textures_big=TEXTURES_BIG if os.path.isfile(TEXTURES_BIG) else "")
    print("import", name, "->", res)
    assert res == {"FINISHED"}, "import failed for %s" % name

# ---- 4: assertions ----
mesh_objs = [o for o in bpy.data.objects if o.type == "MESH"]
print("mesh objects:", [(o.name, len(o.data.vertices), len(o.data.polygons))
                        for o in mesh_objs])
assert len(mesh_objs) >= 3, "expected all three new meshes as real geometry"
cube_obj = next(o for o in mesh_objs if "CUBE" in o.name.upper())
grass_obj = next(o for o in mesh_objs if "GRASS" in o.name.upper())
boot_obj = next(o for o in mesh_objs if "BOOT" in o.name.upper())
assert len(cube_obj.data.vertices) == 24 and len(cube_obj.data.polygons) == 12
assert len(grass_obj.data.vertices) == 20 and len(grass_obj.data.polygons) == 10
assert cube_obj.data.uv_layers.active, "cube has no UVs"

# ---- 4b: SKINNED boot -- armature binding + weights survive the round trip ----
arms = [o for o in bpy.data.objects if o.type == "ARMATURE"]
assert len(arms) == 1, "expected exactly 1 armature (the composed boot's)"
arm = arms[0]
bone_names = {b.name for b in arm.data.bones}
assert bone_names == set(boot_skel["names"]), \
    "armature bones %r != donor skeleton %r" % (sorted(bone_names),
                                                sorted(boot_skel["names"]))
mods = [m for m in boot_obj.modifiers if m.type == "ARMATURE"]
assert mods and mods[0].object == arm, "boot: no Armature modifier bound"
assert boot_obj.parent == arm, "boot: mesh not parented to armature"
assert len(boot_obj.data.vertices) == len(boot_prims[0]["verts"])
gi_name = {vg.index: vg.name for vg in boot_obj.vertex_groups}
for vg in boot_obj.vertex_groups:
    assert vg.name in bone_names, "group %r is not an armature bone" % vg.name
for v in boot_obj.data.vertices:
    assert len(v.groups) >= 1, "boot vertex %d has no group" % v.index
    s = sum(g.weight for g in v.groups)
    assert abs(s - 1.0) < 1e-3, "boot vertex %d weights sum %.4f" % (v.index, s)
    got = {gi_name[g.group]: g.weight for g in v.groups}
    want = {}
    for (bname, w) in boot_skin[0][v.index]:
        want[bname] = want.get(bname, 0.0) + w
    tot = sum(want.values())
    want = {k: w / tot for k, w in want.items()}
    assert set(got) == set(want), \
        "boot v%d bones %r != authored %r" % (v.index, sorted(got), sorted(want))
    for k, w in want.items():
        assert abs(got[k] - w) <= 1.5 / 255.0, \
            "boot v%d %s: %.5f != %.5f" % (v.index, k, got[k], w)
print("skinned boot OK: %d verts, %d groups, armature %d bones -- "
      "weights match authored skin" % (len(boot_obj.data.vertices),
                                       len(boot_obj.vertex_groups),
                                       len(arm.data.bones)))

mats = [m for m in bpy.data.materials if m.name.startswith("MESH_FORGE_TEST")]
print("materials:", [m.name for m in mats])
assert mats, "no materials created for the new meshes"

def _mat_image(m):
    if not m.use_nodes:
        return None
    for n in m.node_tree.nodes:
        if n.type == "TEX_IMAGE" and n.image is not None:
            return n.image
    return None

imgs = [_mat_image(m) for m in mats]
have_tex = [im for im in imgs if im is not None]
if os.path.isfile(TEXTURES_BIG):
    assert have_tex, "no material wired to a decoded texture image"
    for im in have_tex:
        assert im.size[0] > 0 and any(p > 0.01 for p in im.pixels[:4000]), \
            "decoded texture appears empty"
    print("textured materials OK:", [im.name for im in have_tex])
else:
    print("textures.big not found -- texture assertions skipped")

# ---- proof artifacts ----
os.makedirs(OUT, exist_ok=True)
blend_out = os.path.join(OUT, "fable_compose_new_meshes.blend")
bpy.ops.wm.save_as_mainfile(filepath=blend_out)
print("saved .blend:", blend_out)

# simple viewport render: camera + light aimed at the new objects
cam = bpy.data.objects.new("Cam", bpy.data.cameras.new("Cam"))
bpy.context.scene.collection.objects.link(cam)
cam.location = (2.2, -2.2, 1.6)
cam.rotation_euler = (1.05, 0.0, 0.785)
bpy.context.scene.camera = cam
sun = bpy.data.objects.new("Sun", bpy.data.lights.new("Sun", 'SUN'))
bpy.context.scene.collection.objects.link(sun)
sun.location = (3, -3, 5)
sun.rotation_euler = (0.6, 0.2, 0.5)
bpy.context.scene.render.resolution_x = 640
bpy.context.scene.render.resolution_y = 480
bpy.context.scene.render.filepath = os.path.join(OUT, "fable_compose_new_meshes.png")
bpy.ops.render.render(write_still=True)
print("saved render:", bpy.context.scene.render.filepath)

print("\nBLENDER COMPOSE TEST: ALL ASSERTIONS PASSED")
