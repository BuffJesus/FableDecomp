#!/usr/bin/env python3
"""fable_core -- bpy-FREE data layer for the Fable: TLC Blender import addon.

This module contains ALL the parse -> convert logic and NONE of the Blender
(`bpy`) calls, so it can be unit-tested standalone (see tests/test_core.py) and
reviewed independently of the Blender layer. The bpy operator layer
(`import_op.py`) consumes the plain-data structures produced here.

It CONSUMES the project readers READ-ONLY (never modifies them):
  * tools/parse_bigb.py      -- BIGB container walker
  * tools/parse_mesh.py      -- MBANK mesh descriptor / submesh / skeleton
  * tools/parse_texture.py   -- 34-byte texture Info descriptor + DXT/ARGB decoders
  * tools/lionhead_lz.py     -- LZO1X decompressor (recovered from the engine)

What this layer produces from a graphics.big (+ its texture banks):
  * ImageData      -- real decoded RGBA pixel arrays (LZO1X-decompress + DXT/ARGB)
  * MaterialData   -- material -> base-texture binding (the headline feature)
  * BoneData       -- skeleton with REAL bind poses (names + parent hierarchy +
                      inverse-bind matrices; docs/BIG_MESH_GEOMETRY.md section 7)
  * SubmeshData / LodData / MeshModel -- LOD + submesh + bounds tree
  * GeometryData -- real verts/faces/UVs per primitive (see _parse_mesh_primitives)

Geometry is now DECODED (ported from EgoCore MeshParser.h + GltfExporter.h; the
on-disk vertex/index buffers are Fable chunked-LZO -- see docs/BIG_MESH_GEOMETRY.md).
Validated: LZO-inflated vertices' bbox matches each entry's stored bbox to 0.000,
and both meshes import as real Blender geometry. Materials/textures, bone names,
and the submesh tree remain decodable as before.
"""
from __future__ import annotations

import os
import sys
import struct
from dataclasses import dataclass, field
from typing import Optional

# ---------------------------------------------------------------------------
# Import the project readers read-only. They live in tools/ (two levels up from
# this file: tools/blender_addon/io_scene_fable/fable_core.py -> tools/).
# We add tools/ to sys.path so `import parse_bigb` etc. resolve exactly as the
# readers expect (parse_mesh/parse_texture themselves `import parse_bigb`).
# ---------------------------------------------------------------------------
_THIS_DIR = os.path.dirname(os.path.abspath(__file__))
_TOOLS_DIR = os.path.normpath(os.path.join(_THIS_DIR, "..", ".."))


def _ensure_tools_on_path(tools_dir: Optional[str] = None) -> str:
    """Put the tools/ dir on sys.path so the read-only readers import.

    Returns the tools dir actually used. Callers (e.g. the bpy operator, which
    may be installed to a copied location that is NOT inside the repo) can pass
    an explicit tools_dir; otherwise the sibling repo layout is assumed.
    """
    td = tools_dir or _TOOLS_DIR
    if td not in sys.path:
        sys.path.insert(0, td)
    return td


# Lazily imported reader module handles (populated by _load_readers()).
parse_bigb = None
parse_mesh = None
parse_texture = None
lionhead_lz = None


def _load_readers(tools_dir: Optional[str] = None):
    """Import (once) the four read-only reader modules. Raises ImportError with
    an actionable message if the tools/ dir cannot be located."""
    global parse_bigb, parse_mesh, parse_texture, lionhead_lz
    if parse_bigb is not None:
        return
    td = _ensure_tools_on_path(tools_dir)
    try:
        import parse_bigb as _pb
        import parse_mesh as _pm
        import parse_texture as _pt
        import lionhead_lz as _lz
    except ImportError as ex:  # pragma: no cover - environment dependent
        raise ImportError(
            "fable_core could not import the FableTLC readers from %r. "
            "Set the 'Tools dir' in the import options to the repo's tools/ "
            "folder (containing parse_bigb.py, parse_mesh.py, parse_texture.py, "
            "lionhead_lz.py). Original error: %s" % (td, ex))
    parse_bigb, parse_mesh, parse_texture, lionhead_lz = _pb, _pm, _pt, _lz


# ===========================================================================
# Plain data structures (bpy-free). These are the contract with the bpy layer.
# ===========================================================================
@dataclass
class ImageData:
    """A decoded texture, ready to hand to bpy.data.images."""
    tex_id: int
    name: str
    width: int              # real (authored) image dimensions
    height: int
    alloc_width: int        # allocated (padded, pow2) surface dims
    alloc_height: int
    fmt_name: str           # "DXT1" / "DXT3" / "A8R8G8B8"
    # RGBA float pixels flattened bottom-row-first (Blender's image.pixels order),
    # length == alloc_width*alloc_height*4, each in [0,1]. See rgba_to_blender().
    pixels: list = field(default_factory=list, repr=False)
    source_tga: str = ""    # original build .tga path (from the entry Dep)

    @property
    def pixel_count(self) -> int:
        return self.alloc_width * self.alloc_height


@dataclass
class MaterialData:
    slot: int                       # material index within the mesh
    name: str                       # e.g. "MESH_FOO_mat0"
    base_tex_id: int                # textures.big entry ID of the base texture
    base_tex_name: str = ""         # resolved texture entry name (if found)
    image: Optional[ImageData] = None  # decoded base texture (None if undecodable)


@dataclass
class BoneData:
    name: str
    index: int
    parent: int = -1                # LOCAL-slot parent index; -1 = root
    # head/tail: model-space bind position (placeholder unless matrix is set)
    head: tuple = (0.0, 0.0, 0.0)
    tail: tuple = (0.0, 0.1, 0.0)
    # REAL bind pose (decoded from the compiled bone blocks; see
    # docs/BIG_MESH_GEOMETRY.md section 7). Empty tuple = not available
    # (name-scan fallback path) -> the bpy layer uses placeholder transforms.
    matrix: tuple = ()              # 16 floats, row-major world/model-space bind
    #                                 matrix, column-vector convention
    #                                 (translation at elements 3, 7, 11)
    global_id: int = -1             # engine-wide Fable bone ID (BoneIndices[i])


@dataclass
class SubmeshData:
    lod: int
    index: int
    n_verts: int
    n_faces: int
    svert: int                      # vertex-format class code {4,6,20,22}
    material_slot: int = -1         # best-effort binding to a MaterialData slot
    geometry: Optional["GeometryData"] = None  # filled by decode_geometry when available


@dataclass
class LodData:
    index: int
    offset: int                     # payload byte offset to the LOD block
    switch_distance: float = 0.0
    submeshes: list = field(default_factory=list)  # list[SubmeshData]


@dataclass
class GeometryData:
    """Actual mesh geometry -- currently always None (stubbed). When
    docs/BIG_MESH_GEOMETRY.md lands, decode_geometry() fills these."""
    vertices: list = field(default_factory=list)   # list[(x,y,z)]
    faces: list = field(default_factory=list)       # list[(i,j,k)]
    uvs: list = field(default_factory=list)         # per-loop [(u,v), ...]
    # Per-vertex skin weights for ANIMATED prims (docs/BIG_MESH_GEOMETRY.md §7.1).
    # Parallel to `vertices`: skin[v] = list[(local_bone_slot, weight_float)],
    # zero-weight influences dropped, weights renormalized to sum 1.0. Empty for
    # static (unskinned) prims.
    skin: list = field(default_factory=list)


@dataclass
class MeshModel:
    """The whole importable model recovered from one MBANK compiled-mesh entry."""
    name: str
    entry_id: int
    entry_type: int
    has_skeleton: bool
    origin: tuple                   # the 10-float bound/pivot block (opaque)
    materials: list = field(default_factory=list)   # list[MaterialData]
    bones: list = field(default_factory=list)        # list[BoneData]
    lods: list = field(default_factory=list)         # list[LodData]
    warnings: list = field(default_factory=list)

    @property
    def bounds_center(self) -> tuple:
        """Best-effort AABB center from the origin block (first 3 floats are a
        plausible pivot/min; kept purely for placing the placeholder empties)."""
        o = self.origin
        return (o[0], o[1], o[2]) if len(o) >= 3 else (0.0, 0.0, 0.0)


# ===========================================================================
# Texture bank index + decode
# ===========================================================================
class TextureBank:
    """Loads one or more GBANK .big files (textures.big / frontend.big) and
    resolves + decodes texture entries by ID.

    A mesh material's `matTexId` is a textures.big ENTRY ID, so we index every
    texture entry by id across all supplied banks and decode on demand.
    """

    def __init__(self, bank_paths, tools_dir: Optional[str] = None):
        _load_readers(tools_dir)
        self._by_id = {}            # id -> (buf, subname, entry)
        self._cache = {}            # id -> ImageData (decoded, memoized)
        for path in bank_paths:
            if path and os.path.isfile(path):
                self._index_bank(path)

    def _index_bank(self, path):
        with open(path, "rb") as f:
            b = f.read()
        _, _, fo, _ = parse_bigb.parse_header(b)
        subs, _ = parse_bigb.parse_footer(b, fo)
        for s in subs:
            entries, _, _, err = parse_bigb.parse_toc(b, s, fo)
            # Texture IDs are per-SUBBANK, and both GBANK_GUI_PC and GBANK_MAIN_PC
            # start their IDs at 1 -> they COLLIDE. A mesh's matTexId resolves to
            # the world/character bank GBANK_MAIN_PC (validated: seagull ids 5,6 =
            # SEAGULL_BODY/WING in MAIN, not the EDITORGUI icons in GUI). So MAIN
            # entries OVERRIDE GUI entries for a colliding id; GUI/frontend only
            # fill ids MAIN doesn't define.
            is_main = "MAIN" in s["name"].upper()
            for e in entries:
                cur = self._by_id.get(e["id"])
                if cur is None:
                    self._by_id[e["id"]] = (b, s["name"], e, is_main)
                elif is_main and not cur[3]:
                    self._by_id[e["id"]] = (b, s["name"], e, is_main)

    def has(self, tex_id: int) -> bool:
        return tex_id in self._by_id

    def entry_name(self, tex_id: int) -> str:
        rec = self._by_id.get(tex_id)
        return rec[2]["name"] if rec else ""

    def entry_subbank(self, tex_id: int) -> str:
        rec = self._by_id.get(tex_id)
        return rec[1] if rec else ""

    def count(self) -> int:
        return len(self._by_id)

    def decode(self, tex_id: int) -> Optional[ImageData]:
        """Decode a texture entry to an ImageData (LZO1X-decompress the base mip,
        then DXT/ARGB decode to RGBA). Memoized. Returns None if not decodable."""
        if tex_id in self._cache:
            return self._cache[tex_id]
        rec = self._by_id.get(tex_id)
        if not rec:
            return None
        buf, subname, e, _is_main = rec
        img = self._decode_entry(buf, e)
        self._cache[tex_id] = img
        return img

    def _decode_entry(self, buf, e) -> Optional[ImageData]:
        if e["infosize"] < 34 or e["size"] == 0:
            return None
        info = parse_texture.parse_info(e["info"])
        if not info:
            return None
        fmt = info["fmt"]
        if fmt not in parse_texture.FORMATS:
            return None
        pay = buf[e["offset"]:e["offset"] + e["size"]]

        # --- read the base (largest) mip block: u16/u32 clen prefix (framing) ---
        try:
            clen, hdr = parse_texture.payload_block_header(pay, 0)
        except Exception:
            return None
        comp = pay[hdr:hdr + clen]
        aw, ah = info["alloc_w"], info["alloc_h"]

        # expected raw byte size of the base mip (linear DXT / ARGB)
        raw_len = _base_mip_raw_len(fmt, aw, ah)
        try:
            raw = lionhead_lz.decompress(comp, raw_len)
        except Exception:
            return None
        # engine encoder emits the base mip up to 3 bytes short -> pad with zero
        if len(raw) < raw_len:
            raw = raw + b"\x00" * (raw_len - len(raw))
        elif len(raw) > raw_len:
            raw = raw[:raw_len]

        # --- decode linear pixels to RGBA (uint8 HxWx4) via parse_texture ---
        try:
            rgba = parse_texture.decode_mip(fmt, raw, aw, ah)
        except Exception:
            return None

        pixels = rgba_to_blender(rgba)
        dep = e["deps"][-1] if e.get("deps") else ""
        return ImageData(
            tex_id=e["id"], name=e["name"], width=info["width"], height=info["height"],
            alloc_width=aw, alloc_height=ah, fmt_name=info["fmt_name"],
            pixels=pixels, source_tga=dep,
        )


def _base_mip_raw_len(fmt: int, aw: int, ah: int) -> int:
    """Raw (uncompressed) byte length of the base mip for a linear surface."""
    name, unit, compressed = parse_texture.FORMATS[fmt]
    if compressed:  # DXT: `unit` bytes per 4x4 block
        bx = (aw + 3) // 4
        by = (ah + 3) // 4
        return bx * by * unit
    # A8R8G8B8: 4 bytes/pixel (unit here is bpp=32)
    return aw * ah * 4


def rgba_to_blender(rgba) -> list:
    """Convert a uint8 HxWx4 RGBA array (row 0 = top) to Blender's flat pixel
    list: float [0,1], RGBA, row-major with the BOTTOM row first (Blender stores
    images bottom-up). Uses numpy if present, else a pure-Python fallback."""
    try:
        import numpy as np
        arr = np.asarray(rgba, dtype=np.float32) / 255.0
        arr = arr[::-1]                       # flip vertically (bottom row first)
        return arr.reshape(-1).tolist()
    except Exception:
        h = len(rgba)
        out = []
        for y in range(h - 1, -1, -1):
            for px in rgba[y]:
                out.append(px[0] / 255.0)
                out.append(px[1] / 255.0)
                out.append(px[2] / 255.0)
                out.append(px[3] / 255.0)
        return out


# ===========================================================================
# Mesh (MBANK) model extraction -- descriptor + skeleton + submesh tree
# ===========================================================================
# 3ds-Max Biped bone-name tokens (per docs/BIG_MESH_FORMAT.md section 3).
# Full Biped bone-name tokens that appear (possibly with an L/R side prefix and
# a trailing digit) in the skeleton string block. Used to anchor extraction.
_BONE_ANCHORS = (
    "Scene Root", "Bip01", "Pelvis", "Spine", "Neck", "Head", "Ponytail",
    "Clavicle", "UpperArm", "Forearm", "Hand", "Finger", "Thigh", "Calf",
    "Foot", "Toe", "Tail", "Footsteps", "Wing", "Jaw", "Ear",
)

_PRINTABLE = set(range(32, 127))


def _clean_token(payload: bytes, pos: int, tok: str) -> str:
    """Given a match of anchor `tok` starting at byte `pos`, expand left over a
    small optional side/prefix ('L '/'R ') and right over a trailing digit, then
    return a clean printable bone name. Bounded so interleaved binary (the region
    is compressed, so names are partly framed by control bytes) can't leak in."""
    start = pos
    # extend left by up to 3 printable chars to catch a 'L '/'R ' side prefix
    j = pos - 1
    left = 0
    while j >= 0 and left < 3 and payload[j] in _PRINTABLE and chr(payload[j]) not in "\x00":
        # only absorb a side letter+space pattern, not arbitrary text
        if chr(payload[j]) in "LR" and pos - j <= 2:
            start = j
        j -= 1
        left += 1
    end = pos + len(tok)
    # extend right over a single trailing digit (e.g. 'Bip01', 'Toe0')
    if end < len(payload) and 48 <= payload[end] <= 57:
        end += 1
    raw = payload[start:end]
    s = "".join(chr(c) for c in raw if c in _PRINTABLE).strip()
    return s


def recover_bone_names(payload: bytes, header_end: int, first_lod_off: int) -> list:
    """Recover the skeleton bone NAMES from the compiled-mesh payload.

    The skeleton is a 3ds-Max Biped bone-name string hierarchy embedded between
    the header and the first LOD block (docs/BIG_MESH_FORMAT.md section 3). That
    region is INTERLEAVED with binary (bone matrices) and is partly framed by the
    BBB control-byte compression, so the names are NOT cleanly NUL-terminated --
    some are truncated ('Forearm'->'For', 'Calf'->'alf'). We therefore recover
    names by anchoring on the known Biped token set and extracting a bounded,
    printable-only window around each hit, in file order.

    This is HONEST PARTIAL recovery: it yields the Biped bone names that survive
    the compression framing, in order, so an armature with the right bone labels
    can be built. Real bone TRANSFORMS (head/tail matrices) and the exact parent
    hierarchy await the mesh-geometry RE (docs/BIG_MESH_GEOMETRY.md); parenting
    here is a flat 'everything under Scene Root' placeholder.
    """
    lo = max(0, header_end)
    hi = first_lod_off if first_lod_off and first_lod_off > lo else len(payload)
    region = payload[lo:hi]

    hits = []  # (position, name)
    for tok in _BONE_ANCHORS:
        tb = tok.encode("latin1")
        start = 0
        while True:
            p = region.find(tb, start)
            if p < 0:
                break
            name = _clean_token(region, p, tok)
            if name:
                hits.append((p, name))
            start = p + 1
    hits.sort(key=lambda x: x[0])

    names = []
    seen = set()
    for _pos, nm in hits:
        if nm not in seen:
            seen.add(nm)
            names.append(nm)

    bones = []
    for idx, nm in enumerate(names):
        # 'Scene Root' is the root; everything else chains under it (placeholder).
        parent = -1 if (idx == 0 or nm == "Scene Root") else 0
        bones.append(BoneData(name=nm, index=idx, parent=parent))
    return bones


# ===========================================================================
# Compiled-mesh GEOMETRY decode.
# Ported from EgoCore (MIT) MeshParser.h (C3DMeshContent::Parse) + GltfExporter.h
# (vertex attribute offsets + strip/list triangulation). The on-disk vertex &
# index buffers are Fable **chunked-LZO** (NOT plain — see docs/BIG_MESH_GEOMETRY.md
# CORRECTION). Validated against real graphics.big: the LZO-inflated vertices'
# bbox matches each entry's STORED bbox to 0.000 (MESH_SEAGULL_01, _STATUE_BEAR).
# ===========================================================================
_POS_XY = 0.0009775171
_POS_Z = 0.0019569471


def _unpack_pos_packed3(packed, scale, off):
    ix = packed & 0x7FF
    if ix & 0x400:
        ix -= 0x800
    iy = (packed >> 11) & 0x7FF
    if iy & 0x400:
        iy -= 0x800
    iz = packed >> 22
    if iz & 0x200:
        iz -= 0x400
    return (ix * _POS_XY * scale[0] + off[0],
            iy * _POS_XY * scale[1] + off[1],
            iz * _POS_Z * scale[2] + off[2])


def _decompress_uv(v):
    return v / 2048.0 - 8.0


def _calc_stride(init_flags, is_animated):
    is_pos_comp = (init_flags & 4) != 0 and (init_flags & 0x10) == 0
    is_norm_comp = (init_flags & 4) != 0
    has_bump = (init_flags & 2) != 0
    s = 4 if is_pos_comp else 12
    if is_animated:
        s += 8
    s += 4 if is_norm_comp else 12
    s += 4 if is_norm_comp else 8
    if has_bump:
        s += 8 if is_norm_comp else 16
    return s


def _lzo_fable(data, cur, expected):
    """Fable chunked-LZO framing (port of EgoCore Utils.h DecompressLZO):
    a run of [u16 clen][clen LZO bytes] (0xFFFF->u32 escape, clen==0->stored
    chunk) filling expected-3 bytes, then 3 trailing plain bytes. Uses the
    engine-recovered LZO1X (lionhead_lz). Returns (bytes, new_cursor)."""
    out = bytearray(expected)
    total = 0
    target = expected - 3 if expected > 3 else 0
    n = len(data)
    while total < target and cur + 2 <= n:
        comp = struct.unpack_from('<H', data, cur)[0]
        cur += 2
        if comp == 0xFFFF:
            if cur + 4 > n:
                break
            comp = struct.unpack_from('<I', data, cur)[0]
            cur += 4
        if comp == 0:  # stored (uncompressed) chunk
            take = min(target - total, n - cur)
            out[total:total + take] = data[cur:cur + take]
            cur += take
            total += take
        else:
            if cur + comp > n:
                break
            dec = lionhead_lz.decompress(bytes(data[cur:cur + comp]), expected - total)
            take = min(len(dec), expected - total)
            out[total:total + take] = dec[:take]
            cur += comp
            total += take
            if len(dec) == 0:
                break
    if cur + 3 <= n and expected >= 3:  # trailing 3 plain bytes
        out[expected - 3:expected] = data[cur:cur + 3]
        cur += 3
    return bytes(out), cur


class _Cur:
    """Minimal cursor over the compiled-mesh payload (matches EgoCore's Read<T>)."""
    __slots__ = ('b', 'i')

    def __init__(self, b):
        self.b = b
        self.i = 0

    def s(self):
        st = self.i
        while self.i < len(self.b) and self.b[self.i] != 0:
            self.i += 1
        r = self.b[st:self.i].decode('latin1')
        if self.i < len(self.b):
            self.i += 1
        return r

    def take(self, fmt, n):
        v = struct.unpack_from(fmt, self.b, self.i)
        self.i += n
        return v

    def i32(self):
        return self.take('<i', 4)[0]

    def u32(self):
        return self.take('<I', 4)[0]

    def u16(self):
        return self.take('<H', 2)[0]

    def u8(self):
        return self.take('<B', 1)[0]

    def f(self):
        return self.take('<f', 4)[0]

    def raw(self, n):
        r = self.b[self.i:self.i + n]
        self.i += n
        return r


def _invert4(m):
    """Invert a 4x4 matrix given as a flat row-major 16-list. Gauss-Jordan with
    partial pivoting; returns a flat 16-list. Raises ValueError if singular."""
    a = [[m[r * 4 + c] for c in range(4)] + [1.0 if r == c else 0.0 for c in range(4)]
         for r in range(4)]
    for col in range(4):
        piv = max(range(col, 4), key=lambda r: abs(a[r][col]))
        if abs(a[piv][col]) < 1e-12:
            raise ValueError("singular matrix")
        a[col], a[piv] = a[piv], a[col]
        d = a[col][col]
        a[col] = [x / d for x in a[col]]
        for r in range(4):
            if r != col and a[r][col] != 0.0:
                f = a[r][col]
                a[r] = [x - f * y for x, y in zip(a[r], a[col])]
    return [a[r][4 + c] for r in range(4) for c in range(4)]


def _decode_bones(idx_raw, names_raw, bone_raw, ibm_raw, bone_count):
    """Decode the four per-bone blocks into BoneData with REAL bind poses.

    Layouts confirmed from EgoCore (MeshParser.h C3DMeshContent::Parse +
    MeshProperties.h UpdateAnimationBones + GltfExporter.h) -- see
    docs/BIG_MESH_GEOMETRY.md section 7:
      idx_raw   -- u16[bone_count] local slot -> GLOBAL Fable bone ID (NOT parents)
      names_raw -- bone_count NUL-terminated names, slot order
      bone_raw  -- 60 B/bone: u32 NameCRC, i32 ParentIndex (LOCAL, -1=root),
                   i32 OriginalNoChildren, float LocalizationMatrix[12]
      ibm_raw   -- 64 B/bone: 4x4 float INVERSE bind matrix, row-major storage
                   of a column-vector-convention matrix (translation at elements
                   3/7/11); the 4th row is junk and is forced to (0,0,0,1).
    World bind = inverse(IBM); its translation is the bone head in model space.
    """
    # names
    names = []
    p = 0
    for _ in range(bone_count):
        e = names_raw.find(b"\x00", p)
        if e < 0:
            e = len(names_raw)
        names.append(names_raw[p:e].decode("latin1"))
        p = e + 1
    bones = []
    for i in range(bone_count):
        parent = struct.unpack_from('<i', bone_raw, i * 60 + 4)[0]
        if parent >= bone_count:      # defensive: engine treats these as roots too
            parent = -1
        raw = list(struct.unpack_from('<16f', ibm_raw, i * 64))
        raw[12:16] = [0.0, 0.0, 0.0, 1.0]
        world = _invert4(raw)         # bind (model-space) matrix
        gid = struct.unpack_from('<H', idx_raw, i * 2)[0] if len(idx_raw) >= 2 * (i + 1) else -1
        head = (world[3], world[7], world[11])
        bones.append(BoneData(
            name=names[i] if i < len(names) and names[i] else "Bone_%d" % i,
            index=i, parent=parent, head=head,
            tail=(head[0], head[1] + 0.1, head[2]),
            matrix=tuple(world), global_id=gid))
    return bones


def _parse_mesh_primitives(pay, mesh_type):
    """Walk one LOD0 mesh content (EgoCore C3DMeshContent::Parse), LZO-inflating
    each primitive's vertex & index buffer.
    Returns (mesh_name, [prim dicts], [BoneData] | None)."""
    c = _Cur(pay)
    name = c.s()
    c.u8()                      # AnimatedFlag
    c.raw(12); c.f(); c.raw(12); c.raw(12)   # bs center/radius, bbmin, bbmax
    hpc = c.u16(); doc = c.u16(); pns = c.u16(); mvc = c.u16(); mgc = c.u16()
    if hpc > 0:
        _, c.i = _lzo_fable(pay, c.i, 20 * hpc)
    if doc > 0:
        _, c.i = _lzo_fable(pay, c.i, 56 * doc)
    if pns > 0:
        _, c.i = _lzo_fable(pay, c.i, pns)
    for _ in range(mvc):
        c.u32(); c.s()
        pc = c.u32()
        if pc > 0:
            _, c.i = _lzo_fable(pay, c.i, 16 * pc)
    for _ in range(mgc):
        c.raw(48); c.i32(); c.s(); c.u32(); c.u8()
    mat_count = c.i32(); prim_count = c.i32(); bone_count = c.i32()
    bone_name_size = c.i32(); c.u8(); c.u16(); c.u16()
    bones = None
    if bone_count > 0:
        idx_raw = c.raw(2 * bone_count)
        names_raw, c.i = _lzo_fable(pay, c.i, bone_name_size)
        bone_raw, c.i = _lzo_fable(pay, c.i, 60 * bone_count)
        _, c.i = _lzo_fable(pay, c.i, 48 * bone_count)   # per-bone TRS records
        ibm_raw, c.i = _lzo_fable(pay, c.i, 64 * bone_count)
        try:
            bones = _decode_bones(idx_raw, names_raw, bone_raw, ibm_raw, bone_count)
        except Exception:
            bones = None    # callers fall back to the name-scan path
    c.raw(48)                   # RootMatrix
    for _ in range(mat_count):
        c.i32(); c.s()
        for _ in range(7):
            c.i32()
        c.u8(); c.u8(); c.u8(); c.u8()
        if c.u8():              # UseFilenames -> 4 strings
            for _ in range(4):
                c.s()
    prims = []
    for _ in range(prim_count):
        p = {}
        p['mat'] = c.i32(); p['reps'] = c.i32()
        c.raw(12); c.f(); c.f()          # SphereCenter, SphereRadius, AvgTextureStretch
        sbc = c.u32(); abc = c.u32()
        p['vc'] = c.u32(); p['tc'] = c.u32(); p['ic'] = c.u32(); p['init'] = c.u32()
        c.u32(); c.u32()
        p['abc'] = abc
        sblocks = []
        for _ in range(sbc):
            pc = c.u32(); si = c.u32(); strip = c.u8(); c.u8(); c.u8(); mi = c.i32()
            sblocks.append((pc, si, strip, mi))
        ablocks = []
        for _ in range(abc):
            pc = c.u32(); si = c.u32(); strip = c.u8(); c.u8(); c.u8()
            avc = c.u32(); c.u16(); c.u8()          # VertexCount, BonesPerVertex, PalettedFlag
            gc = c.u8()
            groups = list(c.raw(gc)) if gc > 0 else []
            # (PrimitiveCount, StartIndex, IsStrip, VertexCount, Groups[]) --
            # VertexCount partitions verts into blocks; Groups[] is the per-block
            # LOCAL-bone palette used to decode per-vertex joint bytes (docs §7.1).
            ablocks.append((pc, si, strip, avc, groups))
        comp = c.raw(32)
        p['scale'] = struct.unpack_from('<4f', comp, 0)
        p['off'] = struct.unpack_from('<4f', comp, 16)
        stride = c.u32(); c.u32()        # on-disk stride, buffer type
        p['stride'] = stride if stride > 0 else _calc_stride(p['init'], abc > 0)
        reps = p['reps'] if p['reps'] > 1 else 1
        vbsz = p['stride'] * p['vc'] * reps
        if vbsz > 0:
            p['vb'], c.i = _lzo_fable(pay, c.i, vbsz)
        else:
            if c.i + 2 <= len(pay) and struct.unpack_from('<H', pay, c.i)[0] == 0:
                c.i += 2
            p['vb'] = b''
        ibsz = 2 * reps * p['ic']
        if ibsz > 0:
            ib, c.i = _lzo_fable(pay, c.i, ibsz)
            p['ib'] = list(struct.unpack_from('<%dH' % (len(ib) // 2), ib, 0))
        else:
            if c.i + 2 <= len(pay) and struct.unpack_from('<H', pay, c.i)[0] == 0:
                c.i += 2
            p['ib'] = []
        for _ in range(c.u32()):          # cloth primitives (skip)
            c.u32(); c.u32()
            pl = c.u32()
            if pl > 0:
                _, c.i = _lzo_fable(pay, c.i, pl)
        p['sblocks'] = sblocks
        p['ablocks'] = ablocks
        prims.append(p)
    return name, prims, bones


def _prim_geometry(p, mesh_type):
    """Extract (vertices, faces, per-loop uvs) from one primitive. Vertex-attribute
    offsets + strip/list triangulation ported from EgoCore GltfExporter.h."""
    vb = p['vb']; st = p['stride']; init = p['init']
    reps = p['reps'] if p['reps'] > 1 else 1
    has_bones = p['abc'] > 0
    is_pos_comp = (init & 4) != 0 and (init & 0x10) == 0
    is_norm_comp = (init & 4) != 0
    i_off = 4 if is_pos_comp else 12
    norm_off = i_off + (8 if has_bones else 0)
    u_off = norm_off + (4 if is_norm_comp else 12)
    if mesh_type == 4 or (st == 36 and not has_bones):
        is_pos_comp = False; is_norm_comp = False; u_off = 24
    elif not has_bones:
        if st == 24 and not is_pos_comp and not is_norm_comp:
            u_off = 16
        elif st == 20 and is_pos_comp and not is_norm_comp:
            u_off = 12
    n = p['vc'] * reps
    verts = []; vert_uv = []
    for v in range(n):
        o = v * st
        if o + 12 > len(vb):
            break
        if is_pos_comp:
            verts.append(_unpack_pos_packed3(struct.unpack_from('<I', vb, o)[0],
                                             p['scale'], p['off']))
        else:
            verts.append(struct.unpack_from('<3f', vb, o))
        u = vc = 0.0
        if o + u_off + 4 <= len(vb):
            if is_norm_comp:
                if mesh_type == 4:
                    t = struct.unpack_from('<2H', vb, o + u_off)
                    u = t[0] / 65535.0; vc = t[1] / 65535.0
                else:
                    t = struct.unpack_from('<2h', vb, o + u_off)
                    u = _decompress_uv(t[0]); vc = _decompress_uv(t[1])
            else:
                t = struct.unpack_from('<2f', vb, o + u_off)
                u = t[0]; vc = t[1]
        vert_uv.append((u, 1.0 - vc))       # V-flip for Blender's bottom-left origin
    ib = p['ib']
    faces = []

    def emit(count, start, strip):
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

    if reps > 1:
        emit(p['tc'], 0, False)
    else:
        wrote = False
        for (pc, si, strip, _mi) in p['sblocks']:
            emit(pc, si, bool(strip)); wrote = True
        for ab in p['ablocks']:
            pc, si, strip = ab[0], ab[1], ab[2]
            emit(pc, si, bool(strip)); wrote = True
        if not wrote and ib:
            emit(len(ib) // 3, 0, False)
    nv = len(verts)
    faces = [f for f in faces if f[0] < nv and f[1] < nv and f[2] < nv]
    loop_uv = []
    for (a, b2, c2) in faces:
        loop_uv.append(vert_uv[a]); loop_uv.append(vert_uv[b2]); loop_uv.append(vert_uv[c2])
    return verts, faces, loop_uv


def _prim_skin(p, bone_count):
    """Decode per-vertex skin weights for one ANIMATED primitive.

    Returns a list parallel to the vertex list: skin[v] = [(local_bone, weight), ...]
    (zero-weight influences dropped, weights renormalized). Empty list if the
    primitive is static (abc == 0). Layout: docs/BIG_MESH_GEOMETRY.md §7.1, ported
    from EgoCore GltfExporter.h (the `hasBones` branch of the vertex loop).

    Joint bytes are triangle-stride palette indices: pID = ind[k]//3 indexes the
    animated block's Groups[] palette -> LOCAL bone slot. Vertices are partitioned
    into animated blocks by VertexCount; each block has its own Groups[] palette.
    """
    if p['abc'] <= 0:
        return []
    vb = p['vb']; st = p['stride']; init = p['init']
    reps = p['reps'] if p['reps'] > 1 else 1
    is_pos_comp = (init & 4) != 0 and (init & 0x10) == 0
    i_off = 4 if is_pos_comp else 12       # joint indices offset
    w_off = i_off + 4                       # weights offset
    n = p['vc'] * reps
    ablocks = p['ablocks']
    # block-walk state (mirrors EgoCore: blk/proc/limit)
    blk = 0
    proc = 0
    limit = ablocks[0][3] if ablocks else 0x7FFFFFFF
    out = []
    for v in range(n):
        if proc >= limit:
            blk += 1
            proc = 0
            limit = ablocks[blk][3] if blk < len(ablocks) else 0x7FFFFFFF
        proc += 1
        o = v * st
        groups = ablocks[blk][4] if blk < len(ablocks) else []
        if o + w_off + 4 > len(vb):
            out.append([(0, 1.0)])
            continue
        ind = vb[o + i_off:o + i_off + 4]
        wgt = vb[o + w_off:o + w_off + 4]
        w = [wgt[0] / 255.0, wgt[1] / 255.0, wgt[2] / 255.0, wgt[3] / 255.0]
        s = w[0] + w[1] + w[2] + w[3]
        if s > 0.001:
            w = [x / s for x in w]
        else:
            w = [1.0, 0.0, 0.0, 0.0]
        infl = {}
        for k in range(4):
            if w[k] <= 0.0:
                continue
            pid = ind[k] // 3
            lid = groups[pid] if pid < len(groups) else 0
            if lid >= bone_count:
                lid = 0
            infl[lid] = infl.get(lid, 0.0) + w[k]   # merge dup slots
        if not infl:
            infl = {0: 1.0}
        out.append(sorted(infl.items(), key=lambda kv: -kv[1]))
    return out


def decode_geometry(payload: bytes, submesh, lod_off: int) -> Optional[GeometryData]:
    """GEOMETRY STUB -- returns None today. (Blocker corrected 2026-07-19.)

    CORRECTION: geometry is NOT compressed. docs/BIG_MESH_GEOMETRY.md (geometry-RE
    agent, from the decompiled Fable.exe loader) REFUTES the old "Big Blue Box
    control-byte compression" claim in BIG_MESH_FORMAT.md section 6: there is no
    decompressor anywhere on the mesh load path. A compiled MBANK entry is a raw
    relocatable serialized C3DMesh2 memory image -- vertices are Read(nVerts*stride)
    and indices are a u16 memcpy, both copied byte-for-byte.

    Why this still returns None: the compiled image has NO chunk framing, so the
    vertex/index blocks are NOT locatable by byte pattern-scan (5 scans tried, all
    correctly failed -- see BIG_MESH_GEOMETRY.md section 4). Filling this body
    requires porting the deterministic C3DMesh2::LoadBinary walk end-to-end
    (stats -> volumes -> generators -> bone table -> matrices -> materials ->
    per-primitive blocks -> vertex buffer -> index buffer) to POSITION the reader;
    once positioned, sections 2/3 of that doc are the complete decode spec:
        vertices : list[(x,y,z)]   FLOAT3 (type 2) or dequantized s16x3 (type 10)
        faces    : list[(i,j,k)]   plain u16 index array
        uvs      : per-loop [(u,v)] from the s11e4 tu/tv dwords
    This is a finite serialization port, NOT codec-reversing -- the hard RE unknown
    is closed. The call site in build_model() already wires this in, so only this
    body needs implementing -- nothing else changes.

    Inputs available for the future implementation:
      payload  -- the full compiled-mesh payload bytes
      submesh  -- a SubmeshData (has n_verts, n_faces, svert)
      lod_off  -- byte offset of the enclosing LOD block
    """
    # TODO(geometry): port the C3DMesh2::LoadBinary walk (BIG_MESH_GEOMETRY.md section 4)
    # to locate the vertex/index blocks; the codec question is settled (no codec).
    return None


def _resolve_material_names(mats, tex_bank: Optional[TextureBank]):
    for m in mats:
        if tex_bank and tex_bank.has(m.base_tex_id):
            m.base_tex_name = tex_bank.entry_name(m.base_tex_id)


def build_model(buf: bytes, entry, tex_bank: Optional[TextureBank] = None,
                decode_textures: bool = True, tools_dir: Optional[str] = None) -> MeshModel:
    """Build a MeshModel from one MBANK compiled-mesh TOC entry.

    buf       -- the full graphics.big bytes
    entry     -- a parse_bigb TOC entry dict (type in {1,2,4,5}, compiled-mesh)
    tex_bank  -- optional TextureBank for material name resolution + decoding
    """
    _load_readers(tools_dir)
    pay = buf[entry["offset"]:entry["offset"] + entry["size"]]
    warnings = []

    desc = parse_mesh.parse_mesh_descriptor(entry["info"], pay, entry["name"])
    if desc is None:
        # Fall back to a descriptor-less parse (rare degenerate meshes).
        desc = parse_mesh.parse_mesh_descriptor(entry["info"])
    if desc is None:
        warnings.append("mesh descriptor did not parse zero-leftover; "
                        "materials/LODs unavailable for this entry")
        return MeshModel(name=entry["name"], entry_id=entry["id"],
                         entry_type=entry["type"], has_skeleton=False,
                         origin=(0.0,) * 10, warnings=warnings)

    # --- payload header (name, skeleton flag, 10-float origin) ---
    try:
        hdr = parse_mesh.parse_compiled_header(pay)
        has_skel = bool(hdr["skel_flag"])
        origin = hdr["origin"]
        header_end = hdr["header_end"]
    except Exception:
        has_skel = False
        origin = desc["origin"]
        header_end = 0

    # --- materials (matTexId -> base texture) ---
    materials = []
    for slot, tid in enumerate(desc["matTexId"]):
        md = MaterialData(slot=slot, name="%s_mat%d" % (entry["name"], slot),
                          base_tex_id=tid)
        materials.append(md)
    _resolve_material_names(materials, tex_bank)
    if decode_textures and tex_bank is not None:
        for md in materials:
            md.image = tex_bank.decode(md.base_tex_id)

    # --- primitive parse (geometry + REAL skeleton bind poses) ---
    # Run the EgoCore-derived payload walk ONCE; it yields both the per-primitive
    # geometry and the decoded bone blocks (names + parent hierarchy + bind
    # matrices -- docs/BIG_MESH_GEOMETRY.md section 7).
    prims = None
    real_bones = None
    try:
        _nm, prims, real_bones = _parse_mesh_primitives(pay, entry["type"])
    except Exception as ex:   # fall back to the placeholder submesh tree below
        warnings.append("primitive geometry parse failed (%s); "
                        "using submesh placeholders" % ex)

    # --- skeleton: real bind poses when decoded, name-scan fallback otherwise ---
    bones = []
    if real_bones:
        bones = real_bones
    elif has_skel:
        first_lod = desc["lod"][0] if desc["lod"] else 0
        bones = recover_bone_names(pay, header_end, first_lod)
        if bones:
            warnings.append("bone bind poses undecodable; NAME-scan fallback "
                            "(placeholder transforms)")
        else:
            warnings.append("skeleton flag set but no Biped bone names recovered")

    # --- LOD + submesh tree ---
    lods = []
    lod_dists = desc.get("lod_dist", []) or []
    for li, loff in enumerate(desc["lod"]):
        lod = LodData(index=li, offset=loff,
                      switch_distance=(lod_dists[li] if li < len(lod_dists) else 0.0))
        # Only LOD0's block start is precisely known (lod[0] = byte offset). For
        # deeper LODs the descriptor stores sizes, not offsets, so we scan from
        # lod[0] for LOD0 and leave deeper LODs as structural placeholders unless
        # a submesh scan finds headers. We surface LOD0 submeshes concretely.
        if li == 0:
            # Geometry from the EgoCore-derived primitive parse (validated: the
            # LZO-inflated vertices' bbox matches the entry's stored bbox to
            # 0.000). One Blender submesh per compiled PRIMITIVE with real
            # verts/faces/UVs. This supersedes the find_subm_headers heuristic
            # (which returned 0 submeshes for some static meshes, e.g. the bear).
            built = False
            if prims is not None:
                bone_ct = len(real_bones) if real_bones else 0
                for si, prim in enumerate(prims):
                    verts, faces, luvs = _prim_geometry(prim, entry["type"])
                    skin = _prim_skin(prim, bone_ct) if (bone_ct and prim['abc'] > 0) else []
                    geo = (GeometryData(vertices=verts, faces=faces, uvs=luvs, skin=skin)
                           if verts and faces else None)
                    mslot = (prim["mat"] if 0 <= prim["mat"] < len(materials)
                             else (si if si < len(materials) else -1))
                    lod.submeshes.append(SubmeshData(
                        lod=li, index=si, n_verts=prim["vc"], n_faces=len(faces),
                        svert=prim["stride"], material_slot=mslot, geometry=geo))
                built = len(prims) > 0
            if not built:
                for si, sm in enumerate(parse_mesh.find_subm_headers(pay, loff)):
                    lod.submeshes.append(SubmeshData(
                        lod=li, index=si, n_verts=sm["nVerts"], n_faces=sm["nFaces"],
                        svert=sm["sVert"],
                        material_slot=(si if si < len(materials) else -1)))
        lods.append(lod)

    return MeshModel(name=entry["name"], entry_id=entry["id"],
                     entry_type=entry["type"], has_skeleton=has_skel,
                     origin=tuple(origin), materials=materials, bones=bones,
                     lods=lods, warnings=warnings)


# ===========================================================================
# Top-level convenience: open a graphics.big, list/import compiled meshes
# ===========================================================================
def open_graphics_big(path: str, tools_dir: Optional[str] = None):
    """Return (buf, [compiled-mesh entries]) from a graphics.big. Compiled meshes
    are entry types {1,2,4,5} whose payload begins with 'MESH_'."""
    _load_readers(tools_dir)
    with open(path, "rb") as f:
        b = f.read()
    magic, _, fo, _ = parse_bigb.parse_header(b)
    if magic != b"BIGB":
        raise ValueError("%s is not a BIGB container" % path)
    subs, _ = parse_bigb.parse_footer(b, fo)
    meshes = []
    for s in subs:
        entries, _, _, err = parse_bigb.parse_toc(b, s, fo)
        for e in entries:
            if e["type"] in parse_mesh.MESH_TYPES:
                if parse_mesh.classify_payload(b, e) == "compiled-mesh":
                    e["sub"] = s["name"]
                    meshes.append(e)
    return b, meshes


def guess_texture_banks(graphics_big_path: str) -> list:
    """Given a graphics.big path, guess the sibling texture bank paths
    (textures.big + frontend.big) under the standard install layout:
        <data>/graphics/graphics.big
        <data>/graphics/pc/textures.big
        <data>/graphics/pc/frontend.big
    Returns the ones that exist."""
    gdir = os.path.dirname(os.path.abspath(graphics_big_path))
    candidates = [
        os.path.join(gdir, "pc", "textures.big"),
        os.path.join(gdir, "pc", "frontend.big"),
        os.path.join(gdir, "textures.big"),
        os.path.join(gdir, "frontend.big"),
    ]
    return [c for c in candidates if os.path.isfile(c)]


def find_entry(buf, meshes, name_or_id):
    """Locate a compiled-mesh entry by exact name or by numeric id."""
    try:
        want_id = int(name_or_id)
    except (TypeError, ValueError):
        want_id = None
    for e in meshes:
        if want_id is not None and e["id"] == want_id:
            return e
        if e["name"] == name_or_id:
            return e
    return None
