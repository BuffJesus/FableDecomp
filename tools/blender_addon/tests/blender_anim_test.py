# SPDX-License-Identifier: MIT
"""Headless Blender test: ANIM (3DAF) <-> Blender action ROUND-TRIP.

Run:
  blender --background --factory-startup --python \
      tools/blender_addon/tests/blender_anim_test.py -- \
      "<graphics.big (retail install or work copy)>" [out_dir]

For each (creature mesh, retail anim) pair it:
  1. imports the creature via the addon (armature with REAL bind poses),
  2. decodes the retail ANIM entry and builds a Blender action from it
     (anim_ops.build_action_from_anim -- the import direction),
  3. exports the action back over the SAME retail entry as donor
     (anim_ops.export_action_to_image -- the donor-clone export direction),
  4. compares the exported per-frame quat/pos sequences NUMERICALLY against
     the retail entry (quat metric sign-invariant: 1-|dot|; pos: euclidean
     distance in model units), asserting tight tolerances,
  5. re-parses the composed payload with the independent decoder
     (parse_anim_xseq) and validates it clean,
  6. writes a STAGED .big (edit via big_write.rebuild) into out_dir and
     re-verifies the round-trip from THAT file. Never touches the source.
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
import io_scene_fable.anim_ops as ao  # noqa: E402
io_scene_fable.register()
ao._load_anim_tools(TOOLS_DIR)
parse_anim_xseq = ao.parse_anim_xseq
anim_build = ao.anim_build
big_write = ao.big_write

# (mesh entry, anim entry) pairs -- same Biped rig each
PAIRS = [
    ("MESH_CREATURE_GOBLIN", "ANIM_CREATURE_GOBLIN_ATTACK_01"),
    ("MESH_CREATURE_SPARROW", "ANIM_CREATURE_SPARROW_FLY"),
]

QUAT_TOL = 5e-6      # 1 - |dot| per frame (sign-invariant)
POS_TOL = 2e-3       # model units (cm-ish); i16 requantization + f32 math

b_anim, anim_entries = parse_anim_xseq.load_anim_entries(GBIG)
anim_by_name = {e["name"]: e for e in anim_entries}
# fall back to any entry containing the stem if the exact seagull name differs
for i, (mesh, aname) in enumerate(PAIRS):
    if aname not in anim_by_name:
        stem = aname.replace("ANIM_", "").rsplit("_", 2)[0]
        cands = sorted(n for n in anim_by_name if stem in n)
        assert cands, "no ANIM entries matching %r" % stem
        PAIRS[i] = (mesh, cands[0])
        print("NOTE: %s not found; using %s" % (aname, cands[0]))


def expand(track):
    """Retail per-frame (quats, poss) from a parse_anim_xseq.Track."""
    qs = [track.rot_at(i) for i in range(track.frame_count)] if track.rot_pool else []
    ps = [track.pos_at(i) for i in range(track.frame_count)] if track.pos_pool else []
    return qs, ps


def expand_build(track):
    """Per-frame (quats, poss) from an anim_build.Track (the exported one)."""
    qs, ps = [], []
    rot = track.rot_keys
    if rot:
        pal = track.rot_palette
        for i in range(track.frame_count):
            j = pal[min(i, len(pal) - 1)] if pal else i
            qs.append(rot[min(j, len(rot) - 1)])
    pos = track.pos_keys
    if pos:
        pal = track.pos_palette
        for i in range(track.frame_count):
            j = pal[min(i, len(pal) - 1)] if pal else i
            ps.append(pos[min(j, len(pos) - 1)])
    return qs, ps


def compare_images(retail_payload, exported_a3, label):
    """Numeric compare exported tracks vs the retail decode. Returns
    (max 1-|qdot|, max |dp|)."""
    ra = parse_anim_xseq.parse_anim(retail_payload)
    rby = {t.bone_name: t for t in ra.tracks}
    max_qerr = 0.0
    max_perr = 0.0
    n_rot = n_pos = 0
    for _node, t in exported_a3.tracks():
        rt = rby.get(t.bone_name)
        assert rt is not None, "%s: exported track %r not in retail" % (label, t.bone_name)
        assert t.frame_count == rt.frame_count, \
            "%s/%s: frame_count %d != %d" % (label, t.bone_name,
                                             t.frame_count, rt.frame_count)
        eq, ep = expand_build(t)
        rq, rp = expand(rt)
        assert len(eq) == len(rq), "%s/%s rot frames %d != %d" % (
            label, t.bone_name, len(eq), len(rq))
        assert len(ep) == len(rp), "%s/%s pos frames %d != %d" % (
            label, t.bone_name, len(ep), len(rp))
        for a, r in zip(eq, rq):
            d = abs(sum(x * y for x, y in zip(a, r)))
            max_qerr = max(max_qerr, 1.0 - d)
            n_rot += 1
        for a, r in zip(ep, rp):
            e = sum((x - y) ** 2 for x, y in zip(a, r)) ** 0.5
            max_perr = max(max_perr, e)
            n_pos += 1
    print("%s: compared %d rot frames (max 1-|dot| %.3g), %d pos frames "
          "(max |dp| %.3g units)" % (label, n_rot, max_qerr, n_pos, max_perr))
    assert max_qerr < QUAT_TOL, "%s: quat error %g >= %g" % (label, max_qerr, QUAT_TOL)
    assert max_perr < POS_TOL, "%s: pos error %g >= %g" % (label, max_perr, POS_TOL)
    return max_qerr, max_perr


buf, mesh_entries = fc.open_graphics_big(GBIG, tools_dir=TOOLS_DIR)
os.makedirs(OUT, exist_ok=True)

for MESH, ANIM in PAIRS:
    bpy.ops.wm.read_factory_settings(use_empty=True)

    # 1) import the creature (real bind-pose armature)
    res = bpy.ops.import_scene.fable_model(
        filepath=GBIG, entry=MESH, import_all=False, decode_textures=False,
        tools_dir=TOOLS_DIR)
    assert res == {"FINISHED"}, "import failed for %s" % MESH
    arms = [o for o in bpy.data.objects if o.type == "ARMATURE"]
    assert len(arms) == 1, "%s: expected 1 armature" % MESH
    arm = arms[0]
    assert "REAL bind pose" in arm.get("fable_note", ""), \
        "%s: not the real-bind-pose armature" % MESH

    # 2) retail anim -> Blender action (import direction)
    ae = anim_by_name[ANIM]
    retail_payload = b_anim[ae["offset"]:ae["offset"] + ae["size"]]
    anim = parse_anim_xseq.parse_anim(retail_payload)
    assert anim is not None and anim.tracks, "%s: decode failed" % ANIM
    action, st = ao.build_action_from_anim(arm, anim, name=ANIM)
    n_data_tracks = sum(1 for t in anim.tracks if t.rot_pool or t.pos_pool)
    n_in_rig = sum(1 for t in anim.tracks
                   if (t.rot_pool or t.pos_pool)
                   and arm.pose.bones.get(t.bone_name) is not None)
    print("%s: %d tracks (%d with data, %d in rig) -> action with %d bone "
          "tracks; skipped bones: %s"
          % (ANIM, len(anim.tracks), n_data_tracks, n_in_rig,
             st["tracks"], st["skipped"] or "none"))
    assert st["tracks"] == n_in_rig, "action lost tracks"
    assert st["tracks"] > 0, "no tracks landed in the action"

    # 3) action -> donor-cloned 3DAF image (export direction)
    donor_img = parse_anim_xseq.decompress_payload(retail_payload)
    a3, est = ao.export_action_to_image(arm, action, donor_img)
    print("%s: export replaced %d rot / %d pos tracks, kept %d, missing %s"
          % (ANIM, est["replaced_rot"], est["replaced_pos"], est["kept"],
             est["missing_bones"] or "none"))
    assert est["replaced_rot"] > 0 and est["replaced_pos"] > 0

    # 4) numeric round-trip proof
    compare_images(retail_payload, a3, ANIM)

    # 5) independent decoder validates the composed payload clean
    payload = anim_build.compose_payload(a3.serialize())
    re_anim = parse_anim_xseq.parse_anim(payload)
    assert re_anim is not None and len(re_anim.tracks) == len(anim.tracks), \
        "%s: recompose lost tracks" % ANIM
    v = parse_anim_xseq.validate_anim(re_anim)
    print("%s: recomposed payload validate: %s" % (ANIM, v))
    assert v["pal_oob"] == 0 and v["tail_bytes"] == 0 and v["quat_bad"] == 0, \
        "%s: recomposed payload not clean: %s" % (ANIM, v)

    # 6) staged .big injection + re-verify from disk
    staged = os.path.join(OUT, "fable_anim_roundtrip_%s.big" % ANIM)
    out = big_write.rebuild(b_anim, edits={(ae["bank"], ANIM): payload})
    with open(staged, "wb") as f:
        f.write(out)
    b2, entries2 = parse_anim_xseq.load_anim_entries(staged)
    e2 = [e for e in entries2 if e["name"] == ANIM][0]
    a2 = parse_anim_xseq.parse_anim(b2[e2["offset"]:e2["offset"] + e2["size"]])
    assert a2 is not None and len(a2.tracks) == len(anim.tracks)
    # spot-check first data track quat frame-0 against retail, sign-invariant
    rt0 = next(t for t in anim.tracks if t.rot_pool)
    st0 = next(t for t in a2.tracks if t.bone_name == rt0.bone_name)
    d = abs(sum(x * y for x, y in zip(rt0.rot_at(0), st0.rot_at(0))))
    assert 1.0 - d < QUAT_TOL, "%s: staged .big frame-0 quat off (%g)" % (ANIM, 1 - d)
    os.remove(staged)  # keep the tests dir clean; the numbers above are the proof
    print("%s: staged .big injection + reload OK" % ANIM)

print("\nBLENDER ANIM TEST: ALL ASSERTIONS PASSED")
