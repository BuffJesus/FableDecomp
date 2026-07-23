# SPDX-License-Identifier: MIT
"""Headless Blender test: per-vertex SKIN WEIGHTS -> vertex groups + Armature
modifier (docs/BIG_MESH_GEOMETRY.md sec. 7.1).

Run:
  blender --background --factory-startup --python \
      tools/blender_addon/tests/blender_skin_test.py -- \
      "<install>/data/graphics/graphics.big" [out_dir]

For each skinned test creature (goblin 67 bones, seagull 27) it imports via the
addon and ASSERTS, per imported mesh object:
  * vertex groups exist and EVERY group name is a real bone in the armature,
  * every vertex has >= 1 group assignment,
  * per-vertex weights sum to ~1.0 (decoder renormalizes),
  * an ARMATURE modifier points at the imported armature and the mesh is
    parented to it,
  * the Blender-side weights MATCH the ground-truth fable_core._prim_skin
    decode for that primitive (same bone set + weight values).
Saves a .blend per mesh as proof.
"""
import os
import sys

import bpy

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

MESHES = ["MESH_CREATURE_GOBLIN", "MESH_SEAGULL_01"]

buf, entries = fc.open_graphics_big(GBIG, tools_dir=TOOLS_DIR)

for MESH in MESHES:
    bpy.ops.wm.read_factory_settings(use_empty=True)

    # ground truth straight from the compiled payload
    e = fc.find_entry(buf, entries, MESH)
    assert e is not None, "entry %s not found" % MESH
    pay = bytes(buf[e["offset"]:e["offset"] + e["size"]])
    _nm, prims, gt_bones = fc._parse_mesh_primitives(pay, e["type"])
    assert gt_bones, "%s: no bones decoded" % MESH
    gt_skin = {pi: fc._prim_skin(p, len(gt_bones))
               for pi, p in enumerate(prims) if p["abc"] > 0}
    assert gt_skin, "%s: no skinned primitives in payload" % MESH
    slot_name = {b.index: b.name for b in gt_bones}

    res = bpy.ops.import_scene.fable_model(
        filepath=GBIG, entry=MESH, import_all=False, decode_textures=False,
        tools_dir=TOOLS_DIR)
    assert res == {"FINISHED"}, "import failed for %s" % MESH

    arms = [o for o in bpy.data.objects if o.type == "ARMATURE"]
    assert len(arms) == 1, "%s: expected exactly 1 armature" % MESH
    arm = arms[0]
    bone_names = {b.name for b in arm.data.bones}

    mesh_objs = [o for o in bpy.data.objects
                 if o.type == "MESH" and o.get("fable_prim") is not None]
    assert mesh_objs, "%s: no imported mesh objects" % MESH

    checked = 0
    for obj in mesh_objs:
        pi = int(obj["fable_prim"])
        if pi not in gt_skin:
            continue  # static primitive: no skin expected
        skin = gt_skin[pi]
        me = obj.data

        # 1) vertex groups exist and are all real bone names
        assert len(obj.vertex_groups) > 0, "%s prim%d: no vertex groups" % (MESH, pi)
        for vg in obj.vertex_groups:
            assert vg.name in bone_names, \
                "%s prim%d: group %r is not an armature bone" % (MESH, pi, vg.name)

        # 2) Armature modifier bound to the imported armature, mesh parented
        mods = [m for m in obj.modifiers if m.type == "ARMATURE"]
        assert mods and mods[0].object == arm, \
            "%s prim%d: no Armature modifier bound to the armature" % (MESH, pi)
        assert obj.parent == arm, "%s prim%d: mesh not parented to armature" % (MESH, pi)

        # 3) every vertex assigned; weights sum ~1.0; values match ground truth
        gi_name = {vg.index: vg.name for vg in obj.vertex_groups}
        for v in me.vertices:
            assert len(v.groups) >= 1, \
                "%s prim%d: vertex %d has no group" % (MESH, pi, v.index)
            s = sum(g.weight for g in v.groups)
            assert abs(s - 1.0) < 1e-3, \
                "%s prim%d: vertex %d weights sum %.4f" % (MESH, pi, v.index, s)
            got = {gi_name[g.group]: g.weight for g in v.groups}
            want = {}
            for (slot, w) in skin[v.index]:
                if w > 0.0:
                    want[slot_name[slot]] = want.get(slot_name[slot], 0.0) + w
            assert set(got) == set(want), \
                "%s prim%d v%d: bones %r != decoded %r" % (
                    MESH, pi, v.index, sorted(got), sorted(want))
            for name, w in want.items():
                assert abs(got[name] - w) < 1e-4, \
                    "%s prim%d v%d %s: %.5f != %.5f" % (MESH, pi, v.index, name,
                                                        got[name], w)
        checked += 1
        print("%s prim%d: %d verts, %d groups -- weights match decode" %
              (MESH, pi, len(me.vertices), len(obj.vertex_groups)))

    assert checked == len(gt_skin), \
        "%s: checked %d skinned prims, payload has %d" % (MESH, checked, len(gt_skin))

    os.makedirs(OUT, exist_ok=True)
    blend_out = os.path.join(OUT, "fable_skin_%s.blend" % MESH)
    bpy.ops.wm.save_as_mainfile(filepath=blend_out)
    print("saved:", blend_out)
    print("%s: PASS (%d skinned primitive(s), armature %d bones)" %
          (MESH, checked, len(arm.data.bones)))

print("\nBLENDER SKIN TEST: ALL ASSERTIONS PASSED")
