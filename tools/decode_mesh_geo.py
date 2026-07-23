#!/usr/bin/env python3
"""decode_mesh_geo.py — MBANK compiled-mesh GEOMETRY decode facts + helpers.

GROUND TRUTH (from the Fable.exe mesh loader, decompiled — see docs/BIG_MESH_GEOMETRY.md):
The compiled MBANK mesh geometry is **PLAIN / UNCOMPRESSED**. There is NO decompressor
(no LZO, no zlib, no bespoke control-byte codec) anywhere on the mesh load path. A compiled
mesh entry is a **relocatable serialized memory image** of a C3DMesh2 object graph. Loading =
deserialize the flat image via CMemoryDataInputStream (plain Read()/memcpy of every field) and
rebase embedded pointers (the RelocateData family). Vertices and indices are copied byte-for-byte
straight into locked D3D vertex/index buffers.

Loader chain (retail addresses, Fable.exe @ ImageBase 0x400000):
  C3DMeshLODInfo::OnResourceLoaded  0x00a25470  -> BuildMesh
  C3DMeshLODInfo::BuildMesh         0x00a24520  -> C3DMesh2::LoadBinary + C3DPrimitive2::Upload*Buffers
  C3DMesh2::LoadBinary              0x00a8ad40  top-level deserializer (counts, bones, materials, prims)
  C3DPrimitive2::LoadBinary         0x00a92900  per-primitive: vertex buffer + index buffer read
  C3DPrimitive2::LoadVertexBuffer   0x00a91560  Read(nVerts*stride) -> vertex buffer (raw memcpy)
                                                (index buffer read inline in LoadBinary: Read(nIdx*2))
  C3DPrimitive2::CalculateVertexSize 0x00a910e0 stride from FVF flags (via CVertexShaderInput*)
  CVertexShaderInputStatic::Initialise   0x00a90560   vertex-declaration element list (static)
  CVertexShaderInputAnimated::Initialise 0x00a90630   vertex-declaration element list (skinned)
  CVertexShaderInput::GenerateD3DDeclaration 0x00ab3870  type->byte-size table (stride accumulator)
  CIndexBuffer::Initialise          0x00ac1f70  u16 index buffer (EIndexBufferFormat, tri type=4)

The tagged-BBM (Type-3) path is likewise plain (verified independently):
  C3DMeshFileXVertexDataChunk::ReadFromFile   0x00aa66b0  resize(stride*count); raw block copy
  C3DMeshFileXTriangleStripChunk::ReadFromFile 0x00aa6960  resize(count); raw u16 copy
  C3DMeshFilePrimitiveChunk::ReadFromFile     0x00abcf40  plain u32 material-index array
(The 'control bytes' an earlier agent saw are the CBYTE-encoded material/submesh index list in
 C3DMeshFilePrimitiveChunk + the quantized vertex field values + CVertexCompressionParams — NOT a codec.)

VERTEX-COMPONENT TYPE -> BYTE SIZE  (from GenerateD3DDeclaration @ 0x00ab3870 switch):
    types 0,4,5,7,9,0xc,0xd -> 4 bytes    (D3DCOLOR / packed dword: packed normal, s11e4 UV pair,
                                            UBYTE4 bone indices/weights, etc.)
    types 1,6,0xb          -> 8 bytes    (FLOAT2 / SHORT4 / packed pair)
    type  2                -> 12 bytes   (FLOAT3  — float position)
    type  3                -> 16 bytes   (FLOAT4)
    type  8                -> 2 bytes     (SHORT1-class)
    type  10 (0xa)         -> 6 bytes     (SHORT3-class — packed s16x3 position)

VERTEX DECLARATION per FVF-flag class (flags = primitive dword & ~4, see LoadBinary @ 0xa92900):
  Static, packed-coord class  (flags & 4 == 0):
     elem0=type2(12)  elem1=type2(12)  elem2=type1(8)  [+ type3(16) if flags&2]
  Static, float-coord class   (flags & 4 != 0):
     elem0 = type2(12) if flags&0x10 else type0xd(4)
     elem1 = type0xc(4)   elem2 = type7(4)   [+ type0xb(8) if flags&2]
  Animated/skinned (CVertexShaderInputAnimated) adds a bones element (type per flags).
  Morph/param case (CalculateVertexSize param_3==true) -> fixed stride 0x24 = 36.

INDEX ENCODING: u16 (unsigned short). CIndexBuffer::Initialise(..., EPrimitiveType=4). The runtime
index count = primitiveField[0x60] * repeatMultiplier; on-disk the base index array is stored plainly
(2 bytes each) and read with a single memcpy in C3DPrimitive2::LoadBinary. Positions are quantized;
dequantization is done in the vertex SHADER at draw time using CVertexCompressionParams (8 dwords:
4 float scale + 4 bias) that the loader copies verbatim into the vertex-buffer object (+0x14).

OPEN GAP (documented, not compression): to extract vertices from an arbitrary compiled entry with a
pure-Python reader you must replicate the full deterministic LoadBinary walk (C3DMeshStats -> mesh
volumes -> generator objects -> bone-name table -> bone matrices -> C3DMeshMaterial[] ->
per-primitive CStaticBlock[] -> CVertexCompressionParams -> vertex buffer -> index buffer) to land on
the vertex block; it is NOT locatable by byte pattern-scan because there is no chunk framing in the
compiled image (it is a raw relocatable struct dump). This is a serialization-replication task, not a
codec-reversing task. The stride/type/index facts above are the complete decode spec once positioned.
"""
import struct

# type-code -> byte size, from GenerateD3DDeclaration @ 0x00ab3870
VTYPE_SIZE = {0:4, 1:8, 2:12, 3:16, 4:4, 5:4, 6:8, 7:4, 8:2, 9:4, 10:6, 0xb:8, 0xc:4, 0xd:4}


def static_decl(flags):
    """Return the list of vertex-declaration component type codes for the STATIC path,
    given the primitive FVF flags (see CVertexShaderInputStatic::Initialise @ 0x00a90560)."""
    elems = []
    if (flags & 4) == 0:                       # packed-coord class
        elems += [2, 2, 1]
        if flags & 2:
            elems.append(3)
    else:                                      # float-coord class
        elems.append(2 if (flags & 0x10) else 0xd)
        elems += [0xc, 7]
        if flags & 2:
            elems.append(0xb)
    return elems


def animated_decl(flags):
    """Vertex-declaration type codes for the ANIMATED/skinned path
    (CVertexShaderInputAnimated::Initialise @ 0x00a90630)."""
    elems = []
    elems.append(2 if ((flags & 4) and not (flags & 0x10)) else (0xd if (flags & 4) else 2))
    # elem0: (flags&4 && !(flags&0x10)) -> 0xd else 2   [re-derive carefully]
    elems = []
    if (flags & 4) == 0 or (flags & 0x10):
        elems.append(2)
    else:
        elems.append(0xd)
    elems += [4, 4]                            # elem1,2 = type4 (D3DCOLOR-class, 4B)
    elems.append(0xc if (flags & 4) else 2)    # elem3
    elems.append(7 if (flags & 4) else 1)      # elem4
    if flags & 2:
        elems.append(0xb if (flags & 4) else 3)
    return elems


def stride_of(type_codes):
    """Sum the byte sizes of a vertex-declaration type-code list = the vertex stride."""
    return sum(VTYPE_SIZE[t] for t in type_codes)


def static_stride(flags):
    return stride_of(static_decl(flags))


def animated_stride(flags):
    return stride_of(animated_decl(flags))


def decode_indices_u16(buf, off, count):
    """Decode `count` plain little-endian u16 indices (the on-disk / D3D index format)."""
    return list(struct.unpack_from('<%dH' % count, buf, off))


def decode_positions(buf, off, n, stride, pos_type=2, scale=(1.0, 1.0, 1.0), bias=(0.0, 0.0, 0.0)):
    """Decode `n` vertex positions from a contiguous vertex buffer at `off`.
    pos_type 2 = FLOAT3 (12B, float coords). pos_type 10 = SHORT3 (packed s16x3), dequantized by
    scale/bias from CVertexCompressionParams. Positions are always the first element of the stride."""
    out = []
    for i in range(n):
        b = off + i * stride
        if pos_type == 2:
            x, y, z = struct.unpack_from('<3f', buf, b)
        elif pos_type == 10:
            x, y, z = struct.unpack_from('<3h', buf, b)
            x = x * scale[0] + bias[0]; y = y * scale[1] + bias[1]; z = z * scale[2] + bias[2]
        else:
            raise ValueError("unsupported pos_type %d" % pos_type)
        out.append((x, y, z))
    return out


if __name__ == '__main__':
    print(__doc__)
    print("Example strides:")
    for fl in (0, 2, 4, 6, 0x14, 0x16):
        print("  flags=0x%02x  static_decl=%s stride=%d | animated stride=%d" % (
            fl, static_decl(fl), static_stride(fl), animated_stride(fl)))
