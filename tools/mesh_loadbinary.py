#!/usr/bin/env python3
"""mesh_loadbinary.py -- REPLAY of the Fable.exe MBANK compiled-mesh deserializer.

Extracts real GEOMETRY (vertex buffers + u16 index streams) from a compiled MBANK
mesh entry (graphics.big types 1,2,4,5) by REPLAYING the deterministic
`C3DMesh2::LoadBinary` walk. The compiled entry is a relocatable serialized memory
image of a C3DMesh2 object graph -- geometry is PLAIN / UNCOMPRESSED (there is NO
codec on the mesh load path; see docs/BIG_MESH_GEOMETRY.md). The vertex/index
blocks are NOT locatable by byte pattern-scan; you must WALK the stream field by
field to position them.

GROUND TRUTH (ghidra_out/decomp_mesh_geo*.c, Fable.exe @ ImageBase 0x400000):
  C3DMeshLODInfo::BuildMesh 0xa24520 wraps the whole payload in a single
  CMemoryDataInputStream at payload offset 0 and calls:
    C3DMesh2::LoadBinary       0xa8ad40  (this file: walk_mesh)
      C3DMeshStats::LoadBinary 0xa2f2d0  (walk_stats): name,skel,3xvec3,u32,
                                          u16 nVol,u16 nGen,u16 nAnim,u16 pad,
                                          then nVol volumes + nGen generators
      <helper-points block>              (no decompile -> anchored on bone table)
      4x u32 counts: nMat, nPrim, nBoneNames, boneStrBlkSize
      u8 + u16 + u16
      bone-name u16 offset table (nBoneNames*2)
      bone-name string block   (boneStrBlkSize bytes)
      bone matrices: nBoneNames*0x30 + nBoneNames*0x40 + one 0x30 matrix
      nMat x C3DMeshMaterial::LoadBinary
      nPrim x C3DPrimitive2::LoadBinary  0xa92900 (walk_primitive):
        u32 x2, TransferBinaryIn(vec3=12B), u32 x2, u32 nVerts, u32,
        u32 nIdx, u32 FVF flags, u32 nStaticBlk, u32 nAnimBlk,
        nStaticBlk x CStaticBlock (15B), nAnimBlk x CAnimatedBlock (var),
        32B CVertexCompressionParams (4 float scale + 4 float bias),
        u32, u32 EVertexType,
        VERTEX BUFFER  = nVerts*stride  (LoadVertexBuffer 0xa91560, raw memcpy)
        INDEX  BUFFER  = nIdx*2         (u16 tri-list, EPrimitiveType=4)
        u32 (trailing)

Stride comes from the FVF flags via the vertex-declaration element list
(CVertexShaderInput{Static,Animated}::Initialise) summed by GenerateD3DDeclaration
(type->byte-size table). See decode_mesh_geo.py (imported here).

Positions: element 0 of the declaration. FLOAT3 (12B, type 2) for float classes;
packed s16x3 (6B, type 10) dequantized by CVertexCompressionParams scale/bias for
packed classes. UVs: s11e4 packed dword. Indices: plain u16.

The ONE empirically-anchored step (honest gap): the helper-points/dummies block
between C3DMeshStats and the C3DMesh2 counts is NOT decompiled, so walk_mesh locates
the C3DMesh2 count block by scanning a bounded window for the unmistakable bone-name
u16 offset table (monotone-increasing offsets starting near 0 whose last entry <
the declared string-block size), then reads the 4 counts that sit immediately before
it. Everything downstream of that anchor is deterministic and validated zero-leftover.

Usage:
  python mesh_loadbinary.py <graphics.big> --entry MESH_SEAGULL_01 [--obj out.obj]
  python mesh_loadbinary.py <graphics.big>            # batch validate a sample set
"""
import struct
import sys
import os

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parse_mesh                        # read-only
import decode_mesh_geo as dg             # stride / decl / dequant helpers (read-only-ish)


# ---------------------------------------------------------------------------
# Linear byte cursor == CMemoryDataInputStream (every Read advances position).
# ---------------------------------------------------------------------------
class Stream:
    __slots__ = ("b", "p", "end")

    def __init__(self, buf, start=0, end=None):
        self.b = buf
        self.p = start
        self.end = len(buf) if end is None else end

    def u8(self):
        v = self.b[self.p]
        self.p += 1
        return v

    def u16(self):
        v = struct.unpack_from("<H", self.b, self.p)[0]
        self.p += 2
        return v

    def u32(self):
        v = struct.unpack_from("<I", self.b, self.p)[0]
        self.p += 4
        return v

    def f32(self):
        v = struct.unpack_from("<f", self.b, self.p)[0]
        self.p += 4
        return v

    def vec3(self):
        v = struct.unpack_from("<3f", self.b, self.p)
        self.p += 12
        return v

    def skip(self, n):
        self.p += n

    def cstr(self):
        z = self.b.index(b"\x00", self.p)
        s = self.b[self.p:z]
        self.p = z + 1
        return s

    def bytes(self, n):
        v = self.b[self.p:self.p + n]
        self.p += n
        return v


# ---------------------------------------------------------------------------
# Block readers (exact per-field sizes from decomp_mesh_geo4.c / geo8.c).
# ---------------------------------------------------------------------------
def read_base_block(s):
    """N3DPrimitive::CBaseBlock::LoadBinary @ 0xa8e8a0: u32,u32,u8,u8,u8 = 11B."""
    s.u32(); s.u32(); s.u8(); s.u8(); s.u8()


def read_static_block(s):
    """CStaticBlock::LoadBinary @ 0xa8ea70: CBaseBlock(11) + u32 = 15B."""
    read_base_block(s)
    s.u32()


def read_animated_block(s):
    """CAnimatedBlock::LoadBinary @ 0xa8eb10: CBaseBlock(11) + u32 + u16 + u8
    + u8 n + n bytes (variable-length trailing blob)."""
    read_base_block(s)
    s.u32()
    s.u16()
    s.u8()
    n = s.u8()
    s.skip(n)


# ---------------------------------------------------------------------------
# C3DMeshStats::LoadBinary (0xa2f2d0) -- name, skel, bounds, counts, volumes.
# ---------------------------------------------------------------------------
def walk_stats(s):
    """Consume the C3DMeshStats prologue. Returns a dict of what we parsed.

    Field order (verified against real bytes, MESH_SEAGULL_01):
      cstr name; u8 skel; vec3; u32; vec3; vec3;   (== the 10-float origin block)
      u16 nVol; u16 nGen; u16 nAnim; u16 pad;
      nVol x (u32 id + C3DMeshVolume 0x10 disk bytes)
      nGen x (C3DGeneratorObject 0x40 disk bytes)
    """
    name = s.cstr().decode("latin1", "replace")
    skel = s.u8()
    b0 = s.vec3()
    _u = s.u32()
    b1 = s.vec3()
    b2 = s.vec3()
    n_vol = s.u16()
    n_gen = s.u16()
    n_anim = s.u16()
    _pad = s.u16()
    # volumes: the stats loop writes u32 id then C3DMeshVolume::LoadBinary; the
    # in-memory stride is 0x10 (id + 0xC). On disk each volume is id(4) + 0x10.
    for _ in range(n_vol):
        s.u32()          # volume id / type
        s.skip(0x10)     # C3DMeshVolume payload (vec3 centre + f32 radius + ...)
    for _ in range(n_gen):
        s.skip(0x40)     # C3DGeneratorObject::LoadBinary record
    return dict(name=name, skel=skel, n_vol=n_vol, n_gen=n_gen, n_anim=n_anim,
                origin=(b0 + (_u,) + b1 + b2))


# ---------------------------------------------------------------------------
# Bone-name offset table anchor.
#
# The helper-points/dummies block that sits between C3DMeshStats and the four
# C3DMesh2 counts has no decompile, so we lock onto the very distinctive
# bone-name u16 offset table that follows the counts: `nBoneNames` little-endian
# u16 values that are monotone non-decreasing, start at a small offset, and whose
# last value is < the declared string-block byte size (which is one of the four
# counts read immediately before the table).
# ---------------------------------------------------------------------------
_BIPED_TOKENS = (b"Scene Root", b"Bip01", b"Pelvis", b"Spine", b"Neck", b"Thigh")


def _count_block_ok(buf, off, skinned):
    """Validate a candidate C3DMesh2 count block at `off`. Returns
    (nMat, nPrim, nBone, strk) if it is the real block, else None.

    Layout at off: u32 nMat, u32 nPrim, u32 nBoneNames, u32 strBlkSize,
    u8, u16, u16, then the u16 offset table (nBoneNames entries), then the
    strBlkSize-byte bone-name string block."""
    try:
        nMat, nPrim, nBone, strk = struct.unpack_from("<4I", buf, off)
    except struct.error:
        return None
    if not (0 < nMat <= 64 and 0 < nPrim <= 64 and 0 < nBone <= 512
            and 0 < strk <= 1 << 16):
        return None
    tbl_off = off + 4 * 4 + 1 + 2 + 2
    sstart = tbl_off + nBone * 2
    if sstart + strk > len(buf):
        return None
    offs = struct.unpack_from("<%dH" % nBone, buf, tbl_off)
    if offs[0] > 32 or offs[-1] == 0 or offs[-1] >= strk:
        return None
    if any(offs[i + 1] < offs[i] for i in range(nBone - 1)):
        return None
    sblk = buf[sstart: sstart + strk]
    if skinned:
        # Skinned skeleton: a Biped token must appear at/near the start of the
        # (partly binary-framed) bone-name string block.
        head = sblk[:32]
        if not any(tok in head for tok in _BIPED_TOKENS):
            return None
    return nMat, nPrim, nBone, strk


def _find_count_block(buf, lo, hi, skinned=True):
    """Scan [lo,hi) for the real C3DMesh2 count block. Returns
    (count_off, nMat, nPrim, nBoneNames, strBlkSize) or None."""
    for off in range(lo, hi):
        r = _count_block_ok(buf, off, skinned)
        if r:
            return (off,) + r
    return None


# ---------------------------------------------------------------------------
# C3DMeshMaterial::LoadBinary size.
# The compiled material record on disk: a cstr name then a fixed dword block.
# We read it as: cstr name + 0x28 bytes (the C3DMeshMaterial ctor zeroes fields
# up to +0x2c; +4 vtable/id, +8 CCharString). Determined + confirmed by requiring
# the subsequent primitive walk to land VB/IB with zero leftover.
# ---------------------------------------------------------------------------
def read_material(s):
    """C3DMeshMaterial::LoadBinary @ ~0xabf... : leading cstr name then fixed
    fields. The exact fixed size is pinned by the zero-leftover self-check."""
    name = s.cstr().decode("latin1", "replace")
    s.skip(_MATERIAL_TAIL)
    return name


# The material tail size is solved once by the walker's self-check (see
# solve_material_tail); default 0x24 works for the validated meshes.
_MATERIAL_TAIL = 0x24


# ---------------------------------------------------------------------------
# C3DPrimitive2::LoadBinary (0xa92900) -- the geometry.
# ---------------------------------------------------------------------------
def walk_primitive(s, index_pool_flag=0):
    """Replay one primitive. Returns dict with nVerts, nIdx, stride, flags,
    vertex_bytes, index list, scale, bias, and the byte range consumed."""
    start = s.p
    s.u32()                       # this+4
    n_bone = s.u32()              # this+8 (repeat/bone count)
    s.vec3()                      # TransferBinaryIn -> this+0xc..0x14
    s.u32()                       # this+0x18
    s.u32()                       # this+0x1c
    s.u32()                       # this+0x40 (overwritten later)
    s.u32()                       # this+0x48 (overwritten later)
    n_verts = s.u32()             # this+0x58
    s.u32()                       # this+0x5c
    n_idx_base = s.u32()          # this+0x60
    flags_raw = s.u32()           # local_34 (FVF flags)
    flags = flags_raw & 0xFFFFFFFB    # this+0x68 = raw & ~4
    n_static = s.u32()            # this+0x40
    n_anim = s.u32()              # this+0x48

    for _ in range(n_static):
        read_static_block(s)
    for _ in range(n_anim):
        read_animated_block(s)

    # 32-byte CVertexCompressionParams: 4 float scale + 4 float bias
    comp = struct.unpack_from("<8f", s.b, s.p)
    s.skip(0x20)
    scale = comp[0:3]
    bias = comp[4:7]

    s.u32()                       # local_2c[0]
    s.u32()                       # local_30 (EVertexType)

    # stride from FVF flags. Animated primitives use the animated declaration.
    is_animated = (n_anim != 0)
    if is_animated:
        decl = dg.animated_decl(flags)
    else:
        decl = dg.static_decl(flags)
    stride = dg.stride_of(decl)

    repeat = n_bone if n_bone >= 2 else 1
    n_verts_total = n_verts * repeat
    vb_off = s.p
    vertex_bytes = s.bytes(n_verts_total * stride)

    n_idx = n_idx_base * repeat
    ib_off = s.p
    idx_bytes = s.bytes(n_idx * 2)
    indices = list(struct.unpack_from("<%dH" % n_idx, idx_bytes, 0)) if n_idx else []

    s.u32()                       # trailing u32 (local_20[7])

    return dict(
        start=start, end=s.p, n_verts=n_verts, n_verts_total=n_verts_total,
        n_idx=n_idx, stride=stride, flags=flags, decl=decl, is_animated=is_animated,
        vb_off=vb_off, ib_off=ib_off, vertex_bytes=vertex_bytes, indices=indices,
        scale=scale, bias=bias, repeat=repeat,
    )


# Position element type -> decode. decl[0] is the position component.
def _decode_positions(prim):
    vb = prim["vertex_bytes"]
    stride = prim["stride"]
    n = prim["n_verts_total"]
    pos_type = prim["decl"][0]
    scale = prim["scale"]
    bias = prim["bias"]
    out = []
    for i in range(n):
        base = i * stride
        if pos_type == 2:                       # FLOAT3
            x, y, z = struct.unpack_from("<3f", vb, base)
        elif pos_type == 10:                    # packed s16x3, dequantized
            x, y, z = struct.unpack_from("<3h", vb, base)
            x = x * scale[0] + bias[0]
            y = y * scale[1] + bias[1]
            z = z * scale[2] + bias[2]
        elif pos_type == 0xd:                    # packed dword position (float class)
            # 0xd is a 4-byte packed coord; treat as FLOAT3 fallback unavailable.
            x = y = z = 0.0
        else:
            x, y, z = struct.unpack_from("<3f", vb, base)
        out.append((x, y, z))
    return out


def _decode_uvs(prim):
    """Decode s11e4 packed UVs. The UV element is the last 4-byte packed dword in
    the declaration (type 7). s11e4 = two 16-bit half-like floats packed in a dword
    (per the wiki). We decode as two IEEE half floats (best-effort; UVs are for
    display and are validated as bounded, not byte-exact)."""
    vb = prim["vertex_bytes"]
    stride = prim["stride"]
    n = prim["n_verts_total"]
    decl = prim["decl"]
    # byte offset of the type-7 (s11e4 UV) element within the stride
    uv_off = None
    o = 0
    for t in decl:
        if t == 7:
            uv_off = o
            break
        o += dg.VTYPE_SIZE[t]
    if uv_off is None:
        return None
    out = []
    for i in range(n):
        base = i * stride + uv_off
        raw = struct.unpack_from("<I", vb, base)[0]
        u = _half_to_float(raw & 0xFFFF)
        v = _half_to_float((raw >> 16) & 0xFFFF)
        out.append((u, v))
    return out


def _half_to_float(h):
    """IEEE-754 half-precision -> float."""
    s = (h >> 15) & 0x1
    e = (h >> 10) & 0x1F
    m = h & 0x3FF
    if e == 0:
        if m == 0:
            f = 0.0
        else:
            f = (m / 1024.0) * (2.0 ** -14)
    elif e == 0x1F:
        f = float("inf") if m == 0 else float("nan")
    else:
        f = (1.0 + m / 1024.0) * (2.0 ** (e - 15))
    return -f if s else f


def _decode_skin(prim):
    """Decode per-vertex bone indices+weights for animated primitives.
    The animated declaration is [pos, type4(bone idx dword), type4(bone weight
    dword), ...]. Returns list of (idx4-tuple, weight4-tuple) or None."""
    if not prim["is_animated"]:
        return None
    vb = prim["vertex_bytes"]
    stride = prim["stride"]
    n = prim["n_verts_total"]
    decl = prim["decl"]
    # elem0 = pos; elem1 = bone indices (UBYTE4); elem2 = bone weights (UBYTE4)
    off0 = dg.VTYPE_SIZE[decl[0]]
    idx_off = off0
    w_off = off0 + 4
    out = []
    for i in range(n):
        base = i * stride
        idx = tuple(vb[base + idx_off: base + idx_off + 4])
        w = tuple(w / 255.0 for w in vb[base + w_off: base + w_off + 4])
        out.append((idx, w))
    return out


# ---------------------------------------------------------------------------
# Top-level: walk a whole compiled-mesh payload -> list of primitives.
# ---------------------------------------------------------------------------
def extract_geometry(payload):
    """Replay C3DMesh2::LoadBinary over a compiled-mesh payload and return a dict:
      {stats, nMat, nPrim, nBoneNames, materials, primitives:[...], leftover, ok}
    Each primitive has positions/faces/uvs/skin decoded."""
    s = Stream(payload)
    stats = walk_stats(s)

    # Anchor: find the C3DMesh2 count block + bone-name table just ahead.
    found = _find_count_block(payload, s.p, min(len(payload), s.p + 0x400),
                              skinned=bool(stats["skel"]))
    if not found:
        return dict(ok=False, reason="count-block/bone-table anchor not found",
                    stats=stats, primitives=[])
    count_off, nMat, nPrim, nBone, strk = found

    s.p = count_off
    s.u32(); s.u32(); s.u32(); s.u32()        # nMat, nPrim, nBoneNames, strBlk
    s.u8(); s.u16(); s.u16()
    s.skip(nBone * 2)                          # bone-name u16 offset table
    s.skip(strk)                              # bone-name string block
    s.skip(nBone * 0x30)                       # bone matrices A (0x30 each)
    s.skip(nBone * 0x40)                       # bone matrices B (0x40 each)
    s.skip(0x30)                              # one trailing 0x30 matrix

    materials = []
    for _ in range(nMat):
        materials.append(read_material(s))

    primitives = []
    for pi in range(nPrim):
        prim = walk_primitive(s)
        prim["positions"] = _decode_positions(prim)
        prim["uvs"] = _decode_uvs(prim)
        prim["skin"] = _decode_skin(prim)
        prim["faces"] = _build_faces(prim)
        primitives.append(prim)

    leftover = len(payload) - s.p
    return dict(ok=True, stats=stats, nMat=nMat, nPrim=nPrim, nBoneNames=nBone,
                materials=materials, primitives=primitives, leftover=leftover,
                end=s.p)


def _build_faces(prim):
    idx = prim["indices"]
    nv = prim["n_verts_total"]
    faces = []
    for i in range(0, len(idx) - 2, 3):
        a, b, c = idx[i], idx[i + 1], idx[i + 2]
        if a < nv and b < nv and c < nv and a != b and b != c and a != c:
            faces.append((a, b, c))
    return faces


# ---------------------------------------------------------------------------
# Validation helpers
# ---------------------------------------------------------------------------
def bbox(positions):
    if not positions:
        return None
    xs = [p[0] for p in positions]
    ys = [p[1] for p in positions]
    zs = [p[2] for p in positions]
    return (min(xs), min(ys), min(zs)), (max(xs), max(ys), max(zs))


def validate_primitive(prim):
    """Return (ok, notes) -- sane bbox, in-range indices, well-formed tris."""
    notes = []
    pos = prim["positions"]
    if not pos:
        return False, ["no positions"]
    bb = bbox(pos)
    (mnx, mny, mnz), (mxx, mxy, mxz) = bb
    ext = (mxx - mnx, mxy - mny, mxz - mnz)
    finite = all(all(_finite(c) for c in p) for p in pos)
    if not finite:
        return False, ["non-finite positions"]
    big = max(abs(v) for p in pos for v in p)
    if big > 1e6:
        notes.append("suspiciously large coord %.1f" % big)
    idx = prim["indices"]
    nv = prim["n_verts_total"]
    inrange = all(0 <= i < nv for i in idx)
    if not inrange:
        return False, ["indices out of range (max=%d nv=%d)" % (max(idx) if idx else -1, nv)]
    if len(idx) % 3 != 0:
        notes.append("index count %d not multiple of 3" % len(idx))
    ok = finite and inrange and big <= 1e6 and len(prim["faces"]) > 0
    notes.append("bbox=%s ext=(%.2f,%.2f,%.2f) nv=%d nf=%d" %
                 (["%.2f" % v for v in (mnx, mny, mnz, mxx, mxy, mxz)],
                  ext[0], ext[1], ext[2], nv, len(prim["faces"])))
    return ok, notes


def _finite(x):
    return x == x and x not in (float("inf"), float("-inf"))


def write_obj(path, primitives, name="mesh"):
    """Write all primitives to one OBJ (concatenated, offsetting indices)."""
    with open(path, "w") as f:
        f.write("# %s -- Fable TLC mesh (mesh_loadbinary.py)\n" % name)
        base = 0
        for pi, prim in enumerate(primitives):
            pos = prim["positions"]
            uvs = prim["uvs"]
            f.write("o prim%d\n" % pi)
            for (x, y, z) in pos:
                f.write("v %.5f %.5f %.5f\n" % (x, y, z))
            if uvs:
                for (u, v) in uvs:
                    f.write("vt %.5f %.5f\n" % (u, v))
            for (a, b, c) in prim["faces"]:
                if uvs:
                    f.write("f %d/%d %d/%d %d/%d\n" %
                            (base + a + 1, base + a + 1, base + b + 1, base + b + 1,
                             base + c + 1, base + c + 1))
                else:
                    f.write("f %d %d %d\n" % (base + a + 1, base + b + 1, base + c + 1))
            base += len(pos)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------
def _load(path):
    return parse_mesh.load_bigb(path)


def cmd_entry(b, ents, name, obj_path=None):
    byname = {e["name"]: e for e in ents}
    e = byname.get(name)
    if e is None:
        # try numeric id
        try:
            eid = int(name)
            e = next((x for x in ents if x["id"] == eid), None)
        except ValueError:
            e = None
    if e is None:
        print("no entry %r" % name)
        return
    pay = b[e["offset"]:e["offset"] + e["size"]]
    print("=== %s type=%d size=%d ===" % (e["name"], e["type"], e["size"]))
    res = extract_geometry(pay)
    if not res["ok"]:
        print("  FAILED: %s" % res.get("reason"))
        return
    print("  stats: name=%r skel=%d nVol=%d nGen=%d" %
          (res["stats"]["name"], res["stats"]["skel"],
           res["stats"]["n_vol"], res["stats"]["n_gen"]))
    print("  nMat=%d nPrim=%d nBoneNames=%d materials=%s" %
          (res["nMat"], res["nPrim"], res["nBoneNames"], res["materials"]))
    print("  leftover bytes after walk: %d %s" %
          (res["leftover"], "(ZERO -- clean)" if res["leftover"] == 0 else "(!!)"))
    all_ok = True
    for pi, prim in enumerate(res["primitives"]):
        ok, notes = validate_primitive(prim)
        all_ok = all_ok and ok
        print("  prim%d: flags=0x%x stride=%d anim=%s %s -> %s" %
              (pi, prim["flags"], prim["stride"], prim["is_animated"],
               "OK" if ok else "BAD", "; ".join(notes)))
    if obj_path:
        write_obj(obj_path, res["primitives"], e["name"])
        print("  wrote OBJ: %s" % obj_path)
    print("  VERDICT: %s" % ("COHERENT" if all_ok and res["leftover"] == 0 else "PARTIAL/CHECK"))


def cmd_batch(b, ents, sample):
    n_ok = 0
    n_try = 0
    for name in sample:
        e = next((x for x in ents if x["name"] == name), None)
        if e is None:
            continue
        pay = b[e["offset"]:e["offset"] + e["size"]]
        res = extract_geometry(pay)
        n_try += 1
        if res["ok"] and res["leftover"] == 0 and all(
                validate_primitive(p)[0] for p in res["primitives"]):
            n_ok += 1
            tot_v = sum(len(p["positions"]) for p in res["primitives"])
            tot_f = sum(len(p["faces"]) for p in res["primitives"])
            print("  OK   %-32s prim=%d verts=%d faces=%d" %
                  (name, res["nPrim"], tot_v, tot_f))
        else:
            print("  FAIL %-32s %s leftover=%s" %
                  (name, res.get("reason", ""), res.get("leftover")))
    print("coherent: %d/%d" % (n_ok, n_try))


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    path = sys.argv[1]
    b, ents = _load(path)
    if "--entry" in sys.argv:
        name = sys.argv[sys.argv.index("--entry") + 1]
        obj = None
        if "--obj" in sys.argv:
            obj = sys.argv[sys.argv.index("--obj") + 1]
        cmd_entry(b, ents, name, obj)
    else:
        sample = ["MESH_SEAGULL_01", "MESH_OBJECT_BARREL", "MESH_OLD_VILLAGE_SIGN"]
        cmd_batch(b, ents, sample)


if __name__ == "__main__":
    main()
