#include "candidates/landscape_layer_mesh_load_foreground_compiled.h"

CDataInputStream::CDataInputStream(const void* data, unsigned long size)
    : Data(static_cast<const fable_u8*>(data)), Size(size), Position(0)
{
}

bool CDataInputStream::Read8(fable_u8& value)
{
    if (Position + 1 > Size)
        return false;
    value = Data[Position++];
    return true;
}

bool CDataInputStream::Read16(fable_u16& value)
{
    if (Position + 2 > Size)
        return false;
    value = static_cast<fable_u16>(Data[Position]) |
        static_cast<fable_u16>(Data[Position + 1] << 8);
    Position += 2;
    return true;
}

bool CDataInputStream::Read32(fable_u32& value)
{
    if (Position + 4 > Size)
        return false;
    value = static_cast<fable_u32>(Data[Position]) |
        (static_cast<fable_u32>(Data[Position + 1]) << 8) |
        (static_cast<fable_u32>(Data[Position + 2]) << 16) |
        (static_cast<fable_u32>(Data[Position + 3]) << 24);
    Position += 4;
    return true;
}

bool CDataInputStream::ReadBytes(void* destination, unsigned long size)
{
    if (Position + size > Size)
        return false;
    fable_u8* output = static_cast<fable_u8*>(destination);
    for (unsigned long index = 0; index < size; ++index)
        output[index] = Data[Position + index];
    Position += size;
    return true;
}

static fable_i32 ResolvePaletteTextureIndex(
    CEngineTexturePalette& palette,
    fable_u32 serializedIndex)
{
    if (palette.DirectMode != 0)
        return static_cast<fable_i32>(serializedIndex);
    return static_cast<fable_i32>(palette.Entries[serializedIndex].TextureId);
}

bool CLandscapeLayerMesh::LoadForeground(
    CDataInputStream& stream,
    CEngineTexturePalette& texturePalette)
{
    fable_u16 value16 = 0;
    fable_u32 value32 = 0;
    fable_u8 value8 = 0;

    if (!stream.Read16(value16)) return false;
    VertexCount = value16;
    if (!stream.Read16(value16)) return false;
    PolyCountIncDegenerates = value16;
    if (!stream.Read8(value8)) return false;
    MappingDirection = value8;

    if (!stream.Read32(value32)) return false;
    ForegroundTextureIndex = ResolvePaletteTextureIndex(texturePalette, value32);
    if (!stream.Read32(value32)) return false;
    BackgroundTextureIndex = ResolvePaletteTextureIndex(texturePalette, value32);
    if (!stream.Read32(value32)) return false;
    BumpMapTextureIndex = ResolvePaletteTextureIndex(texturePalette, value32);

    if (!stream.Read8(value8)) return false;
    UsesSharedIndexBuffer = value8 != 0;
    if (!stream.Read32(value32)) return false;
    MinTextureMipMap = static_cast<fable_i32>(value32);
    if (!stream.Read32(value32)) return false;
    MinBumpMapMipMap = static_cast<fable_i32>(value32);
    if (!stream.Read32(value32)) return false;
    *reinterpret_cast<fable_u32*>(&SelfIllumination) = value32;

    ForegroundVertexBuffer = AllocateLandscapeForegroundVertexBuffer(this, VertexCount, 0x18);
    if (ForegroundVertexBuffer == 0)
        return false;

    CLandscapeForegroundVertex* vertex = static_cast<CLandscapeForegroundVertex*>(
        LockLandscapeVertexBuffer(ForegroundVertexBuffer));
    if (vertex == 0)
        return false;

    for (unsigned int index = 0; index < VertexCount; ++index, ++vertex)
    {
        if (!stream.Read16(vertex->Value0) || !stream.Read16(vertex->Value1) ||
            !stream.Read32(vertex->NormalBase))
        {
            UnlockLandscapeVertexBuffer(ForegroundVertexBuffer);
            return false;
        }
        if (!stream.Read32(value32))
        {
            UnlockLandscapeVertexBuffer(ForegroundVertexBuffer);
            return false;
        }
        SetLandscapeTargetVertexNormal(vertex, value32);
        if (!stream.Read8(vertex->Byte16) || !stream.Read8(vertex->Byte15) ||
            !stream.Read8(vertex->Byte14))
        {
            UnlockLandscapeVertexBuffer(ForegroundVertexBuffer);
            return false;
        }
    }
    UnlockLandscapeVertexBuffer(ForegroundVertexBuffer);

    ForegroundTexture = ResolveLandscapeTextureFrame(ForegroundTextureIndex);
    BackgroundTexture = ResolveLandscapeTextureFrame(BackgroundTextureIndex);
    BumpMap = ResolveLandscapeTextureFrame(BumpMapTextureIndex);

    if (UsesSharedIndexBuffer)
    {
        IndexBuffer = GetSharedLandscapeIndexBuffer();
        return true;
    }

    unsigned long indexCount = static_cast<unsigned long>(PolyCountIncDegenerates) + 2;
    unsigned long indexBytes = indexCount * 2;
    IndexBuffer = AllocateLandscapeForegroundIndexBuffer(this, indexBytes, 0x65, 5);
    if (IndexBuffer == 0)
        return false;
    void* indices = LockLandscapeIndexBuffer(IndexBuffer);
    if (indices == 0)
        return false;
    bool read = stream.ReadBytes(indices, indexBytes);
    UnlockLandscapeIndexBuffer(IndexBuffer);
    return read;
}
