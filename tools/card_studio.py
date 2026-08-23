#!/usr/bin/env python3
"""Card Studio orchestrator — one command builds + deploys a full quest-card reskin
from a project JSON. This is the painless backend the FableForge GUI calls: import a
mesh (with orientation/scale baked in), reskin textures (AI or import), set the card
model, edit text, write the ForgeFSE quest, deploy (with backup), and optionally launch.

Usage: python tools/card_studio.py <project.json>

project.json (all sections optional):
{
  "quest": "MELEE",
  "objectName": "OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES",
  "gameRoot": "C:/.../Fable The Lost Chapters",
  "forge": "D:/Code/FableForge/build/forge.exe",
  "schema": "D:/Code/FableForge/docs/re_reference/def_schema.json",
  "blender": "C:/Programs/Blender/blender.exe",
  "work": "C:/.../scratch",                     # working dir for staged .big
  "mesh": {"path": "D:/.../HeadSpider.fbx",      # .fbx/.glb/.obj (textures auto-detected)
           "name": "MESH_HEADSPIDER",
           "rot": [0,0,0], "scale": 1.0},        # degrees XYZ + uniform scale (baked)
  "cardModelId": 8113,                           # used if no mesh section
  "textures": [ {"bank":"textures","entry":"HUD_ORB_QUEST_CORE","image":"orb.png"},
                {"bank":"frontend","entry":"MINIMAP_GUILDWOODS_FRONT_END","prompt":"...","ref":"..."} ],
  "text": {"title":"...", "summary":"...", "objective":"..."},
  "rewards": {"gold":20, "renown":30, "xp":150},
  "deploy": true, "launch": false
}
"""
import json, os, sys, subprocess, shutil, math
sys.path.insert(0, os.path.join(os.path.dirname(__file__)))
sys.path.insert(0, os.path.join(os.path.dirname(__file__), 'blender_addon', 'io_scene_fable'))

def log(m): print("[studio] " + m, flush=True)

# Quest-start backdrop atlas: 3 columns x rows, canvas 768x640. The bottom row is
# HALF height (256x128) -- splitting a whole-background image must honor this or the
# tiles distort (esp. the ring in the middle). tile id -> (x, y, w, h).
QUEST_START_LAYOUT = {
    4722: (0, 0, 256, 256),   4723: (256, 0, 256, 256),   4724: (512, 0, 256, 256),
    4725: (0, 256, 256, 256), 4726: (256, 256, 256, 256), 4727: (512, 256, 256, 256),
    4728: (0, 512, 256, 128), 4729: (256, 512, 256, 128), 4730: (512, 512, 256, 128),
}
QUEST_START_CANVAS = (768, 640)

def expand_backdrop(proj, work):
    """proj['backdrop']['image'] -> resize to the atlas canvas and slice into the 9
    quest-start tiles, appending each as a texture edit. 'draw the whole background
    and it splits accordingly.'"""
    bd = proj.get("backdrop") or {}
    img = bd.get("image")
    if not img and bd.get("prompt"):   # AI-generate the whole background, then split
        img = os.path.join(work, "_bd_ai.png")
        subprocess.run([sys.executable, os.path.join(os.path.dirname(__file__), "ai_texture.py"),
                        bd["prompt"], "--out", img, "--size", "1024x1024",
                        "--final-size", "768", "--quality", "medium"], check=True, env={**os.environ})
        log("backdrop AI-generated from prompt")
    if not img:
        return
    from PIL import Image
    cw, ch = QUEST_START_CANVAS
    # align: [dx, dy] in canvas px. POSITIVE dx shifts the whole background LEFT,
    # positive dy shifts it UP (compensates the in-game right/down offset of the
    # sprite quads). Source is resized OVERSIZED so the shift pulls in real content
    # at the exposed edge instead of a black/stretched band.
    ox, oy = (bd.get("align") or [0, 0])[:2]
    ox, oy = int(ox), int(oy)
    if ox or oy:
        big = Image.open(img).convert("RGB").resize((cw + 2 * abs(ox), ch + 2 * abs(oy)), Image.LANCZOS)
        x0, y0 = abs(ox) + ox, abs(oy) + oy
        src = big.crop((x0, y0, x0 + cw, y0 + ch))
        log("backdrop shifted by align=[%d,%d] (px: +=left/up)" % (ox, oy))
    else:
        src = Image.open(img).convert("RGB").resize(QUEST_START_CANVAS, Image.LANCZOS)
    proj.setdefault("textures", [])
    # Tiles stay FULLY OPAQUE (custom paint). The card's live render viewport is handled by the
    # mesh's backing quad (import_mesh), NOT by tile alpha — applying retail alpha here just
    # re-exposes the garble (empirically confirmed). So: opaque spider everywhere.
    for tid, (x, y, w, h) in QUEST_START_LAYOUT.items():
        p = os.path.join(work, "_bd_%d.png" % tid)
        src.crop((x, y, x + w, y + h)).save(p)
        proj["textures"].append({"bank": "textures", "id": tid, "image": p})
    log("backdrop split into 9 opaque quest-start tiles (%dx%d atlas)" % QUEST_START_CANVAS)

# ---- geometry transform (bakes orientation/scale into the mesh) --------------
def _rot_matrix(rx, ry, rz):
    import numpy as np
    rx, ry, rz = math.radians(rx), math.radians(ry), math.radians(rz)
    cx, sx = math.cos(rx), math.sin(rx); cy, sy = math.cos(ry), math.sin(ry); cz, sz = math.cos(rz), math.sin(rz)
    Rx = np.array([[1,0,0],[0,cx,-sx],[0,sx,cx]])
    Ry = np.array([[cy,0,sy],[0,1,0],[-sy,0,cy]])
    Rz = np.array([[cz,-sz,0],[sz,cz,0],[0,0,1]])
    return Rz @ Ry @ Rx

def import_mesh(proj):
    """Blender-extract the mesh, bake rot/scale, compose, add to graphics.big.
    Returns (new_graphics_big_path, mesh_id, {tex diffuse ids})."""
    import numpy as np, texture_build as tb
    import mesh_rw, big_write, parse_bigb
    m = proj["mesh"]; root = proj["gameRoot"]; work = proj["work"]
    name = m.get("name", "MESH_CUSTOM")
    geo = os.path.join(work, "_studio_geo.json")
    ext = os.path.splitext(m["path"])[1].lower()
    if ext in (".fbx", ".glb", ".gltf"):
        blender = proj.get("blender", r"C:/Programs/Blender/blender.exe")
        script = proj.get("extractScript") or os.path.join(os.path.dirname(__file__), "fbx_extract.py")
        if not os.path.exists(script):
            raise SystemExit("mesh import needs fbx_extract.py (extractScript) for %s" % ext)
        subprocess.run([blender, "--background", "--python", script, "--", m["path"], geo], check=True)
    elif ext == ".obj":
        import trimesh
        tm = trimesh.load(m["path"], process=False)
        prims = []
        meshes = tm.geometry.values() if hasattr(tm, "geometry") else [tm]
        for me in meshes:
            uv = me.visual.uv if hasattr(me.visual, "uv") and me.visual.uv is not None else None
            verts = []
            for tri in me.faces:
                for vi in tri:
                    v = me.vertices[vi]; n = me.vertex_normals[vi]
                    u = uv[vi] if uv is not None else (0.0, 0.0)
                    verts.append([v[0], v[1], v[2], u[0], u[1], n[0], n[1], n[2]])
            prims.append({"verts": verts, "faces": [[i, i+1, i+2] for i in range(0, len(verts), 3)]})
        json.dump({"prims": prims}, open(geo, "w"))
    else:
        raise SystemExit("unsupported mesh format: " + ext)

    g = json.load(open(geo))
    R = _rot_matrix(*m.get("rot", [0, 0, 0])); s = float(m.get("scale", 1.0))
    prims = []
    for p in g["prims"]:
        vb = np.array(p["verts"], dtype=np.float64)
        pos = (vb[:, 0:3] @ R.T) * s
        nrm = vb[:, 5:8] @ R.T
        prims.append({"verts": [tuple(x) for x in pos],
                      "uvs": [(r[3], r[4]) for r in p["verts"]],
                      "normals": [tuple(x) for x in nrm],
                      "faces": [tuple(f) for f in p["faces"]],
                      "mat": 1 if "eye" in p.get("name", "").lower() else 0})
    # auto-detect sibling textures (diffuse + eyes)
    texids = {}
    tb_path = os.path.join(root, "data", "graphics", "pc", "textures.big")
    out_tb = os.path.join(work, "textures.big")
    src = tb_path
    mdir = os.path.dirname(m["path"])
    for role, patt in (("body", ("headspider", "diffuse", "albedo", "body")), ("eyes", ("eye",))):
        for fn in sorted(os.listdir(mdir)):
            if fn.lower().endswith(".png") and any(k in fn.lower() for k in patt):
                buf, parsed = tb.load_big(src)
                rgba = tb.load_image_rgba(os.path.join(mdir, fn))
                rh, rw = rgba.shape[:2]; aw, ah = tb._pow2_up(rw), tb._pow2_up(rh)
                rgba = tb.fit_to_alloc(rgba, aw, ah)
                payload, info = tb.build_entry(rgba, tb.FMT_DXT3, real_w=rw, real_h=rh)
                mod = big_write.rebuild(buf, adds=[{"sub": "GBANK_MAIN_PC", "name": "TEX_" + name + "_" + role.upper(),
                                                    "payload": payload, "type": 0, "info": info}])
                open(out_tb, "wb").write(mod); src = out_tb
                _, np2 = tb.load_big(out_tb)
                for su, es in np2:
                    for e in es:
                        if e["name"] == "TEX_" + name + "_" + role.upper(): texids[role] = e["id"]
                log("imported texture %s -> id %d" % (fn, texids[role]))
                break
    body = texids.get("body", 1354); eyes = texids.get("eyes", body)
    materials = [{"name": name + "_body", "diffuse_id": body},
                 {"name": name + "_eyes", "diffuse_id": eyes}]
    # BACKING QUAD: the card is shown in a live 3D render viewport; a small mesh leaves the
    # border unpainted -> uninitialised VRAM shows as a garbled rectangle. A head-centered
    # opaque quad BEHIND the head fills the whole viewport (every pixel painted) -> no garble,
    # while the 3D head still renders in front. (RE-verified: retail cards ARE a viewport quad.)
    if m.get("backingQuad", True):
        allv = np.array([v for p in prims for v in p["verts"]])
        mn, mx = allv.min(0), allv.max(0)
        cx, cy = (mn[0] + mx[0]) / 2.0, (mn[1] + mx[1]) / 2.0
        half = max(mx[0] - mn[0], mx[1] - mn[1], 42.0) / 2.0 + 4.0   # >= retail card footprint (~42)
        zq = float(mn[2]) - 0.5                                       # entirely behind the head
        prims.append({"verts": [(cx - half, cy - half, zq), (cx + half, cy - half, zq),
                                 (cx + half, cy + half, zq), (cx - half, cy + half, zq)],
                      "uvs": [(0.0, 0.0), (1.0, 0.0), (1.0, 1.0), (0.0, 1.0)],
                      "normals": [(0.0, 0.0, 1.0)] * 4,        # faces +Z toward the camera
                      "faces": [(0, 1, 2), (0, 2, 3)], "mat": 2})
        materials.append({"name": name + "_backquad", "diffuse_id": body,
                          "two_sided": True, "transparent": False})   # opaque = paints every pixel
        log("added viewport backing quad (half=%.1f z=%.2f) -> kills card garble" % (half, zq))
    payload, info = mesh_rw.compose_mesh(name, prims, materials)
    src_g = os.path.join(root, "data", "graphics", "graphics.big")
    out_g = os.path.join(work, "graphics.big")
    buf = open(src_g, "rb").read()
    mod = big_write.rebuild(buf, adds=[{"sub": "MBANK_ALLMESHES", "name": name,
                                        "payload": payload, "info": info, "type": 1}])
    open(out_g, "wb").write(mod)
    mid = None
    _, _, fo, _ = parse_bigb.parse_header(mod); subs, _ = parse_bigb.parse_footer(mod, fo)
    for su in subs:
        es, _, _, _ = parse_bigb.parse_toc(mod, su, fo)
        for e in es:
            if e["name"] == name: mid = e["id"]
    log("composed %s -> graphics.big id %d (rot=%s scale=%.2f)" % (name, mid, m.get("rot"), s))
    return out_g if texids else out_g, mid, (out_tb if texids else None)

# ---- texture reskins (AI or import) -----------------------------------------
def reskin_textures(proj, base_tb=None):
    import texture_build as tb, parse_texture, big_write, numpy as np
    root = proj["gameRoot"]; work = proj["work"]
    banks = {"textures": os.path.join(root, "data", "graphics", "pc", "textures.big"),
             "frontend": os.path.join(root, "data", "graphics", "pc", "frontend.big")}
    outs = {}
    # group edits by bank so each bank rebuilds once
    by_bank = {}
    for t in proj.get("textures", []):
        by_bank.setdefault(t.get("bank", "textures"), []).append(t)
    for bank, edits in by_bank.items():
        src = (base_tb if bank == "textures" and base_tb else banks[bank])
        buf, parsed = tb.load_big(src); mbuf = bytearray(buf); eby = {}
        for t in edits:
            img = t.get("image")
            if not img and t.get("prompt"):
                img = os.path.join(work, "_ai_%s.png" % t["entry"])
                cmd = [sys.executable, os.path.join(os.path.dirname(__file__), "ai_texture.py"),
                       t["prompt"], "--out", img, "--final-size", "256"]
                if t.get("ref"): cmd += ["--ref", t["ref"]]
                subprocess.run(cmd, check=True, env={**os.environ})
            if not img: continue
            # find entry by explicit id or by (exact/suffix) name
            tid = t.get("id"); tname = t.get("entry")
            for s, es in parsed:
                for e in es:
                    if (tid is not None and e["id"] == tid) or \
                       (tname and (e["name"] == tname or e["name"].endswith(tname))):
                        old = parse_texture.parse_info(e["info"])
                        rgba = tb.fit_to_alloc(tb.load_image_rgba(img), old["alloc_w"], old["alloc_h"])
                        payload, info = tb.build_entry(rgba, old["fmt"], mips=old["mipcount"],
                                                       real_w=old["width"], real_h=old["height"], transparency=e["info"][16])
                        ioff = tb.info_abs_offset(buf, e); mbuf[ioff:ioff+len(info)] = info
                        eby[(s["name"], e["name"])] = payload; log("reskinned %s" % e["name"])
        out = os.path.join(work, os.path.basename(banks[bank]))
        open(out, "wb").write(big_write.rebuild(bytes(mbuf), edits=eby)); outs[bank] = out
    return outs

# ---- forge CLI edits (set-cardmodel, text) ----------------------------------
def forge(proj, *args):
    return subprocess.run([proj["forge"], *args], check=True, capture_output=True, text=True).stdout

def main():
    proj = json.load(open(sys.argv[1]))
    work = proj["work"]; os.makedirs(work, exist_ok=True)
    staged = {}   # rel-path -> built file

    base_tb = None
    if proj.get("mesh"):
        g_big, mid, tb_out = import_mesh(proj)
        staged["data/graphics/graphics.big"] = g_big
        if tb_out: base_tb = tb_out
        proj["cardModelId"] = mid

    expand_backdrop(proj, work)   # one whole-background image -> the 9 quest-start tiles
    tex_outs = reskin_textures(proj, base_tb)
    for bank, path in tex_outs.items():
        rel = "data/graphics/pc/%s.big" % ("textures" if bank == "textures" else "frontend")
        staged[rel] = path
    if base_tb and "data/graphics/pc/textures.big" not in staged:
        staged["data/graphics/pc/textures.big"] = base_tb

    # card model + text via forge (writes into an --out dir cumulatively)
    outroot = os.path.join(work, "defs_out"); shutil.rmtree(outroot, ignore_errors=True)
    if proj.get("cardModelId") and proj.get("objectName"):
        forge(proj, "ui", "set-cardmodel", proj["gameRoot"], proj["objectName"],
              str(proj["cardModelId"]), "--out", outroot, "--schema", proj["schema"])
    tx = proj.get("text", {})
    tx_big = os.path.join(proj["gameRoot"], "data", "lang", "English", "text.big")
    tx_out = os.path.join(work, "text.big"); made_text = False
    q = proj["quest"]
    prefix = proj.get("textPrefix", q)  # existing quests use their own symbol prefix
    for key, sym in (("title", "TITLE"), ("summary", "SUMMARY")):
        if tx.get(key):
            src = tx_out if made_text else tx_big
            args = ["text", "set", src, "TEXT_QUEST_%s_%s" % (prefix, sym), tx[key]]
            args += (["--in-place"] if made_text else ["--out", tx_out])
            forge(proj, *args); made_text = True
    if made_text: staged["data/lang/English/text.big"] = tx_out

    # game.bin from set-cardmodel out
    if os.path.exists(os.path.join(outroot, "data", "CompiledDefs", "game.bin")):
        staged["data/CompiledDefs/game.bin"] = os.path.join(outroot, "data", "CompiledDefs", "game.bin")
        staged["data/CompiledDefs/names.bin"] = os.path.join(outroot, "data", "CompiledDefs", "names.bin")

    # ForgeFSE quest (rewards/objective) — painless auto-registration.
    # Skip for pure asset reskins on an existing/real quest (no companion needed).
    if proj.get("skipQuest"):
        log("skipQuest set — no companion ForgeFSE quest written (pure asset reskin)")
    else:
        rw = proj.get("rewards", {})
        fse = os.path.join(proj["gameRoot"], "FSE", q); os.makedirs(fse, exist_ok=True)
        lua = "\n".join([
            "-- %s.lua (Card Studio, auto-generated)" % q, "Quest = nil",
            "function Init(o) Quest=o; Quest:SetStateBool('done',false) end",
            "function Main(o)", "  Quest=o",
            "  Quest:AddQuestCard(\"%s\", \"%s\", false, false)" % (proj.get("objectName", ""), q),
            "  Quest:SetQuestGoldReward(\"%s\", %d)" % (q, rw.get("gold", 0)),
            "  Quest:SetQuestRenownReward(\"%s\", %d)" % (q, rw.get("renown", 0)),
            "  Quest:KickOffQuestStartScreen(\"%s\", true, true)" % q, "end",
            "function OnPersist(o,c) Quest=o; Quest:PersistTransferBool(c,'done') end"])
        open(os.path.join(fse, q + ".lua"), "w").write(lua)
        log("wrote ForgeFSE quest FSE/%s/%s.lua (auto-registered; .qst self-heals)" % (q, q))

    # deploy (backup + copy) into the install
    if proj.get("deploy"):
        for rel, built in staged.items():
            dst = os.path.join(proj["gameRoot"], rel.replace("/", os.sep))
            bak = dst + ".retail-bak"
            if os.path.exists(dst) and not os.path.exists(bak): shutil.copy2(dst, bak)
            os.makedirs(os.path.dirname(dst), exist_ok=True); shutil.copy2(built, dst)
            log("deployed %s" % rel)
    print(json.dumps({"ok": True, "staged": list(staged.keys()),
                      "cardModelId": proj.get("cardModelId")}))
    if proj.get("launch"):
        exe = os.path.join(proj["gameRoot"], "Fable.exe")
        if os.name == "nt": os.startfile(exe)  # noqa
        log("launched Fable")

if __name__ == "__main__":
    main()
