"""
Step 1: recompose the SKINNED Meshy mesh under the CUSTOM name MESH_MESHY_HUNTER
into a COPY of graphics.big, and report its assigned MBANK_ALLMESHES entry id.

This is the mesh the new CREATURE_MESHY_HUNTER def's Graphic.modelId will point at.
Identical pipeline to work/meshy_npc_mesh/skinned/compose_inject_verify.py, only the
entry name changes (MESH_MESHY_TRADER_SKINNED -> MESH_MESHY_HUNTER) and we print id.

Output: <WORK>/graphics_meshy_hunter.big  (+ prints the id to stdout & writes mesh_id.txt)
Run:  python 01_recompose_mesh.py
"""
import os, sys, json

REPO = "D:/Documents/FableTLC"
for p in (os.path.join(REPO, "tools/blender_addon/io_scene_fable"),
          os.path.join(REPO, "tools")):
    sys.path.insert(0, p)
import fable_core as fc
import mesh_rw
import big_write

SCRATCH = ("C:/Users/Cornelio/AppData/Local/Temp/claude/"
           "D--Documents-FableTLC/8814901e-8027-465b-b864-563575216e87/scratchpad")
RETAIL = ("C:/Programs/Steam/steamapps/common/"
          "Fable The Lost Chapters/data/graphics/graphics.big")
WORK = os.path.dirname(os.path.abspath(__file__))
GEO = os.path.join(SCRATCH, "meshy_geo.json")
WEIGHTS = os.path.join(SCRATCH, "weights_trader.json")
OUT = os.path.join(WORK, "graphics_meshy_hunter.big")

DONOR = "MESH_TRADER_01"          # type-5, 63-bone Bip01 biped
NEWNAME = "MESH_MESHY_HUNTER"     # <-- our custom body mesh name
DIFFUSE_ID = 6291                 # TEX_MESHY_TRADER, already in textures.big copy


def main():
    geo = json.load(open(GEO))
    verts = [tuple(v) for v in geo["verts"]]
    faces = [tuple(f) for f in geo["faces"]]
    uvs = [tuple(u) for u in geo["uvs"]]
    normals = [tuple(n) for n in geo["normals"]]
    nv = len(verts)

    W = json.load(open(WEIGHTS))
    op, wt = W["orig_pos"], W["weights"]
    assert len(wt) == nv
    for i in range(nv):
        d = sum((verts[i][j] - op[i][j]) ** 2 for j in range(3))
        assert d < 1e-8, "vertex order mismatch at %d" % i

    buf0, meshes0 = fc.open_graphics_big(RETAIL)
    de = fc.find_entry(buf0, meshes0, DONOR)
    assert de and de["type"] == 5, "donor not type-5"
    donor_pay = buf0[de["offset"]:de["offset"] + de["size"]]
    skel = mesh_rw.clone_skeleton(donor_pay, de["type"])
    bone_names = set(skel["names"])
    assert skel["count"] == 63

    skin, unknown = [], set()
    for v in range(nv):
        infl = []
        for name, w in wt[v]:
            if name in bone_names:
                infl.append((name, float(w)))
            else:
                unknown.add(name)
        if not infl:
            infl = [(skel["names"][0], 1.0)]
        skin.append(infl[:3])
    if unknown:
        print("WARN weights ref bones not in donor:", unknown)

    prim = dict(verts=verts, faces=faces, uvs=uvs, normals=normals, mat=0, skin=skin)
    mats = [dict(name="MeshyHunterMat", diffuse_id=DIFFUSE_ID)]
    payload, info = mesh_rw.compose_mesh(NEWNAME, [prim], mats, skeleton=skel)
    print("composed type-5 payload=%dB info=%dB" % (len(payload), len(info)))

    adds = [dict(sub="MBANK_ALLMESHES", name=NEWNAME, payload=payload,
                 type=5, info=info)]
    rebuilt = big_write.rebuild(buf0, adds=adds)
    open(OUT, "wb").write(rebuilt)
    print("wrote", OUT, "(%d bytes)" % len(rebuilt))

    # reopen & verify + capture assigned id
    buf2, meshes2 = fc.open_graphics_big(OUT)
    e2 = fc.find_entry(buf2, meshes2, NEWNAME)
    assert e2 and e2["type"] == 5, "injected entry missing/wrong type"
    model = fc.build_model(buf2, e2, tex_bank=None, decode_textures=False)
    assert model.has_skeleton, "no skeleton"
    assert len(model.bones) == skel["count"], "bone count mismatch"
    assert [b.name for b in model.bones] == skel["names"], "bone names differ"

    mesh_id = e2["id"]
    print("VERIFY OK: name=%s  id=%d  type=5  has_skeleton=%s bones=%d"
          % (NEWNAME, mesh_id, model.has_skeleton, len(model.bones)))
    open(os.path.join(WORK, "mesh_id.txt"), "w").write(str(mesh_id))
    print("MESH_ID=%d (written to mesh_id.txt)" % mesh_id)


if __name__ == "__main__":
    main()
