# SPDX-License-Identifier: MIT
"""Headless Blender test: REAL bone bind poses (docs/BIG_MESH_GEOMETRY.md sec. 7).

Run:
  blender --background --factory-startup --python \
      tools/blender_addon/tests/blender_bones_test.py -- \
      "<install>/data/graphics/graphics.big" [out_dir]

For each test mesh (a bird + a humanoid) it imports via the addon and ASSERTS:
  * the armature bone count equals the compiled BoneCount,
  * the hierarchy is a tree with a single root named Scene Root,
  * bone heads are NOT all at the origin and lie inside the (padded) mesh bbox,
  * the skinned mesh bbox surrounds the skeleton (center distance < half-extent).
Saves a .blend + viewport render per mesh as proof.
"""
import os
import sys

import bpy
from mathutils import Vector

argv = sys.argv
args = argv[argv.index("--") + 1:] if "--" in argv else []
GBIG = args[0] if args else None
OUT = args[1] if len(args) > 1 else os.path.dirname(os.path.abspath(__file__))

if not GBIG or not os.path.isfile(GBIG):
    print("SKIP: graphics.big not provided/found:", GBIG)
    sys.exit(0)

ADDON_DIR = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
TOOLS_DIR = os.path.normpath(os.path.join(ADDON_DIR, ".."))
for p in (ADDON_DIR, TOOLS_DIR):
    if p not in sys.path:
        sys.path.insert(0, p)

import io_scene_fable  # noqa: E402
import io_scene_fable.fable_core as fc  # noqa: E402
io_scene_fable.register()

MESHES = ["MESH_SEAGULL_01", "MESH_CREATURE_GOBLIN"]

buf, entries = fc.open_graphics_big(GBIG, tools_dir=TOOLS_DIR)

for MESH in MESHES:
    bpy.ops.wm.read_factory_settings(use_empty=True)

    # ground truth straight from the compiled payload
    e = fc.find_entry(buf, entries, MESH)
    assert e is not None, "entry %s not found" % MESH
    pay = bytes(buf[e["offset"]:e["offset"] + e["size"]])
    _nm, _prims, gt_bones = fc._parse_mesh_primitives(pay, e["type"])
    assert gt_bones, "%s: no bones decoded from payload" % MESH
    print("%s: compiled BoneCount = %d" % (MESH, len(gt_bones)))

    res = bpy.ops.import_scene.fable_model(
        filepath=GBIG, entry=MESH, import_all=False, decode_textures=True,
        tools_dir=TOOLS_DIR)
    assert res == {"FINISHED"}, "import failed for %s" % MESH

    arms = [o for o in bpy.data.objects if o.type == "ARMATURE"]
    assert len(arms) == 1, "%s: expected exactly 1 armature" % MESH
    arm = arms[0]
    bones = arm.data.bones

    # 1) bone count matches the compiled payload
    assert len(bones) == len(gt_bones), \
        "%s: armature has %d bones, payload has %d" % (MESH, len(bones), len(gt_bones))

    # 2) single-root tree named Scene Root
    roots = [b for b in bones if b.parent is None]
    assert len(roots) == 1, "%s: expected 1 root, got %r" % (MESH, [b.name for b in roots])
    assert roots[0].name == "Scene Root", "%s: root is %r" % (MESH, roots[0].name)
    assert any(b.name == "Bip01" for b in bones), "%s: no Bip01 bone" % MESH

    # 3) heads not all at origin, and inside the padded mesh bbox
    mesh_objs = [o for o in bpy.data.objects if o.type == "MESH"]
    assert mesh_objs, "%s: no mesh geometry imported" % MESH
    pts = [o.matrix_world @ Vector(c) for o in mesh_objs for c in o.bound_box]
    bb_min = Vector((min(p[i] for p in pts) for i in range(3)))
    bb_max = Vector((max(p[i] for p in pts) for i in range(3)))
    pad = (bb_max - bb_min) * 0.5 + Vector((0.2,) * 3)
    heads = [arm.matrix_world @ b.head_local for b in bones]
    n_origin = sum(1 for h in heads if h.length < 1e-6)
    assert n_origin < len(heads), "%s: ALL bone heads at origin" % MESH
    outside = [b.name for b, h in zip(bones, heads)
               if not all(bb_min[i] - pad[i] <= h[i] <= bb_max[i] + pad[i]
                          for i in range(3))]
    assert not outside, "%s: bones outside padded mesh bbox: %r" % (MESH, outside)

    # 4) skinned mesh surrounds the skeleton: bbox-center distance < half-extent
    mesh_c = (bb_min + bb_max) * 0.5
    skel_c = sum(heads, Vector()) / len(heads)
    half_ext = (bb_max - bb_min).length * 0.5
    d = (mesh_c - skel_c).length
    print("%s: mesh center %s, skeleton mean %s, dist %.2f (half-extent %.2f)"
          % (MESH, tuple(round(v, 2) for v in mesh_c),
             tuple(round(v, 2) for v in skel_c), d, half_ext))
    assert d < half_ext, "%s: skeleton is not inside the mesh volume" % MESH

    # 5) real-bind-pose marker (not the name-scan fallback)
    arm_obj_note = arm.get("fable_note", "")
    assert "REAL bind pose" in arm_obj_note, \
        "%s: armature was built from the fallback path: %r" % (MESH, arm_obj_note)

    # proof artifacts
    os.makedirs(OUT, exist_ok=True)
    blend_out = os.path.join(OUT, "fable_bones_%s.blend" % MESH)
    bpy.ops.wm.save_as_mainfile(filepath=blend_out)
    # simple OpenGL-independent render: camera framed on the model
    scn = bpy.context.scene
    cam_data = bpy.data.cameras.new("cam")
    cam = bpy.data.objects.new("cam", cam_data)
    scn.collection.objects.link(cam)
    scn.camera = cam
    center = (bb_min + bb_max) * 0.5
    size = max((bb_max - bb_min).length, 1.0)
    cam.location = center + Vector((0.8, -1.6, 0.9)) * size
    look = center - cam.location
    cam.rotation_euler = look.to_track_quat("-Z", "Y").to_euler()
    cam_data.clip_end = size * 10
    light = bpy.data.objects.new("sun", bpy.data.lights.new("sun", "SUN"))
    scn.collection.objects.link(light)
    light.rotation_euler = (0.6, 0.2, 0.8)
    for eng in ("BLENDER_EEVEE", "BLENDER_EEVEE_NEXT", "BLENDER_WORKBENCH"):
        try:
            scn.render.engine = eng
            break
        except TypeError:
            continue
    scn.render.resolution_x = 800
    scn.render.resolution_y = 600
    scn.render.filepath = os.path.join(OUT, "fable_bones_%s.png" % MESH)
    try:
        bpy.ops.render.render(write_still=True)
        print("render saved:", scn.render.filepath)
    except Exception as ex:
        print("(render skipped:", ex, ")")
    print("saved:", blend_out)
    print("%s: PASS (%d bones, root=%s, %d at origin, all inside bbox)"
          % (MESH, len(bones), roots[0].name, n_origin))

print("\nBLENDER BONES TEST: ALL ASSERTIONS PASSED")
