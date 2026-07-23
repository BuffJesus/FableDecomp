#!/usr/bin/env python3
r"""Native compiled-mesh WRITE path for graphics.big (Route B -- no EgoCore exe).

Segment-capture round-trip:
  parse_segments()  walks a compiled-mesh entry EXACTLY like
                    fable_core._parse_mesh_primitives, recording an ordered list of
                    segments -- ('plain', bytes) for the un-framed fields (copied
                    verbatim) and ('block', decoded_bytes, tag) for each Fable
                    chunked-LZO sub-block (captured DECODED).
  serialize_segments()  re-emits plain spans verbatim and each block as a STORED
                    chunk (b'\x00\x00' + block). DecompressLZO's clen==0 path reads
                    that back byte-for-byte, so the result is a valid, engine-loadable,
                    UNCOMPRESSED entry. No LZO compressor needed (real game data uses
                    stored chunks for small blocks; here we use them for all).

Editing a mesh == replace_block(segs, tag, new_bytes) then serialize -- everything
else stays byte-identical. The round-trip oracle (roundtrip_report) proves
decode(serialize(parse(x))) is identical to decode(x).
"""
import os
import sys
import struct

_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)
import fable_core as fc


def parse_segments(pay, mesh_type):
    """Ordered segments that fully reconstruct `pay`: LOD0's chunked-LZO blocks
    captured DECODED (tagged), all un-framed bytes captured verbatim."""
    fc._load_readers()
    segs = []
    st = {'last': 0}

    def blk(cur, expected, tag):
        segs.append(('plain', bytes(pay[st['last']:cur])))
        out, ncur = fc._lzo_fable(pay, cur, expected)
        segs.append(('block', out, tag))
        st['last'] = ncur
        return out, ncur

    c = fc._Cur(pay)
    c.s(); c.u8(); c.raw(12); c.f(); c.raw(12); c.raw(12)
    hpc = c.u16(); doc = c.u16(); pns = c.u16(); mvc = c.u16(); mgc = c.u16()
    if hpc > 0:
        _, c.i = blk(c.i, 20 * hpc, 'helpers')
    if doc > 0:
        _, c.i = blk(c.i, 56 * doc, 'dummies')
    if pns > 0:
        _, c.i = blk(c.i, pns, 'packednames')
    for _ in range(mvc):
        c.u32(); c.s()
        pcn = c.u32()
        if pcn > 0:
            _, c.i = blk(c.i, 16 * pcn, 'planes')
    for _ in range(mgc):
        c.raw(48); c.i32(); c.s(); c.u32(); c.u8()
    mat_count = c.i32(); prim_count = c.i32(); bone_count = c.i32()
    bone_name_size = c.i32(); c.u8(); c.u16(); c.u16()
    if bone_count > 0:
        c.raw(2 * bone_count)
        _, c.i = blk(c.i, bone_name_size, 'bonenames')
        _, c.i = blk(c.i, 60 * bone_count, 'bones')
        _, c.i = blk(c.i, 48 * bone_count, 'bonekeyframes')
        _, c.i = blk(c.i, 64 * bone_count, 'bonetransforms')
    c.raw(48)
    for _ in range(mat_count):
        c.i32(); c.s()
        for _ in range(7):
            c.i32()
        c.u8(); c.u8(); c.u8(); c.u8()
        if c.u8():
            for _ in range(4):
                c.s()
    for pi in range(prim_count):
        c.i32(); reps = c.i32()
        c.raw(12); c.f(); c.f()
        sbc = c.u32(); abc = c.u32()
        vc = c.u32(); c.u32(); ic = c.u32(); init = c.u32()
        c.u32(); c.u32()
        for _ in range(sbc):
            c.u32(); c.u32(); c.u8(); c.u8(); c.u8(); c.i32()
        for _ in range(abc):
            c.u32(); c.u32(); c.u8(); c.u8(); c.u8(); c.u32(); c.u16(); c.u8()
            gc = c.u8()
            if gc > 0:
                c.raw(gc)
        c.raw(32)
        stride = c.u32(); c.u32()
        stride = stride if stride > 0 else fc._calc_stride(init, abc > 0)
        r = reps if reps > 1 else 1
        vbsz = stride * vc * r
        if vbsz > 0:
            _, c.i = blk(c.i, vbsz, ('vbuf', pi))
        elif c.i + 2 <= len(pay) and struct.unpack_from('<H', pay, c.i)[0] == 0:
            c.i += 2
        ibsz = 2 * r * ic
        if ibsz > 0:
            _, c.i = blk(c.i, ibsz, ('ibuf', pi))
        elif c.i + 2 <= len(pay) and struct.unpack_from('<H', pay, c.i)[0] == 0:
            c.i += 2
        for _ in range(c.u32()):
            c.u32(); c.u32()
            pl = c.u32()
            if pl > 0:
                _, c.i = blk(c.i, pl, ('cloth', pi))
    segs.append(('plain', bytes(pay[st['last']:])))   # plain tail + deeper LODs verbatim
    return segs


def serialize_segments(segs, compress=False):
    """Re-emit segments.

    compress=False (default): blocks become STORED chunks b'\\x00\\x00' + block,
      which _lzo_fable reads back exactly (clen==0 -> copy size-3 + 3 trailing).
      No compressor needed; larger output.
    compress=True: blocks become Fable chunked-LZO (lionhead_lz_compress), so the
      output is as compact as the original game data. Both are engine-loadable.
    """
    _comp = None
    if compress:
        import lionhead_lz_compress as _comp   # tools/ is on path via fable_core
    out = bytearray()
    for seg in segs:
        if seg[0] == 'plain':
            out += seg[1]
        elif compress and len(seg[1]) > 3:
            out += _comp.compress_fable_block(seg[1])
        else:
            out += b'\x00\x00' + seg[1]
    return bytes(out)


def replace_block(segs, tag, new_bytes):
    """Return segs with the single block tagged `tag` swapped for new_bytes."""
    out, hit = [], False
    for seg in segs:
        if seg[0] == 'block' and seg[2] == tag:
            out.append(('block', bytes(new_bytes), tag)); hit = True
        else:
            out.append(seg)
    if not hit:
        raise KeyError('no block tagged %r' % (tag,))
    return out


def roundtrip_report(pay, mesh_type):
    """Prove decode(serialize(parse(x))) == decode(x): re-parse both the original
    and the re-serialized payload and compare every primitive's vertex/index bytes."""
    reser = serialize_segments(parse_segments(pay, mesh_type))
    _n1, p1, _b1 = fc._parse_mesh_primitives(pay, mesh_type)
    _n2, p2, _b2 = fc._parse_mesh_primitives(reser, mesh_type)
    ok = (len(p1) == len(p2))
    for a, b in zip(p1, p2):
        ok = ok and a['vb'] == b['vb'] and a['ib'] == b['ib'] \
            and a['vc'] == b['vc'] and a['init'] == b['init'] and a['stride'] == b['stride']
    return {'ok': ok, 'prims': len(p1), 'orig_len': len(pay), 'reser_len': len(reser)}


# ---------------------------------------------------------------------------
# Vertex-field ENCODER (edit geometry): overwrite each vertex's POSITION (and
# optionally UV) in the vertex buffer, preserving normals / bone indices /
# weights. Inverse of fable_core._prim_geometry; must mirror its is_pos_comp /
# offset decisions so decode(encode(x)) round-trips.
# ---------------------------------------------------------------------------
def pack_pos_packed3(pos, scale, off):
    """Inverse of fable_core._unpack_pos_packed3: float xyz -> packed u32
    (signed 11/11/10) via the primitive's CVertexCompressionParams."""
    sx = 0.0009775171 * (scale[0] if scale[0] else 1.0)
    sy = 0.0009775171 * (scale[1] if scale[1] else 1.0)
    sz = 0.0019569471 * (scale[2] if scale[2] else 1.0)
    ix = int(round((pos[0] - off[0]) / sx))
    iy = int(round((pos[1] - off[1]) / sy))
    iz = int(round((pos[2] - off[2]) / sz))
    ix = max(-1024, min(1023, ix))
    iy = max(-1024, min(1023, iy))
    iz = max(-512, min(511, iz))
    return (ix & 0x7FF) | ((iy & 0x7FF) << 11) | ((iz & 0x3FF) << 22)


def _pos_is_packed(stride, init_flags, mesh_type, has_bones):
    ipc = (init_flags & 4) != 0 and (init_flags & 0x10) == 0
    if mesh_type == 4 or (stride == 36 and not has_bones):
        ipc = False
    return ipc


def encode_positions(vb, stride, init_flags, mesh_type, positions, scale, off, has_bones):
    """Return a copy of vertex buffer `vb` with each vertex POSITION overwritten
    from `positions` (one xyz per vertex); all other per-vertex fields preserved.
    Packed positions are quantised via the primitive's compression params (lossless
    for unedited verts). Encoding is the exact inverse of the decoder, so
    fable_core._prim_geometry re-decodes an unedited buffer byte-identically."""
    packed = _pos_is_packed(stride, init_flags, mesh_type, has_bones)
    out = bytearray(vb)
    psz = 4 if packed else 12
    for v, pos in enumerate(positions):
        o = v * stride
        if o + psz > len(out):
            break
        if packed:
            struct.pack_into('<I', out, o, pack_pos_packed3(pos, scale, off))
        else:
            struct.pack_into('<3f', out, o, float(pos[0]), float(pos[1]), float(pos[2]))
    return bytes(out)


def encode_skin(vb, stride, init_flags, mesh_type, prim, vert_influences):
    """Overwrite each ANIMATED vertex's joint-index + weight bytes in the vertex
    buffer from `vert_influences` (one list[(local_bone_slot, weight)] per vertex,
    parallel to the vertex order). Returns (new_vb_bytes, new_ablocks).

    This is the inverse of fable_core._prim_skin (docs/BIG_MESH_GEOMETRY.md §7.1):
      * top-4 influences per vertex, renormalized to u8 (sum forced to 255);
      * joint byte = paletteIndex*3, where paletteIndex is the local bone's slot in
        this animated block's Groups[] palette (appended if absent, capped at 84
        palette entries since ind byte / 3 must stay < 256 -> palette < 85);
      * weight byte = round(weight*255), residual pushed onto the largest so the
        four bytes sum to exactly 255 (matches the engine's u8 renormalization).
    `prim` is the parsed-primitive dict (needs 'abc', 'ablocks', 'vc', 'reps').
    Returns the updated Groups[] palettes alongside so the caller can re-emit the
    animated-block records. Static prims (abc<=0) are returned unchanged."""
    if prim['abc'] <= 0:
        return bytes(vb), prim['ablocks']
    is_pos_comp = (init_flags & 4) != 0 and (init_flags & 0x10) == 0
    if mesh_type == 4 or (stride == 36 and prim['abc'] <= 0):
        is_pos_comp = False
    i_off = 4 if is_pos_comp else 12
    w_off = i_off + 4
    reps = prim['reps'] if prim['reps'] > 1 else 1
    n = prim['vc'] * reps
    out = bytearray(vb)
    # mutable copy of each block's palette (list of local bone slots)
    ablocks = [list(ab) for ab in prim['ablocks']]
    for ab in ablocks:
        ab[4] = list(ab[4])
    blk = 0
    proc = 0
    limit = ablocks[0][3] if ablocks else 0x7FFFFFFF
    for v in range(n):
        if proc >= limit:
            blk += 1
            proc = 0
            limit = ablocks[blk][3] if blk < len(ablocks) else 0x7FFFFFFF
        proc += 1
        o = v * stride
        if o + w_off + 4 > len(out):
            break
        infl = vert_influences[v] if v < len(vert_influences) else []
        infl = sorted([(s, w) for (s, w) in infl if w > 0.0],
                      key=lambda kv: -kv[1])[:4]
        if not infl:
            infl = [(0, 1.0)]
        tot = sum(w for _, w in infl)
        if tot <= 0:
            infl = [(0, 1.0)]; tot = 1.0
        palette = ablocks[blk][4] if blk < len(ablocks) else []
        jb = [0, 0, 0, 0]
        wb = [0, 0, 0, 0]
        for k, (slot, w) in enumerate(infl):
            if slot not in palette:
                if len(palette) < 85:
                    palette.append(slot)
                    pid = len(palette) - 1
                else:
                    pid = 0
            else:
                pid = palette.index(slot)
            jb[k] = min(255, pid * 3)
            wb[k] = int(round(w / tot * 255.0))
        # force weight bytes to sum to exactly 255 (u8 renormalization)
        diff = 255 - sum(wb)
        if diff != 0:
            mk = max(range(4), key=lambda k: wb[k])
            wb[mk] = max(0, min(255, wb[mk] + diff))
        out[o + i_off:o + i_off + 4] = bytes(jb)
        out[o + w_off:o + w_off + 4] = bytes(wb)
    return bytes(out), [tuple(ab) for ab in ablocks]


def encode_prim_vbuf(prim, positions):
    """Convenience: encode `positions` into a parsed primitive dict (from
    fable_core._parse_mesh_primitives) and return the new vertex-buffer bytes."""
    return encode_positions(prim['vb'], prim['stride'], prim['init'], None,
                            positions, prim['scale'], prim['off'], prim['abc'] > 0)


# ---------------------------------------------------------------------------
# NEW-MESH COMPOSITION: build a complete compiled-mesh payload from arbitrary
# geometry (port of EgoCore MeshCompiler.h CompileSingleLOD/CompileForExport +
# GltfMeshImporter.h ImportType1, static subset). v1 scope: STATIC meshes only
# (BoneCount=0), LOD0 + retail-style ghost LOD, triangle-list static blocks.
#
# Retail conventions mirrored (verified on MESH_OBJECT_PLATE / _OLD_VILLAGE_SIGN
# / _RAILING_MID, all type 1, LODCount=1):
#   * payload = LOD0 block + a GHOST LOD (same header, MaterialCount=
#     PrimitiveCount=0, + 48-byte identity RootMatrix); Info LODSizes[0] =
#     len(LOD0) only (the ghost is NOT counted).
#   * a trailing sentinel material named 'DegenerateTriangles' (all map IDs 0,
#     MapFlags 0, DegenerateTriangles=1) is appended after the real materials.
#   * static type-1 vertex layouts attested in retail: InitFlags 0x04/stride 12
#     (packed pos+normal+UV) and InitFlags 0x14/stride 20 (FLOAT3 pos + packed
#     normal + s11e4 int16 UV). We emit 0x14 by default ('float' layout,
#     positions exact) or 0x04 ('packed', quantised via CVertexCompressionParams).
# ---------------------------------------------------------------------------
def pack_normal(n):
    """EgoCore GltfMeshImporter.h PackNormal: unit normal -> 11/11/10 dword."""
    x, y, z = float(n[0]), float(n[1]), float(n[2])
    ln = (x * x + y * y + z * z) ** 0.5
    if ln > 1e-5:
        x, y, z = x / ln, y / ln, z / ln
    ix = max(-1023, min(1023, int(round(x * 1023.0))))
    iy = max(-1023, min(1023, int(round(y * 1023.0))))
    iz = max(-511, min(511, int(round(z * 511.0))))
    return (ix & 0x7FF) | ((iy & 0x7FF) << 11) | ((iz & 0x3FF) << 22)


def compress_uv(v):
    """EgoCore CompressUV: float UV -> int16, inverse of fable_core._decompress_uv."""
    return max(-32768, min(32767, int(round((float(v) + 8.0) * 2048.0))))


def _face_normals(verts, faces):
    """Per-vertex normals accumulated from face normals (fallback when the
    caller supplies none)."""
    acc = [[0.0, 0.0, 0.0] for _ in verts]
    for (a, b, c) in faces:
        ax, ay, az = verts[a]
        e1 = (verts[b][0] - ax, verts[b][1] - ay, verts[b][2] - az)
        e2 = (verts[c][0] - ax, verts[c][1] - ay, verts[c][2] - az)
        nx = e1[1] * e2[2] - e1[2] * e2[1]
        ny = e1[2] * e2[0] - e1[0] * e2[2]
        nz = e1[0] * e2[1] - e1[1] * e2[0]
        for i in (a, b, c):
            acc[i][0] += nx
            acc[i][1] += ny
            acc[i][2] += nz
    out = []
    for v in acc:
        ln = (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) ** 0.5
        out.append((v[0] / ln, v[1] / ln, v[2] / ln) if ln > 1e-12 else (0.0, 0.0, 1.0))
    return out


def _bbox(vert_lists):
    mn = [1e9, 1e9, 1e9]
    mx = [-1e9, -1e9, -1e9]
    for verts in vert_lists:
        for v in verts:
            for j in range(3):
                if v[j] < mn[j]:
                    mn[j] = v[j]
                if v[j] > mx[j]:
                    mx[j] = v[j]
    return mn, mx


def _bsphere(vert_lists, mn, mx):
    c = [(mn[j] + mx[j]) * 0.5 for j in range(3)]
    r2 = 0.0
    for verts in vert_lists:
        for v in verts:
            d = (v[0] - c[0]) ** 2 + (v[1] - c[1]) ** 2 + (v[2] - c[2]) ** 2
            if d > r2:
                r2 = d
    return c, r2 ** 0.5


def _lzo_block(out, block, compress):
    """Append one Fable chunked-LZO framed block (EgoCore WriteLZOBlock):
    compress=True -> real chunked LZO; else a stored chunk (u16 0 + raw).
    Empty block -> a lone u16 0 (matches the reader's clen==0 skip)."""
    if not block:
        out += b'\x00\x00'
        return
    if compress and len(block) > 3:
        import lionhead_lz_compress as _comp
        out += _comp.compress_fable_block(bytes(block))
    else:
        out += b'\x00\x00' + bytes(block)


def _write_mat(out, mat_id, name, decal, diff, bump, refl, illum, mapflags,
               selfillum, two_sided, transparent, boolean_alpha, degen):
    out += struct.pack('<i', mat_id)
    out += name.encode('latin1') + b'\x00'
    out += struct.pack('<7i', decal, diff, bump, refl, illum, mapflags, selfillum)
    out += struct.pack('<5B', 1 if two_sided else 0, 1 if transparent else 0,
                       1 if boolean_alpha else 0, 1 if degen else 0, 0)  # UseFilenames=0


def _mesh_header(out, name, bsc, bsr, bbmin, bbmax, hpc=0, doc=0, pns=0, mvc=0,
                 mgc=0, animated=0):
    out += name.encode('latin1') + b'\x00'
    out += struct.pack('<B', 1 if animated else 0)     # AnimatedFlag
    out += struct.pack('<3f', *bsc)
    out += struct.pack('<f', bsr)
    out += struct.pack('<3f', *bbmin)
    out += struct.pack('<3f', *bbmax)
    out += struct.pack('<5H', hpc, doc, pns, mvc, mgc)


_IDENTITY_ROOT = struct.pack('<12f', 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0)


# ---------------------------------------------------------------------------
# SKINNED composition support (docs/BIG_MESH_GEOMETRY.md §7/§7.1, EgoCore
# MeshCompiler.h CompileSingleLOD + GltfMeshImporter.h ImportType5).
# The skeleton is CLONED from a donor mesh (retail boot / creature) as the four
# raw per-bone blocks -- byte-exact, no re-derivation of bind matrices needed.
# ---------------------------------------------------------------------------
def clone_skeleton(pay, mesh_type):
    """Extract the skeleton of an existing compiled mesh (LOD0) as raw blocks
    ready for compose_mesh(skeleton=...). Returns a dict:
      count         -- BoneCount
      indices_raw   -- u16[count] local slot -> GLOBAL Fable bone id (2*count B)
      names_raw     -- count NUL-terminated names (BoneNameSize B)
      bones_raw     -- 60 B/bone C3DBone records (hierarchy + LocalizationMatrix)
      keyframes_raw -- 48 B/bone TRS records
      ibms_raw      -- 64 B/bone inverse bind matrices
      names         -- decoded name list, local-slot order (for name->slot skin)
    Raises ValueError if the donor has no skeleton."""
    fc._load_readers()
    c = fc._Cur(pay)
    c.s(); c.u8(); c.raw(12); c.f(); c.raw(12); c.raw(12)
    hpc = c.u16(); doc = c.u16(); pns = c.u16(); mvc = c.u16(); mgc = c.u16()
    if hpc:
        _, c.i = fc._lzo_fable(pay, c.i, 20 * hpc)
    if doc:
        _, c.i = fc._lzo_fable(pay, c.i, 56 * doc)
    if pns:
        _, c.i = fc._lzo_fable(pay, c.i, pns)
    for _ in range(mvc):
        c.u32(); c.s()
        pc = c.u32()
        if pc:
            _, c.i = fc._lzo_fable(pay, c.i, 16 * pc)
    for _ in range(mgc):
        c.raw(48); c.i32(); c.s(); c.u32(); c.u8()
    c.i32(); c.i32(); bone_count = c.i32()
    bone_name_size = c.i32(); c.u8(); c.u16(); c.u16()
    if bone_count <= 0:
        raise ValueError('donor mesh has no skeleton (BoneCount=0)')
    idx_raw = bytes(c.raw(2 * bone_count))
    names_raw, c.i = fc._lzo_fable(pay, c.i, bone_name_size)
    bones_raw, c.i = fc._lzo_fable(pay, c.i, 60 * bone_count)
    kf_raw, c.i = fc._lzo_fable(pay, c.i, 48 * bone_count)
    ibm_raw, c.i = fc._lzo_fable(pay, c.i, 64 * bone_count)
    names = []
    p = 0
    for _ in range(bone_count):
        e = names_raw.find(b'\x00', p)
        if e < 0:
            e = len(names_raw)
        names.append(names_raw[p:e].decode('latin1'))
        p = e + 1
    return dict(count=bone_count, indices_raw=idx_raw, names_raw=bytes(names_raw),
                bones_raw=bytes(bones_raw), keyframes_raw=bytes(kf_raw),
                ibms_raw=bytes(ibm_raw), names=names)


def _write_skeleton(out, skel, compress):
    """Emit the BoneIndices span + the four chunked-LZO bone blocks
    (EgoCore CompileSingleLOD, the BoneCount>0 branch)."""
    n = skel['count']
    assert len(skel['indices_raw']) == 2 * n
    assert len(skel['bones_raw']) == 60 * n
    assert len(skel['keyframes_raw']) == 48 * n
    assert len(skel['ibms_raw']) == 64 * n
    out += skel['indices_raw']
    _lzo_block(out, skel['names_raw'], compress)
    _lzo_block(out, skel['bones_raw'], compress)
    _lzo_block(out, skel['keyframes_raw'], compress)
    _lzo_block(out, skel['ibms_raw'], compress)


def _resolve_skin(skin, skel, n_verts):
    """Normalize per-vertex influences to [(local_slot:int, weight), ...]:
    bones may be given as local slots (int) or bone NAMES (str, resolved via the
    skeleton's name list). Missing/short lists are padded with full-weight
    root-slot influences."""
    name2slot = {nm: i for i, nm in enumerate(skel['names'])}
    out = []
    for v in range(n_verts):
        infl = skin[v] if v < len(skin) else []
        res = []
        for bone, w in infl:
            if isinstance(bone, str):
                if bone not in name2slot:
                    raise ValueError('unknown bone name %r (skeleton has %s)'
                                     % (bone, skel['names']))
                slot = name2slot[bone]
            else:
                slot = int(bone)
            if not 0 <= slot < skel['count']:
                raise ValueError('bone slot %d out of range (BoneCount=%d)'
                                 % (slot, skel['count']))
            if w > 0.0:
                res.append((slot, float(w)))
        # top-3 influences, weight-desc (retail/EgoCore: BonesPerVertex=3)
        res = sorted(res, key=lambda kv: -kv[1])[:3]
        out.append(res if res else [(0, 1.0)])
    return out


def _auto_skin_blocks(verts, faces, uvs, norms, skin, max_palette=16):
    """Partition faces into animated blocks whose bone palettes fit
    `max_palette` slots (EgoCore ImportType5's greedy face walk; retail palettes
    go up to 18, EgoCore emits <=16 -- we default to the proven 16).

    Single-block fast path: if every used slot fits one palette, the data is
    returned UNCHANGED with one block covering all vertices (matches retail
    single-block prims where ablock.VertexCount == prim.VertexCount).
    Multi-block: vertices are regrouped per block in face-first-use order and
    DUPLICATED across blocks when shared (EgoCore behavior); returns the
    rebuilt (verts, uvs, norms, skin, blocks). Each block is a dict:
      palette    -- list of local bone slots (u8 Groups[])
      vert_count -- vertices owned by this block (file-order partition)
      faces      -- [(a,b,c), ...] with GLOBAL vertex indices
    """
    palette = []
    for infl in skin:
        for slot, _w in infl:
            if slot not in palette:
                palette.append(slot)
    if len(palette) <= max_palette:
        return verts, uvs, norms, skin, [dict(palette=palette,
                                              vert_count=len(verts), faces=faces)]
    # greedy face partition (EgoCore): grow the palette until it would overflow
    groups = []          # [(face_list, palette)]
    cur_faces, cur_pal = [], []
    for f in faces:
        fj = []
        for vi in f:
            for slot, _w in skin[vi]:
                if slot not in fj:
                    fj.append(slot)
        if not fj:
            fj = [0]
        test = list(cur_pal)
        for j in fj:
            if j not in test:
                test.append(j)
        if len(test) > max_palette and cur_faces:
            groups.append((cur_faces, cur_pal))
            cur_faces, cur_pal = [f], list(fj)
        else:
            cur_faces, cur_pal = cur_faces + [f], test
    if cur_faces:
        groups.append((cur_faces, cur_pal))
    nv, nu, nn, ns = [], [], [], []
    blocks = []
    for bfaces, pal in groups:
        remap = {}
        out_faces = []
        base = len(nv)
        for f in bfaces:
            g = []
            for vi in f:
                if vi not in remap:
                    remap[vi] = base + len(remap)
                    nv.append(verts[vi])
                    nu.append(uvs[vi])
                    nn.append(norms[vi] if norms else None)
                    ns.append(skin[vi])
                g.append(remap[vi])
            out_faces.append(tuple(g))
        blocks.append(dict(palette=pal, vert_count=len(remap), faces=out_faces))
    if norms is None or any(x is None for x in nn):
        nn = None
    return nv, nu, nn, ns, blocks


def _skin_bytes(infl, palette):
    """Encode one vertex's influences against its block palette: 4 joint bytes
    (paletteIndex*3) + 4 weight bytes (u8, forced to sum EXACTLY 255 -- every
    retail skinned vertex checked sums to 255). Influences must already be
    resolved local slots present in `palette` (<=3, weight-desc)."""
    jb = [0, 0, 0, 0]
    wb = [0, 0, 0, 0]
    tot = sum(w for _s, w in infl)
    if tot <= 0.0:
        return bytes(jb), bytes([255, 0, 0, 0])
    for k, (slot, w) in enumerate(infl[:3]):
        pid = palette.index(slot)
        jb[k] = pid * 3
        wb[k] = max(0, min(255, int(round(w / tot * 255.0))))
    diff = 255 - sum(wb)
    if diff != 0:
        mk = max(range(4), key=lambda k: wb[k])
        wb[mk] = max(0, min(255, wb[mk] + diff))
    return bytes(jb), bytes(wb)


def compose_mesh(name, prims, materials, compress=True, layout='float',
                 physics_index=0, skeleton=None, max_palette=16):
    """Compose a complete NEW compiled-mesh payload (+ its TOC Info blob) from
    arbitrary geometry. STATIC (v1) and SKINNED (v2, skeleton=...) meshes; no
    helpers/dummies/volumes/generators/cloth, LOD0 + retail ghost LOD,
    triangle lists.

    name      -- entry/mesh name; MUST start with 'MESH_' (open_graphics_big
                 classifies compiled meshes by that payload prefix).
    prims     -- list of dicts:
                   verts   : [(x,y,z), ...]            (required)
                   faces   : [(a,b,c), ...]            (required, CCW like the
                             faces fable_core._prim_geometry decodes)
                   uvs     : [(u,v), ...] PER-VERTEX   (optional, default 0,0);
                             Blender-convention V (the decoder yields 1-v, so we
                             store 1-v back -- compose(decode(x)) round-trips)
                   normals : [(x,y,z), ...] per-vertex (optional, else computed)
                   mat     : material slot (optional, default 0)
                   layout  : 'float'|'packed' override (optional)
                   comp_scale/comp_off : 4-float lists (optional, packed layout
                             only; else derived from the bbox like EgoCore)
                   skin    : per-vertex influence lists (SKINNED prims only;
                             requires skeleton=). skin[v] = [(bone, weight),...]
                             where bone is a LOCAL slot int or a bone NAME str.
                   skin_blocks : explicit animated-block partition (optional;
                             list of dicts {palette, vert_count, faces} -- used
                             by decode_to_compose_args to preserve the source
                             block structure; else built by _auto_skin_blocks).
    materials -- list of dicts: name (opt), diffuse_id (textures.big entry id),
                 bump_id/reflect_id/illum_id/decal_id (opt, 0), self_illum (opt 0),
                 two_sided/transparent/boolean_alpha (opt False).
    layout    -- static:  'float' InitFlags 0x14/stride 20, 'packed' 0x04/12.
                 skinned: 'float' InitFlags 0x14/stride 28 (retail boots),
                          'packed' 0x04/20 (retail creatures) -- +4 joint bytes
                          +4 weight bytes after the position (docs §7.1).
    skeleton  -- clone_skeleton() dict (raw donor bone blocks). When given the
                 payload is a type-5 grammar: AnimatedFlag=1, BoneCount>0 + the
                 four bone blocks, skinned prims carry CAnimatedBlock records
                 (BonesPerVertex=3, PalettedFlag=1) instead of static blocks,
                 and NO 'DegenerateTriangles' sentinel material is appended
                 (checked: retail skinned meshes don't carry one).
    Returns (payload_bytes, info_bytes). info_bytes is the entry subheader
    (EgoCore C3DMeshContent::SerializeEntryMetadata: PhysicsIndex, bounds,
    LODCount=1, LODSizes[0]=len(LOD0), SafeBoundingRadius, TextureIDs)."""
    fc._load_readers()
    if not name.startswith('MESH_'):
        raise ValueError("mesh name must start with 'MESH_' (payload classifier)")
    if not prims:
        raise ValueError('at least one primitive required')
    if not materials:
        raise ValueError('at least one material required')
    if any(p.get('skin') for p in prims) and skeleton is None:
        raise ValueError('skinned prims require skeleton= (see clone_skeleton)')

    # resolve skin + block partition FIRST (multi-block partitioning may
    # duplicate shared vertices), so bounds are computed on the final geometry
    built = []
    for p in prims:
        p = dict(p)
        if skeleton is not None and p.get('skin') is not None:
            skin = _resolve_skin(p['skin'], skeleton, len(p['verts']))
            uvs = p.get('uvs') or [(0.0, 0.0)] * len(p['verts'])
            norms = p.get('normals') or _face_normals(p['verts'], p['faces'])
            if p.get('skin_blocks'):
                blocks = p['skin_blocks']
                assert sum(b['vert_count'] for b in blocks) == len(p['verts'])
            else:
                p['verts'], uvs, norms, skin, blocks = _auto_skin_blocks(
                    p['verts'], p['faces'], uvs, norms, skin, max_palette)
                if norms is None:
                    norms = _face_normals(
                        p['verts'], [f for b in blocks for f in b['faces']])
            p['uvs'], p['normals'] = uvs, norms
            p['_skin'], p['_blocks'] = skin, blocks
            p['faces'] = [f for b in blocks for f in b['faces']]
        else:
            p['_skin'], p['_blocks'] = None, None
        built.append(p)
    prims = built

    all_verts = [p['verts'] for p in prims]
    bbmin, bbmax = _bbox(all_verts)
    bsc, bsr = _bsphere(all_verts, bbmin, bbmax)

    lod = bytearray()
    _mesh_header(lod, name, bsc, bsr, bbmin, bbmax,
                 animated=1 if skeleton is not None else 0)

    n_sblocks = sum(1 for p in prims if p['_blocks'] is None)
    n_ablocks = sum(len(p['_blocks']) for p in prims if p['_blocks'] is not None)
    sentinel = skeleton is None          # retail skinned meshes carry no sentinel
    bone_count = skeleton['count'] if skeleton is not None else 0
    bone_name_size = len(skeleton['names_raw']) if skeleton is not None else 0
    lod += struct.pack('<4i', len(materials) + (1 if sentinel else 0),
                       len(prims), bone_count, bone_name_size)
    lod += struct.pack('<B2H', 0, n_sblocks, n_ablocks)  # ClothFlag, TSB, TAB
    if skeleton is not None:
        _write_skeleton(lod, skeleton, compress)
    lod += _IDENTITY_ROOT

    # materials (+ retail 'DegenerateTriangles' sentinel, static only)
    for slot, m in enumerate(materials):
        diff = int(m.get('diffuse_id', 0))
        bump = int(m.get('bump_id', 0))
        refl = int(m.get('reflect_id', 0))
        illum = int(m.get('illum_id', 0))
        mapflags = ((1 if diff > 0 else 0) | (2 if bump > 0 else 0) |
                    (4 if refl > 0 else 0) | (8 if illum > 0 else 0))
        _write_mat(lod, slot, m.get('name', '%s_mat%d' % (name, slot)),
                   int(m.get('decal_id', 0)), diff, bump, refl, illum, mapflags,
                   int(m.get('self_illum', 0)), m.get('two_sided', False),
                   m.get('transparent', False), m.get('boolean_alpha', False), False)
    if sentinel:
        _write_mat(lod, 0, 'DegenerateTriangles', 0, 0, 0, 0, 0, 0, 0,
                   False, False, False, True)

    # primitives
    for p in prims:
        verts = p['verts']
        faces = p['faces']
        skinned = p['_blocks'] is not None
        if len(verts) > 65535:
            raise ValueError('vertex overflow: Fable indices are u16 (max 65535)')
        uvs = p.get('uvs') or [(0.0, 0.0)] * len(verts)
        norms = p.get('normals') or _face_normals(verts, faces)
        lay = p.get('layout', layout)
        pmn, pmx = _bbox([verts])
        psc, psr = _bsphere([verts], pmn, pmx)

        if lay == 'packed':
            init = 0x04
            stride = 20 if skinned else 12
            scale = list(p.get('comp_scale') or ())
            off = list(p.get('comp_off') or ())
            if len(scale) < 4 or len(off) < 4:
                # EgoCore ImportType1: offset = bbox center, scale = extent*0.505
                off = [(pmx[j] + pmn[j]) * 0.5 for j in range(3)] + [0.0]
                scale = [max((pmx[j] - pmn[j]) * 0.505, 0.0001) for j in range(3)] + [1.0]
        elif lay == 'float':
            init = 0x14
            stride = 28 if skinned else 20
            scale = [1.0, 1.0, 1.0, 1.0]
            off = [0.0, 0.0, 0.0, 0.0]
        else:
            raise ValueError('unknown layout %r' % (lay,))

        ic = 3 * len(faces)
        lod += struct.pack('<2i', int(p.get('mat', 0)), 0)   # MaterialIndex, Reps=0
        lod += struct.pack('<3f', *psc)
        lod += struct.pack('<2f', psr, float(p.get('avg_stretch', 0.1)))
        n_sb = 0 if skinned else 1
        n_ab = len(p['_blocks']) if skinned else 0
        lod += struct.pack('<6I', n_sb, n_ab, len(verts), len(faces), ic, init)
        lod += struct.pack('<2I', n_sb, n_ab)                 # counts repeated
        if skinned:
            # CAnimatedBlock records: triangle LISTS, BonesPerVertex=3,
            # PalettedFlag=1 (all retail skinned prims checked)
            si = 0
            for b in p['_blocks']:
                pal = b['palette']
                if len(pal) > 85:
                    raise ValueError('palette overflow: joint byte is '
                                     'paletteIndex*3, so <=85 bones per block')
                lod += struct.pack('<2I3B', len(b['faces']), si, 0, 0, 0)
                lod += struct.pack('<IHB', b['vert_count'], 3, 1)
                lod += struct.pack('<B', len(pal)) + bytes(pal)
                si += 3 * len(b['faces'])
        else:
            # one CStaticBlock: whole index buffer as a triangle LIST
            lod += struct.pack('<2I3Bi', len(faces), 0, 0, 0, 0, int(p.get('mat', 0)))
        lod += struct.pack('<8f', *(list(scale) + list(off)))
        lod += struct.pack('<2I', stride, 0)                  # stride, BufferType

        vb = bytearray()
        blk = 0
        proc = 0
        for i, v in enumerate(verts):
            u, vv = uvs[i] if i < len(uvs) else (0.0, 0.0)
            cu, cv = compress_uv(u), compress_uv(1.0 - vv)    # invert decoder's V-flip
            if lay == 'packed':
                vb += struct.pack('<I', pack_pos_packed3(v, scale, off))
            else:
                vb += struct.pack('<3f', *v)
            if skinned:
                blocks = p['_blocks']
                while blk < len(blocks) - 1 and proc >= blocks[blk]['vert_count']:
                    blk += 1
                    proc = 0
                proc += 1
                jb, wb = _skin_bytes(p['_skin'][i], blocks[blk]['palette'])
                vb += jb + wb
            vb += struct.pack('<I', pack_normal(norms[i] if i < len(norms)
                                                else (0.0, 0.0, 1.0)))
            vb += struct.pack('<2h', cu, cv)
        _lzo_block(lod, vb, compress)

        ib = bytearray()
        for (a, b, c) in faces:
            # the decoder emits (ib[k], ib[k+2], ib[k+1]) -> store (a, c, b)
            ib += struct.pack('<3H', a, c, b)
        _lzo_block(lod, ib, compress)
        lod += struct.pack('<I', 0)                           # ClothPrimitiveCount

    # ghost LOD (retail convention for types 1/2/5; EgoCore CompileForExport
    # keeps the full skeleton in the ghost of an animated mesh -- retail ships
    # a REDUCED ghost skeleton, but both parse and EgoCore-compiled meshes load)
    ghost = bytearray()
    _mesh_header(ghost, name, bsc, bsr, bbmin, bbmax,
                 animated=1 if skeleton is not None else 0)
    ghost += struct.pack('<4i', 0, 0, bone_count, bone_name_size)
    ghost += struct.pack('<B2H', 0, 0, 0)
    if skeleton is not None:
        _write_skeleton(ghost, skeleton, compress)
    ghost += _IDENTITY_ROOT

    payload = bytes(lod) + bytes(ghost)

    # entry Info blob (EgoCore SerializeEntryMetadata; validated field-for-field
    # against retail single-LOD entries: infosize 60 + 4*texCount)
    tex_ids = []
    for m in materials:
        for k in ('diffuse_id', 'bump_id', 'reflect_id', 'illum_id', 'decal_id'):
            t = int(m.get(k, 0))
            if t > 0 and t not in tex_ids:
                tex_ids.append(t)
    info = bytearray()
    info += struct.pack('<i', physics_index)
    info += struct.pack('<3f', *bsc)
    info += struct.pack('<f', bsr)
    info += struct.pack('<3f', *bbmin)
    info += struct.pack('<3f', *bbmax)
    info += struct.pack('<I', 1)                  # LODCount
    info += struct.pack('<I', len(lod))           # LODSizes[0] (ghost excluded)
    info += struct.pack('<f', 0.0)                # SafeBoundingRadius (retail: 0)
    info += struct.pack('<I', len(tex_ids))
    for t in tex_ids:
        info += struct.pack('<i', t)
    return payload, bytes(info)


def _emit_faces(ib, count, start, strip, nv):
    """Triangulate one block's index span exactly like fable_core._prim_geometry
    (strip parity walk / list winding + the out-of-range vertex filter)."""
    faces = []
    if strip:
        parity = 0
        for k in range(count):
            if start + k + 2 >= len(ib):
                break
            i0 = ib[start + k]; i1 = ib[start + k + 1]; i2 = ib[start + k + 2]
            if 0xFFFF in (i0, i1, i2):
                parity = 0
                continue
            if i0 == i1 or i1 == i2 or i0 == i2:
                parity += 1
                continue
            faces.append((i0, i1, i2) if parity % 2 else (i0, i2, i1))
            parity += 1
    else:
        k = 0
        while k < count * 3:
            if start + k + 2 < len(ib):
                faces.append((ib[start + k], ib[start + k + 2], ib[start + k + 1]))
            k += 3
    return [f for f in faces if f[0] < nv and f[1] < nv and f[2] < nv]


def decode_to_compose_args(pay, mesh_type):
    """Decode an existing compiled mesh into compose_mesh() arguments (the
    RECOMPOSE oracle's front half). Returns (name, prims, materials, skeleton)
    where each prim carries the source layout + compression params so a
    recompose is quantisation-lossless. skeleton is a clone_skeleton() dict
    (None for boneless meshes); skinned prims carry `skin` (per-vertex local
    slot influences) + `skin_blocks` preserving the source animated-block
    partition, so compose_mesh re-emits the same palettes/vertex order."""
    fc._load_readers()
    name, prims, _bones = fc._parse_mesh_primitives(pay, mesh_type)
    # material walk (same field order as fable_core/_EgoCore Parse)
    c = fc._Cur(pay)
    c.s(); c.u8(); c.raw(12); c.f(); c.raw(12); c.raw(12)
    hpc = c.u16(); doc = c.u16(); pns = c.u16(); mvc = c.u16(); mgc = c.u16()
    if hpc:
        _, c.i = fc._lzo_fable(pay, c.i, 20 * hpc)
    if doc:
        _, c.i = fc._lzo_fable(pay, c.i, 56 * doc)
    if pns:
        _, c.i = fc._lzo_fable(pay, c.i, pns)
    for _ in range(mvc):
        c.u32(); c.s()
        pc = c.u32()
        if pc:
            _, c.i = fc._lzo_fable(pay, c.i, 16 * pc)
    for _ in range(mgc):
        c.raw(48); c.i32(); c.s(); c.u32(); c.u8()
    mat_count = c.i32(); c.i32(); bone_count = c.i32()
    bone_name_size = c.i32(); c.u8(); c.u16(); c.u16()
    skeleton = None
    if bone_count > 0:
        # capture the skeleton blocks raw (skinned recompose clones them; a
        # STATIC mesh that carries one, like the bear, still drops it -- its
        # vertex data does not reference bones)
        idx_raw = bytes(c.raw(2 * bone_count))
        names_raw, c.i = fc._lzo_fable(pay, c.i, bone_name_size)
        bones_raw, c.i = fc._lzo_fable(pay, c.i, 60 * bone_count)
        kf_raw, c.i = fc._lzo_fable(pay, c.i, 48 * bone_count)
        ibm_raw, c.i = fc._lzo_fable(pay, c.i, 64 * bone_count)
        bnames = []
        bp = 0
        for _ in range(bone_count):
            be = names_raw.find(b'\x00', bp)
            if be < 0:
                be = len(names_raw)
            bnames.append(names_raw[bp:be].decode('latin1'))
            bp = be + 1
        skeleton = dict(count=bone_count, indices_raw=idx_raw,
                        names_raw=bytes(names_raw), bones_raw=bytes(bones_raw),
                        keyframes_raw=bytes(kf_raw), ibms_raw=bytes(ibm_raw),
                        names=bnames)
    c.raw(48)
    mats = []
    for _ in range(mat_count):
        mid = c.i32(); mname = c.s()
        decal = c.i32(); diff = c.i32(); bump = c.i32(); refl = c.i32(); illum = c.i32()
        c.i32(); selfil = c.i32()
        two = c.u8(); trans = c.u8(); ba = c.u8(); degen = c.u8()
        if c.u8():
            for _ in range(4):
                c.s()
        if mname == 'DegenerateTriangles' and degen and diff == 0:
            continue    # retail sentinel -- compose_mesh re-appends it
        mats.append(dict(name=mname, diffuse_id=diff, bump_id=bump, reflect_id=refl,
                         illum_id=illum, decal_id=decal, self_illum=selfil,
                         two_sided=bool(two), transparent=bool(trans),
                         boolean_alpha=bool(ba), _id=mid))
    out_prims = []
    any_skinned = False
    for p in prims:
        verts, faces, loop_uv = fc._prim_geometry(p, mesh_type)
        uvs = [(0.0, 0.0)] * len(verts)
        for fi, f in enumerate(faces):
            for k in range(3):
                uvs[f[k]] = loop_uv[3 * fi + k]
        is_pos_comp = (p['init'] & 4) != 0 and (p['init'] & 0x10) == 0
        is_norm_comp = (p['init'] & 4) != 0
        skinned = p['abc'] > 0
        # recover per-vertex normals when the source stores the packed dword
        norms = None
        if is_norm_comp:
            n_off = (4 if is_pos_comp else 12) + (8 if skinned else 0)
            norms = []
            for i in range(len(verts)):
                o = i * p['stride'] + n_off
                if o + 4 <= len(p['vb']):
                    packed = struct.unpack_from('<I', p['vb'], o)[0]
                    nx = packed & 0x7FF
                    ny = (packed >> 11) & 0x7FF
                    nz = packed >> 22
                    if nx & 0x400:
                        nx -= 0x800
                    if ny & 0x400:
                        ny -= 0x800
                    if nz & 0x200:
                        nz -= 0x400
                    norms.append((nx * fc._POS_XY, ny * fc._POS_XY, nz * fc._POS_Z))
                else:
                    norms.append((0.0, 0.0, 1.0))
        op = dict(
            verts=[tuple(v) for v in verts], faces=faces, uvs=uvs, normals=norms,
            mat=max(p['mat'], 0),
            layout='packed' if is_pos_comp else 'float',
            comp_scale=list(p['scale']), comp_off=list(p['off']))
        if skinned:
            any_skinned = True
            if skeleton is None:
                raise ValueError('skinned primitive in a mesh with BoneCount=0')
            op['skin'] = fc._prim_skin(p, skeleton['count'])
            # preserve the source animated-block partition: per-block palette,
            # vertex ownership and face lists (strips converted to lists in the
            # decoder's face order, matching _prim_geometry's emit walk)
            nv = len(verts)
            blocks = []
            for ab in p['ablocks']:
                pc, si, strip, avc, groups = ab
                bfaces = _emit_faces(p['ib'], pc, si, bool(strip), nv)
                blocks.append(dict(palette=list(groups), vert_count=avc,
                                   faces=bfaces))
            op['skin_blocks'] = blocks
        out_prims.append(op)
    return name, out_prims, mats, (skeleton if any_skinned else None)
