# SPDX-License-Identifier: MIT
"""NEW-MESH COMPOSITION oracle tests + demo CLI (docs/MESH_COMPOSE.md).

Run (plain Python, no Blender):
  python tools/blender_addon/tests/compose_test.py <graphics.big COPY> [out.big]

NEVER point this at the Steam install: it writes a modified .big. It performs:
  1. RECOMPOSE oracle -- decode retail static meshes (bear statue etc.) into
     geometry+materials, re-compose a full payload with mesh_rw.compose_mesh,
     re-parse with fable_core: verts/faces/UVs must match the original decode
     EXACTLY (packed layout re-quantises through identical params -> lossless).
  1b. RECOMPOSE oracle, SKINNED (type-5) -- goblin / seagull / hero boots:
     skeleton blocks byte-exact, Groups[] palettes identical, per-vertex skin
     decode identical, ghost LOD parses with the skeleton.
  2. SYNTHETIC meshes -- a textured cube ('float' layout) and a grass-blade
     quad cluster ('packed' layout) composed from scratch and re-parsed.
  2b. SYNTHETIC SKINNED -- a re-skinned scaled boot (bone-NAME input, cloned
     donor skeleton, single block) + a 41-bone ribbon (forces the multi-block
     palette partition), verified by face-correspondence.
  3. NEW-ENTRY oracle -- the synthetic meshes are ADDED as new entries to a
     copy of graphics.big (big_write.rebuild adds=..., boot as type 5), the
     copy is re-opened with fable_core.open_graphics_big, and the decoded
     geometry/skeleton/skin is compared against what was authored.
  4. Byte-exact no-edit rebuild regression (big_write with no edits/adds).
"""
import os
import sys
import struct

_THIS = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(_THIS, "..", "io_scene_fable"))
sys.path.insert(0, os.path.normpath(os.path.join(_THIS, "..", "..")))

import fable_core as fc     # noqa: E402
import mesh_rw              # noqa: E402
import big_write            # noqa: E402
import parse_bigb           # noqa: E402

UV_TOL = 1.0 / 2048.0 + 1e-6      # int16 s11e4 UV quantisation step


def _f32(x):
    return struct.unpack('<f', struct.pack('<f', x))[0]


def _cmp_geo(tag, averts, afaces, auvs, bverts, bfaces, buvs, pos_tol=0.0):
    assert len(averts) == len(bverts), "%s: vert count %d != %d" % (tag, len(averts), len(bverts))
    assert afaces == bfaces, "%s: face lists differ" % tag
    for i, (va, vb) in enumerate(zip(averts, bverts)):
        for j in range(3):
            d = abs(va[j] - vb[j])
            assert d <= pos_tol, "%s: vert %d axis %d differs by %g" % (tag, i, j, d)
    for i, (ua, ub) in enumerate(zip(auvs, buvs)):
        assert abs(ua[0] - ub[0]) <= UV_TOL and abs(ua[1] - ub[1]) <= UV_TOL, \
            "%s: uv %d differs (%r vs %r)" % (tag, i, ua, ub)


def _loop_to_vert(faces, loop_uv, nverts):
    uvs = [(0.0, 0.0)] * nverts
    for fi, f in enumerate(faces):
        for k in range(3):
            uvs[f[k]] = loop_uv[3 * fi + k]
    return uvs


def _skin_as_dicts(skin):
    return [{s: w for (s, w) in infl} for infl in skin]


def _cmp_skin(tag, askin, bskin):
    a, b = _skin_as_dicts(askin), _skin_as_dicts(bskin)
    assert len(a) == len(b), "%s: skin length %d != %d" % (tag, len(a), len(b))
    for i, (da, db) in enumerate(zip(a, b)):
        assert set(da) == set(db), \
            "%s: vertex %d bone set %r != %r" % (tag, i, sorted(da), sorted(db))
        for s in da:
            assert abs(da[s] - db[s]) <= 1.0 / 255.0 + 1e-9, \
                "%s: vertex %d slot %d weight %g != %g" % (tag, i, s, da[s], db[s])


def recompose_oracle(buf, meshes, name, compress=True):
    e = fc.find_entry(buf, meshes, name)
    assert e, "entry %s not found" % name
    pay = buf[e['offset']:e['offset'] + e['size']]
    mname, prims, mats, skel = mesh_rw.decode_to_compose_args(pay, e['type'])
    new_pay, new_info = mesh_rw.compose_mesh(mname, prims, mats, compress=compress,
                                             skeleton=skel)

    _n2, prims2, b2 = fc._parse_mesh_primitives(new_pay, e['type'])
    _n1, prims1, b1 = fc._parse_mesh_primitives(pay, e['type'])
    assert _n2 == mname
    assert len(prims2) == len(prims1)
    for pi, (p1, p2) in enumerate(zip(prims1, prims2)):
        v1, f1, u1 = fc._prim_geometry(p1, e['type'])
        v2, f2, u2 = fc._prim_geometry(p2, e['type'])
        # loop UVs -> per-vertex for comparison (compose stores per-vertex)
        _cmp_geo('%s prim%d' % (name, pi), v1, f1, u1, v2, f2, u2, pos_tol=0.0)
        if skel is not None and p1['abc'] > 0:
            assert p2['abc'] > 0, "%s prim%d: lost animated blocks" % (name, pi)
            assert [ab[4] for ab in p1['ablocks']] == [ab[4] for ab in p2['ablocks']], \
                "%s prim%d: Groups[] palettes changed" % (name, pi)
            s1 = fc._prim_skin(p1, len(b1))
            s2 = fc._prim_skin(p2, len(b2))
            _cmp_skin('%s prim%d' % (name, pi), s1, s2)
    if skel is not None:
        # skeleton blocks must survive BYTE-EXACT (they are cloned raw)
        skel2 = mesh_rw.clone_skeleton(new_pay, e['type'])
        for k in ('count', 'indices_raw', 'names_raw', 'bones_raw',
                  'keyframes_raw', 'ibms_raw', 'names'):
            assert skel2[k] == skel[k], "%s: skeleton block %s changed" % (name, k)
        # decoded bones identical (names, hierarchy, bind matrices, global ids)
        assert len(b1) == len(b2)
        for x, y in zip(b1, b2):
            assert (x.name, x.parent, x.global_id, x.matrix) == \
                   (y.name, y.parent, y.global_id, y.matrix), \
                   "%s: bone %s decode changed" % (name, x.name)
        # the ghost LOD must itself parse and carry the skeleton (type-5 grammar)
        lod0_len = struct.unpack_from('<I', new_info, 48)[0]
        _gn, gprims, gbones = fc._parse_mesh_primitives(new_pay[lod0_len:], e['type'])
        assert _gn == mname and not gprims and gbones and len(gbones) == skel['count']
    # materials survive: re-decode and compare texture bindings
    m3 = mesh_rw.decode_to_compose_args(new_pay, e['type'])[2]
    assert [m['diffuse_id'] for m in m3] == [m['diffuse_id'] for m in mats], \
        "%s: material texture ids changed" % name
    print("  RECOMPOSE OK  %-28s prims=%d bones=%s orig=%dB recomposed=%dB info=%dB"
          % (name, len(prims1), skel['count'] if skel else 0, len(pay),
             len(new_pay), len(new_info)))
    return new_pay, new_info


def make_cube(size=0.5, tex_id=333):
    """A textured cube: 24 verts (per-face UVs), 12 tris, 'float' layout."""
    s = _f32(size)
    faces_def = [
        ((+1, 0, 0), [(s, -s, -s), (s, s, -s), (s, s, s), (s, -s, s)]),
        ((-1, 0, 0), [(-s, s, -s), (-s, -s, -s), (-s, -s, s), (-s, s, s)]),
        ((0, +1, 0), [(s, s, -s), (-s, s, -s), (-s, s, s), (s, s, s)]),
        ((0, -1, 0), [(-s, -s, -s), (s, -s, -s), (s, -s, s), (-s, -s, s)]),
        ((0, 0, +1), [(-s, -s, s), (s, -s, s), (s, s, s), (-s, s, s)]),
        ((0, 0, -1), [(-s, s, -s), (s, s, -s), (s, -s, -s), (-s, -s, -s)]),
    ]
    verts, uvs, normals, faces = [], [], [], []
    quad_uv = [(0.0, 0.0), (1.0, 0.0), (1.0, 1.0), (0.0, 1.0)]
    for n, quad in faces_def:
        b = len(verts)
        verts += quad
        uvs += quad_uv
        normals += [n] * 4
        faces += [(b, b + 1, b + 2), (b, b + 2, b + 3)]
    prim = dict(verts=verts, faces=faces, uvs=uvs, normals=normals, mat=0)
    mats = [dict(name='CubeMat', diffuse_id=tex_id, two_sided=False)]
    return 'MESH_FORGE_TEST_CUBE', [prim], mats


def make_grass(tex_id=333, blades=5):
    """A grass-blade cluster: `blades` crossed quads, 'packed' layout,
    two-sided transparent material -- the foliage-authoring shape."""
    import math
    verts, uvs, faces = [], [], []
    w, h = _f32(0.08), _f32(0.6)
    for i in range(blades):
        ang = math.pi * i / blades
        dx, dy = _f32(math.cos(ang) * w), _f32(math.sin(ang) * w)
        ox, oy = _f32(math.cos(ang + 1.3) * 0.1), _f32(math.sin(ang + 1.3) * 0.1)
        b = len(verts)
        verts += [(ox - dx, oy - dy, 0.0), (ox + dx, oy + dy, 0.0),
                  (ox + dx, oy + dy, h), (ox - dx, oy - dy, h)]
        uvs += [(0.0, 0.0), (1.0, 0.0), (1.0, 1.0), (0.0, 1.0)]
        faces += [(b, b + 1, b + 2), (b, b + 2, b + 3)]
    prim = dict(verts=verts, faces=faces, uvs=uvs, mat=0, layout='packed')
    mats = [dict(name='GrassMat', diffuse_id=tex_id, two_sided=True,
                 transparent=True)]
    return 'MESH_FORGE_TEST_GRASS', [prim], mats


def make_skinned_boot(buf, meshes, donor='MESH_HERO_APPRENTICE_BOOT_L_01',
                      scale=1.15):
    """The custom-equipment shape: DONOR boot geometry, modified (scaled),
    re-skinned to the SAME hero-rig bone subset via bone NAMES, composed with
    the donor's cloned skeleton. Returns (name, prims, mats, skeleton,
    authored_skin_by_name)."""
    e = fc.find_entry(buf, meshes, donor)
    assert e and e['type'] == 5, "donor %s not found / not type-5" % donor
    pay = buf[e['offset']:e['offset'] + e['size']]
    _nm, prims, mats, skel = mesh_rw.decode_to_compose_args(pay, e['type'])
    assert skel is not None and prims and prims[0].get('skin')
    slot_name = {i: n for i, n in enumerate(skel['names'])}
    new_prims = []
    skin_by_name = []
    for p in prims:
        p = dict(p)
        p['verts'] = [(x * scale, y * scale, z * scale) for (x, y, z) in p['verts']]
        # re-skin by bone NAME (the authoring-path input format)
        named = [[(slot_name[s], w) for (s, w) in infl] for infl in p['skin']]
        p['skin'] = named
        p.pop('skin_blocks', None)      # force the auto block builder
        p.pop('comp_scale', None); p.pop('comp_off', None)
        skin_by_name.append(named)
        new_prims.append(p)
    return 'MESH_FORGE_TEST_BOOT', new_prims, mats, skel, skin_by_name


def make_multiblock_ribbon(skel, slots_used=40):
    """A synthetic skinned ribbon touching `slots_used` bones (>16), forcing the
    EgoCore-style multi-block palette partition + vertex duplication."""
    verts, uvs, faces, skin = [], [], [], []
    for i in range(slots_used + 1):
        x = _f32(i * 0.05)
        verts += [(x, 0.0, 0.0), (x, 0.1, 0.0)]
        uvs += [(i / slots_used, 0.0), (i / slots_used, 1.0)]
        slot = min(i, slots_used - 1) % skel['count']
        nslot = (slot + 1) % skel['count']
        skin += [[(slot, 0.75), (nslot, 0.25)]] * 2
        if i:
            b = 2 * (i - 1)
            faces += [(b, b + 1, b + 2), (b + 1, b + 3, b + 2)]
    prim = dict(verts=verts, faces=faces, uvs=uvs, skin=skin, mat=0)
    mats = [dict(name='RibbonMat', diffuse_id=807)]
    return 'MESH_FORGE_TEST_RIBBON', [prim], mats


def check_synth_skinned(name, prims, mats, skel, entry_type=5):
    """Compose a synthetic SKINNED mesh and re-parse it standalone. The auto
    block builder may reorder/duplicate vertices (multi-block), so skin +
    geometry are checked via FACE-CORRESPONDENCE against the authored data."""
    pay, info = mesh_rw.compose_mesh(name, prims, mats, skeleton=skel)
    n2, prims2, bones2 = fc._parse_mesh_primitives(pay, entry_type)
    assert n2 == name
    assert bones2 and len(bones2) == skel['count'], "skeleton not emitted"
    assert len(prims2) == len(prims)
    name_slot = {n: i for i, n in enumerate(skel['names'])}
    total_blocks = 0
    for pi, (pa, p2) in enumerate(zip(prims, prims2)):
        assert p2['abc'] >= 1, "%s prim%d: no animated blocks" % (name, pi)
        total_blocks += p2['abc']
        for ab in p2['ablocks']:
            assert ab[4] and len(ab[4]) <= 16, "palette size out of range"
        v2, f2, _u2 = fc._prim_geometry(p2, entry_type)
        s2 = fc._prim_skin(p2, len(bones2))
        assert len(f2) == len(pa['faces']), \
            "%s prim%d: face count %d != %d" % (name, pi, len(f2), len(pa['faces']))
        for fi, (fa, fb) in enumerate(zip(pa['faces'], f2)):
            for k in range(3):
                va, vb = pa['verts'][fa[k]], v2[fb[k]]
                for j in range(3):
                    # float32 storage: half-ulp relative rounding of the
                    # float64-authored coordinate
                    assert abs(va[j] - vb[j]) <= 1e-6 + 1e-6 * abs(va[j]), \
                        "%s prim%d face%d corner%d pos differs" % (name, pi, fi, k)
                want = {}
                for (bone, w) in pa['skin'][fa[k]]:
                    slot = name_slot[bone] if isinstance(bone, str) else bone
                    want[slot] = want.get(slot, 0.0) + w
                tot = sum(want.values())
                want = {s: w / tot for s, w in want.items()}
                got = dict(s2[fb[k]])
                assert set(got) == set(want), \
                    "%s prim%d face%d corner%d bones %r != %r" % (
                        name, pi, fi, k, sorted(got), sorted(want))
                for s in want:
                    assert abs(got[s] - want[s]) <= 1.5 / 255.0, \
                        "%s prim%d face%d corner%d slot %d weight" % (name, pi, fi, k, s)
    print("  SYNTH-SKIN OK %-28s prims=%d ablocks=%d bones=%d payload=%dB"
          % (name, len(prims), total_blocks, skel['count'], len(pay)))
    return pay, info, total_blocks


def check_synth(name, prims, mats, entry_type=1):
    """Compose a synthetic mesh and re-parse it standalone (pre-container)."""
    pay, info = mesh_rw.compose_mesh(name, prims, mats)
    n2, prims2, bones2 = fc._parse_mesh_primitives(pay, entry_type)
    assert n2 == name and bones2 is None
    assert len(prims2) == len(prims)
    for pi, (pa, p2) in enumerate(zip(prims, prims2)):
        v2, f2, u2 = fc._prim_geometry(p2, entry_type)
        tol = 0.0
        if pa.get('layout') == 'packed':
            # POSPACKED3 quantisation: 1 lsb of the 11/11/10 grid
            _mn, _mx = mesh_rw._bbox([pa['verts']])
            tol = max(max(_mx[j] - _mn[j] for j in range(3)) * 0.505 * 0.002, 1e-4)
        _cmp_geo('%s prim%d' % (name, pi), pa['verts'], pa['faces'], pa['uvs'],
                 v2, f2, _loop_to_vert(f2, u2, len(v2)), pos_tol=tol)
    print("  SYNTH OK      %-28s verts=%d faces=%d payload=%dB info=%dB"
          % (name, len(prims[0]['verts']), len(prims[0]['faces']), len(pay), len(info)))
    return pay, info


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(2)
    gbig = sys.argv[1]
    out_big = sys.argv[2] if len(sys.argv) > 2 else os.path.join(
        os.path.dirname(gbig), 'graphics_with_new_meshes.big')
    inst = 'steamapps' in gbig.lower().replace('/', '\\')
    assert not inst, "refusing to run against the Steam install -- use a COPY"

    print("== open %s" % gbig)
    buf, meshes = fc.open_graphics_big(gbig)
    print("   %d compiled meshes" % len(meshes))

    print("== 1. RECOMPOSE oracle (decode -> compose -> re-decode, semantic identity)")
    for nm in ("MESH_OBJECT_STATUE_BEAR", "MESH_OBJECT_PLATE", "MESH_OLD_VILLAGE_SIGN"):
        recompose_oracle(buf, meshes, nm)

    print("== 1b. RECOMPOSE oracle, SKINNED (type-5: skeleton + animated blocks)")
    for nm in ("MESH_CREATURE_GOBLIN", "MESH_SEAGULL_01",
               "MESH_HERO_APPRENTICE_BOOT_L_01", "MESH_HERO_ASSASSIN_BOOT_L_01"):
        recompose_oracle(buf, meshes, nm)

    print("== 2. SYNTHETIC meshes (standalone compose -> parse)")
    cube = make_cube()
    grass = make_grass()
    cube_pay, cube_info = check_synth(*cube)
    grass_pay, grass_info = check_synth(*grass)

    print("== 2b. SYNTHETIC SKINNED meshes")
    boot = make_skinned_boot(buf, meshes)
    boot_pay, boot_info, boot_blocks = check_synth_skinned(*boot[:4])
    assert boot_blocks == 1, "boot re-skin should fit one palette block"
    ge = fc.find_entry(buf, meshes, 'MESH_CREATURE_GOBLIN')
    gskel = mesh_rw.clone_skeleton(
        buf[ge['offset']:ge['offset'] + ge['size']], ge['type'])
    ribbon = make_multiblock_ribbon(gskel)
    _rpay, _rinfo, ribbon_blocks = check_synth_skinned(*ribbon, gskel)
    assert ribbon_blocks >= 3, "ribbon should force a multi-block partition"

    print("== 3. NEW-ENTRY oracle (add to a copy of graphics.big)")
    adds = [
        dict(sub='MBANK_ALLMESHES', name=cube[0], payload=cube_pay,
             type=1, info=cube_info),
        dict(sub='MBANK_ALLMESHES', name=grass[0], payload=grass_pay,
             type=1, info=grass_info),
        dict(sub='MBANK_ALLMESHES', name=boot[0], payload=boot_pay,
             type=5, info=boot_info),
    ]
    rebuilt = big_write.rebuild(buf, adds=adds)
    with open(out_big, 'wb') as f:
        f.write(rebuilt)
    print("   wrote %s (%d -> %d bytes)" % (out_big, len(buf), len(rebuilt)))

    # container-level validation: parse_toc walks CLEAN, histogram bumped
    magic, _v, fo, _fs = parse_bigb.parse_header(rebuilt)
    assert magic == b'BIGB'
    subs, _ = parse_bigb.parse_footer(rebuilt, fo)
    for s in subs:
        entries, stats, endp, err = parse_bigb.parse_toc(rebuilt, s, fo)
        assert err is None, "TOC parse error in %s: %s" % (s['name'], err)
        assert len(entries) == s['entry_count'], "entry_count mismatch in %s" % s['name']
        if s['name'] == 'MBANK_ALLMESHES':
            t1 = dict(stats)[1]
            print("   TOC CLEAN: %s entries=%d type-1 stats=%d" % (s['name'], len(entries), t1))

    buf2, meshes2 = fc.open_graphics_big(out_big)
    for (name, prims, mats), _pay in ((cube, cube_pay), (grass, grass_pay)):
        e2 = fc.find_entry(buf2, meshes2, name)
        assert e2, "new entry %s not found by open_graphics_big" % name
        pay2 = buf2[e2['offset']:e2['offset'] + e2['size']]
        _n, prims2, _b = fc._parse_mesh_primitives(pay2, e2['type'])
        for pi, (pa, p2) in enumerate(zip(prims, prims2)):
            v2, f2, u2 = fc._prim_geometry(p2, e2['type'])
            tol = 0.0
            if pa.get('layout') == 'packed':
                _mn, _mx = mesh_rw._bbox([pa['verts']])
                tol = max(max(_mx[j] - _mn[j] for j in range(3)) * 0.505 * 0.002, 1e-4)
            _cmp_geo('%s@bank prim%d' % (name, pi), pa['verts'], pa['faces'],
                     pa['uvs'], v2, f2, _loop_to_vert(f2, u2, len(v2)), pos_tol=tol)
        # the full model path (descriptor + materials) must also work
        model = fc.build_model(buf2, e2, tex_bank=None, decode_textures=False)
        assert model.lods and model.lods[0].submeshes, "%s: no submeshes via build_model" % name
        geo = model.lods[0].submeshes[0].geometry
        assert geo and len(geo.vertices) == len(prims[0]['verts'])
        assert [m.base_tex_id for m in model.materials] == \
               [m['diffuse_id'] for m in mats], "%s: Info texture ids wrong" % name
        print("   NEW-ENTRY OK  %-24s id=%d decoded verts=%d faces=%d mats=%s"
              % (name, e2['id'], len(geo.vertices), len(geo.faces),
                 [m.base_tex_id for m in model.materials]))

    # skinned new entry: full model path must yield skeleton + per-vertex skin
    bname, bprims, _bm, bskel, _bskin = boot
    eb = fc.find_entry(buf2, meshes2, bname)
    assert eb and eb['type'] == 5, "skinned new entry not found as type 5"
    model = fc.build_model(buf2, eb, tex_bank=None, decode_textures=False)
    assert model.has_skeleton and len(model.bones) == bskel['count'], \
        "skinned new entry: wrong bone count via build_model"
    assert [b.name for b in model.bones] == bskel['names']
    sm = model.lods[0].submeshes[0]
    assert sm.geometry and sm.geometry.skin, "skinned new entry: no skin decoded"
    assert len(sm.geometry.skin) == len(bprims[0]['verts'])
    name_slot = {n: i for i, n in enumerate(bskel['names'])}
    for v, infl in enumerate(sm.geometry.skin):
        want = {name_slot[b]: w for (b, w) in bprims[0]['skin'][v]}
        tot = sum(want.values())
        want = {s: w / tot for s, w in want.items()}
        got = dict(infl)
        assert set(got) == set(want), "skinned@bank v%d bone set differs" % v
        for s in want:
            assert abs(got[s] - want[s]) <= 1.5 / 255.0
    print("   NEW-ENTRY OK  %-24s id=%d type=5 bones=%d skinned verts=%d"
          % (bname, eb['id'], len(model.bones), len(sm.geometry.skin)))

    print("== 4. no-edit rebuild regression (big_write contract: every entry's")
    print("      record fields + payload bytes round-trip; file layout may repack)")
    plain = big_write.rebuild(buf)
    m0, _v0, fo0, _ = parse_bigb.parse_header(buf)
    m1, _v1, fo1, _ = parse_bigb.parse_header(plain)
    subs0, _ = parse_bigb.parse_footer(buf, fo0)
    subs1, _ = parse_bigb.parse_footer(plain, fo1)
    assert [s['name'] for s in subs0] == [s['name'] for s in subs1]
    for s0, s1 in zip(subs0, subs1):
        e0, st0, _, err0 = parse_bigb.parse_toc(buf, s0, fo0)
        e1, st1, _, err1 = parse_bigb.parse_toc(plain, s1, fo1)
        assert err0 is None and err1 is None and st0 == st1
        assert len(e0) == len(e1) == s0['entry_count'] == s1['entry_count']
        for a, b in zip(e0, e1):
            for k in ('id', 'type', 'size', 'crc', 'name', 'timestamp', 'deps', 'info'):
                assert a[k] == b[k], "no-edit rebuild changed %s of %s" % (k, a['name'])
            assert buf[a['offset']:a['offset'] + a['size']] == \
                plain[b['offset']:b['offset'] + b['size']], \
                "no-edit rebuild changed payload of %s" % a['name']
    print("   no-edit content round-trip OK (%d entries)" %
          sum(s['entry_count'] for s in subs0))

    print("\nCOMPOSE TEST: ALL ASSERTIONS PASSED")


if __name__ == '__main__':
    main()
