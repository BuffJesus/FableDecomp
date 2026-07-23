#!/usr/bin/env python3
"""MBANK mesh/model parser for Fable: TLC graphics.big.

Pure byte-level (NO Ghidra). Decodes the MBANK (graphics.big) mesh entries at the
level the raw bytes support self-consistently:

  * Entry classification (mesh vs animation; compiled-model vs tagged-BBM payload).
  * The per-entry mesh DESCRIPTOR (the TOC `Info` blob): bbox/origin block, LOD
    table, LOD distances, and the material -> textures.big texture-ID bindings.
    This descriptor parses with ZERO leftover on 3294/3295 compiled meshes and its
    material IDs resolve 100% to real textures.big entries (validated).
  * The compiled-model payload HEADER (name, skeleton flag, 10-float origin block,
    helper names, bone-name skeleton) and the per-LOD SUBM sub-headers
    (nVerts, nFaces, nFaceVertexIndices, sVert format code).

NOT decoded (documented gap): the actual packed vertex buffer and the compressed
index stream inside a LOD block. Both use Big Blue Box's proprietary control-byte
compression ("compressed index information" per the fabletlcmod.com wiki); a
byte-exact decode needs the engine's decompressor (Ghidra), not pure byte scanning.

Format reference: docs/BIG_MESH_FORMAT.md (this project) + fabletlcmod.com wiki
(file_formats:bbm). Container reader dependency: tools/parse_bigb.py.

Usage:
  python parse_mesh.py <graphics.big>                 # classify + validate all
  python parse_mesh.py <graphics.big> --entry NAME    # dump one entry
  python parse_mesh.py <graphics.big> --xref <textures.big>   # material xref
"""
import struct, sys, os, collections

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_bigb
from parse_bigb import parse_header, parse_footer


# --------------------------------------------------------------------------
def load_bigb(path):
    with open(path, 'rb') as f:
        b = f.read()
    magic, ver, foot_off, foot_size = parse_header(b)
    if magic != b'BIGB':
        raise ValueError("not BIGB")
    subs, _ = parse_footer(b, foot_off)
    ents = []
    for si, sub in enumerate(subs):
        entries, stats, ep, err = parse_bigb.parse_toc(b, sub, foot_off)
        if err:
            raise ValueError(f"toc[{si}] {err}")
        for e in entries:
            e['sub'] = sub['name']
            ents.append(e)
    return b, ents


# --------------------------------------------------------------------------
# Entry classification
# --------------------------------------------------------------------------
# MBANK Type histogram (retail): 1,2,4,5 = meshes (compiled-model payload),
# 3 = tagged BBM (3DMF chunk container, mostly [PHYSICS]/collision), 6,7,9 = ANIM.
MESH_TYPES = (1, 2, 4, 5)
BBM_TYPE = 3
ANIM_TYPES = (6, 7, 9)


def classify_payload(b, e):
    """Return the payload family from the leading bytes."""
    if e['size'] == 0:
        return 'empty'
    head = b[e['offset']:e['offset'] + 64]
    if b'3DMF' in head:
        return 'tagged-bbm'          # 3DMF chunk container
    if head[:5] == b'MESH_':
        return 'compiled-mesh'       # name-prefixed compiled model
    if head[:5] == b'ANIM_':
        return 'anim'
    return 'other:' + head[:4].hex()


# --------------------------------------------------------------------------
# Mesh descriptor (the TOC `Info` blob) for compiled meshes (types 1,2,4,5)
# --------------------------------------------------------------------------
# Layout (all little-endian, dword-granular):
#   u32   flags
#   f32   origin[10]        # bbox / pivot / bounding-sphere block (see doc)
#   u32   nLOD
#   u32   lod[nLOD]         # lod[0] = byte offset (in payload) to the LOD0 block;
#                           # lod[1..] = descending sizes of the LOD blocks
#   ...   (pad u32 + 1..2 LOD-distance f32; count varies by variant)
#   u32   nMat
#   u32   matTexId[nMat]    # each is a textures.big entry ID (BASE texture per material)
#
# The nMat split is recovered from the TAIL: nMat is the count value whose position
# leaves exactly nMat trailing dwords. This makes the parse self-checking (zero
# leftover) and independent of the variable mid-section (pad + LOD-distance floats).

def parse_mesh_descriptor(info, payload=None, ename=None):
    """Parse the compiled-mesh Info descriptor. Returns dict or None if it does not
    fit exactly (zero-leftover self-check + optional lod[0] name-repeat check)."""
    if len(info) < 48 or len(info) % 4 != 0:
        return None
    nd = len(info) // 4
    dw = struct.unpack_from('<%dI' % nd, info)
    fw = struct.unpack_from('<%df' % nd, info)

    flags = dw[0]
    origin = fw[1:11]
    nLOD = dw[11]
    if not (1 <= nLOD <= 8):
        return None
    base = 12
    if base + nLOD > nd:
        return None
    lod = dw[base:base + nLOD]

    # optional: verify lod[0] lands on the payload's repeated entry name (LOD0 block)
    if payload is not None and ename is not None:
        nm = ename.encode('latin1')[:6]
        off0 = lod[0]
        if off0 >= len(payload) or payload[off0:off0 + len(nm)] != nm:
            return None

    q = base + nLOD
    # recover nMat from the tail: value at nmpos with exactly nMat dwords after it
    for nmpos in range(q, nd):
        nMat = dw[nmpos]
        if nMat == nd - nmpos - 1 and 0 < nMat <= 64:
            mats = dw[nmpos + 1:]
            mid = dw[q:nmpos]           # pad u32 + LOD-distance floats
            mid_f = fw[q:nmpos]
            return dict(flags=flags, origin=origin, nLOD=nLOD, lod=lod,
                        nMat=nMat, matTexId=list(mats),
                        mid=list(mid), lod_dist=[x for x in mid_f if 0.0 < x < 10.0],
                        infosize=len(info))
    return None


# --------------------------------------------------------------------------
# Compiled-model payload header + per-LOD SUBM sub-headers
# --------------------------------------------------------------------------
def parse_compiled_header(pay):
    """Parse the leading header of a compiled-mesh payload.
    Returns dict {name, skel_flag, origin[10], header_end}."""
    z = pay.index(b'\x00')
    name = pay[:z].decode('latin1')
    p = z + 1
    skel_flag = pay[p]; p += 1
    origin = struct.unpack_from('<10f', pay, p); p += 40
    return dict(name=name, skel_flag=skel_flag, origin=origin, header_end=p)


def find_subm_headers(pay, lod_off):
    """Heuristically locate SUBM sub-headers within a LOD block.
    A SUBM header is 5 consecutive dwords (nVerts, nFaces, nFaceVertexIndices,
    sVert, flag) where nFaceVertexIndices == 3*nFaces and sVert in {4,6,20,22}.
    Returns list of dicts. (Detection only; the geometry after it is compressed.)"""
    out = []
    n = len(pay)
    end = min(n, lod_off + (pay.find(b'MESH_', lod_off + 5) - lod_off
                            if pay.find(b'MESH_', lod_off + 5) > 0 else n - lod_off) + lod_off)
    p = lod_off
    while p + 20 <= n:
        nVerts, nFaces, nFVI, sVert, flag = struct.unpack_from('<5I', pay, p)
        if (0 < nFaces < 100000 and nFVI == 3 * nFaces and 0 < nVerts <= 65535
                and sVert in (4, 6, 20, 22)):
            out.append(dict(off=p, nVerts=nVerts, nFaces=nFaces,
                            nFaceVertexIndices=nFVI, sVert=sVert, flag=flag))
            p += 20
        else:
            p += 1
        if len(out) > 64:
            break
    return out


# sVert -> (stride_bytes, description) per fabletlcmod.com wiki (file_formats:bbm)
SVERT_TABLE = {
    # code: list of (stride, description) — a code maps to variants by context
    4:  [(12, "packed coords, packed normals, s11e4 tu tv"),
         (20, "packed coords, bones, packed normals, s11e4 tu tv"),
         (36, "float coords, float normals, float tu tv, dword meshlevel")],
    6:  [(28, "packed coords, bones, packed normals, s11e4 tu, tv, dword[2]")],
    20: [(20, "float coords, packed normals, s11e4 tu tv"),
         (28, "float coords, bones, packed normals, s11e4 tu tv")],
    22: [(36, "float coords, bones, packed normals, s11e4 tu,tv, dword[2]")],
}


# --------------------------------------------------------------------------
def cmd_all(b, ents):
    byt = collections.defaultdict(list)
    for e in ents:
        byt[e['type']].append(e)
    print("=== MBANK type histogram ===")
    for t in sorted(byt):
        fams = collections.Counter(classify_payload(b, e) for e in byt[t])
        kind = ('MESH' if t in MESH_TYPES else 'ANIM' if t in ANIM_TYPES
                else 'BBM' if t == BBM_TYPE else '?')
        # collapse noisy per-entry 'other:<hash>' families into one bucket
        fam_show = collections.Counter()
        for k, v in fams.items():
            fam_show['other' if k.startswith('other:') else k] += v
        print(f"  type {t} [{kind}]: {len(byt[t]):5d}  families={dict(fam_show)}")

    print("\n=== mesh-descriptor validation (types 1,2,4,5) ===")
    ok = fail = 0
    fails = []
    mathit_ids = []
    for t in MESH_TYPES:
        for e in byt[t]:
            pay = b[e['offset']:e['offset'] + e['size']]
            d = parse_mesh_descriptor(e['info'], pay, e['name'])
            if d:
                ok += 1
                mathit_ids.extend(d['matTexId'])
            else:
                fail += 1
                if len(fails) < 12:
                    fails.append((e['name'], e['infosize']))
    print(f"  parsed zero-leftover + lod0 name-repeat: {ok}/{ok + fail}")
    if fails:
        print(f"  fails: {fails}")
    print(f"  total material tex-IDs referenced: {len(mathit_ids)} "
          f"(range {min(mathit_ids)}..{max(mathit_ids)})")

    print("\n=== tagged BBM (type 3) chunk presence ===")
    tags = collections.Counter()
    for e in byt[BBM_TYPE]:
        pay = b[e['offset']:e['offset'] + e['size']]
        for tg in (b'3DMF', b'MTLS', b'SUBM', b'PRIM', b'UNIV', b'VERT', b'BONE', b'HLPR'):
            if tg in pay:
                tags[tg.decode()] += 1
    print(f"  {dict(tags)} of {len(byt[BBM_TYPE])}")


def cmd_entry(b, ents, name):
    e = next((x for x in ents if x['name'] == name), None)
    if not e:
        print(f"no entry named {name!r}")
        return
    pay = b[e['offset']:e['offset'] + e['size']]
    fam = classify_payload(b, e)
    print(f"=== {name!r} type={e['type']} size={e['size']} family={fam} ===")
    d = parse_mesh_descriptor(e['info'], pay, e['name'])
    if d:
        print(f"  descriptor: flags=0x{d['flags']:x} nLOD={d['nLOD']} lod={[hex(x) for x in d['lod']]}")
        print(f"    origin(10f)={['%.3f' % x for x in d['origin']]}")
        print(f"    lod_dist={d['lod_dist']}  nMat={d['nMat']} matTexId={d['matTexId']}")
    if fam == 'compiled-mesh':
        h = parse_compiled_header(pay)
        print(f"  payload header: name={h['name']!r} skel_flag={h['skel_flag']} "
              f"origin={['%.3f' % x for x in h['origin']]}")
        if d:
            sm = find_subm_headers(pay, d['lod'][0])
            print(f"  LOD0 @0x{d['lod'][0]:x}: {len(sm)} SUBM sub-headers")
            for s in sm[:8]:
                strides = SVERT_TABLE.get(s['sVert'], [('?', '?')])
                print(f"    @0x{s['off']:x} nVerts={s['nVerts']} nFaces={s['nFaces']} "
                      f"nFVI={s['nFaceVertexIndices']} sVert={s['sVert']} "
                      f"strides={[x[0] for x in strides]}")


def cmd_xref(b, ents, texpath):
    tb, tents = load_bigb(texpath)
    tids = {e['id']: e for e in tents}
    byt = collections.defaultdict(list)
    for e in ents:
        byt[e['type']].append(e)
    total = hit = 0
    for t in MESH_TYPES:
        for e in byt[t]:
            pay = b[e['offset']:e['offset'] + e['size']]
            d = parse_mesh_descriptor(e['info'], pay, e['name'])
            if not d:
                continue
            for m in d['matTexId']:
                total += 1
                if m in tids:
                    hit += 1
    print(f"material tex-ID xref: {hit}/{total} "
          f"({100.0 * hit / total:.1f}%) resolve to textures.big entries")


def main():
    if len(sys.argv) < 2:
        print(__doc__); return
    path = sys.argv[1]
    b, ents = load_bigb(path)
    if '--entry' in sys.argv:
        cmd_entry(b, ents, sys.argv[sys.argv.index('--entry') + 1])
    elif '--xref' in sys.argv:
        cmd_xref(b, ents, sys.argv[sys.argv.index('--xref') + 1])
    else:
        cmd_all(b, ents)


if __name__ == '__main__':
    main()
