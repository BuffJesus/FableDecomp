#include <new>
#include "candidates/landscape_background_patch_load_compiled.h"

CDataInputStream::CDataInputStream(const void* data, unsigned long size)
    : Data(static_cast<const fable_u8*>(data)), Size(size), Position(0)
{
}

bool CDataInputStream::Read8(fable_u8& value)
{
    if (Position + 1 > Size) return false;
    value = Data[Position++];
    return true;
}

bool CDataInputStream::Read16(fable_u16& value)
{
    if (Position + 2 > Size) return false;
    value = static_cast<fable_u16>(Data[Position]) |
        static_cast<fable_u16>(Data[Position + 1] << 8);
    Position += 2;
    return true;
}

bool CDataInputStream::Read32(fable_u32& value)
{
    if (Position + 4 > Size) return false;
    value = static_cast<fable_u32>(Data[Position]) |
        (static_cast<fable_u32>(Data[Position + 1]) << 8) |
        (static_cast<fable_u32>(Data[Position + 2]) << 16) |
        (static_cast<fable_u32>(Data[Position + 3]) << 24);
    Position += 4;
    return true;
}

bool CDataInputStream::ReadFloat(float& value)
{
    fable_u32 bits = 0;
    if (!Read32(bits)) return false;
    *reinterpret_cast<fable_u32*>(&value) = bits;
    return true;
}

bool CDataInputStream::ReadBytes(void* destination, unsigned long size)
{
    if (Position + size > Size) return false;
    fable_u8* output = static_cast<fable_u8*>(destination);
    for (unsigned long index = 0; index < size; ++index)
        output[index] = Data[Position + index];
    Position += size;
    return true;
}

bool CLandscapeBackgroundPatch::Load(CDataInputStream& input, float minZ, float maxZ)
{
    MinZ = minZ;
    MaxZ = maxZ;

    fable_u8 value8 = 0;
    if (!input.Read16(Width) || !input.Read16(Height) ||
        !input.Read16(MapX) || !input.Read16(MapY)) return false;
    if (!input.Read8(value8)) return false;
    WaterOnlyPatch = value8 != 0;
    if (!input.Read8(LODBand)) return false;

    if (!WaterOnlyPatch)
    {
        if (!input.Read16(PolyCountIncDegenerates) || !input.Read16(VertexCount)) return false;
        if (!input.Read8(value8)) return false;
        TextureWidthNeeded = value8;
        if (!input.Read8(value8)) return false;
        TextureHeightNeeded = value8;
        if (!input.Read8(value8)) return false;
        bool dxt1 = value8 != 0;

        Texture = AllocateLandscapeBackgroundTexture(
            this, TextureWidthNeeded, TextureHeightNeeded, dxt1);
        if (Texture == 0 || !LoadLandscapeBackgroundTexture(Texture, input))
            return false;

        VertexBuffer = AllocateLandscapeBackgroundVertexBuffer(this, VertexCount, 0x18);
        if (VertexBuffer == 0)
            return false;

        fable_u32 compressedVertexBytes = 0;
        if (!input.Read32(compressedVertexBytes)) return false;
        fable_u8* compressedVertices = static_cast<fable_u8*>(
            operator new(compressedVertexBytes == 0 ? 1 : compressedVertexBytes));
        if (compressedVertices == 0) return false;
        if (!input.ReadBytes(compressedVertices, compressedVertexBytes))
        {
            operator delete(compressedVertices);
            return false;
        }
        unsigned long vertexPayloadBytes = static_cast<unsigned long>(VertexCount) * 0x10;
        fable_u8* vertexPayload = static_cast<fable_u8*>(
            operator new(vertexPayloadBytes == 0 ? 1 : vertexPayloadBytes));
        if (vertexPayload == 0)
        {
            operator delete(compressedVertices);
            return false;
        }
        bool verticesReady = DecompressLandscapeRange(
            compressedVertices, compressedVertexBytes, VertexCount, 0x10, vertexPayload);
        operator delete(compressedVertices);
        if (!verticesReady)
        {
            operator delete(vertexPayload);
            return false;
        }

        CDataInputStream vertexStream(vertexPayload, vertexPayloadBytes);
        CBackgroundLandscapeVertex* vertices = static_cast<CBackgroundLandscapeVertex*>(
            LockLandscapeBackgroundVertexBuffer(VertexBuffer));
        if (vertices == 0)
        {
            operator delete(vertexPayload);
            return false;
        }
        for (unsigned int index = 0; index < VertexCount; ++index)
        {
            fable_u32 targetNormal = 0;
            fable_u16 byteValue = 0;
            if (!vertexStream.Read16(vertices[index].X) ||
                !vertexStream.Read16(vertices[index].Y) ||
                !vertexStream.Read32(vertices[index].Z) ||
                !vertexStream.Read32(targetNormal) ||
                !vertexStream.Read16(byteValue))
            {
                UnlockLandscapeBackgroundVertexBuffer(VertexBuffer);
                operator delete(vertexPayload);
                return false;
            }
            SetLandscapeBackgroundTargetNormal(vertices + index, targetNormal);
            vertices[index].Byte12 = static_cast<fable_u8>(byteValue);
            if (!vertexStream.Read16(byteValue))
            {
                UnlockLandscapeBackgroundVertexBuffer(VertexBuffer);
                operator delete(vertexPayload);
                return false;
            }
            vertices[index].Byte11 = static_cast<fable_u8>(byteValue);
        }
        UnlockLandscapeBackgroundVertexBuffer(VertexBuffer);
        operator delete(vertexPayload);

        SharedIndexBuffer = LODBand == 1;
        if (SharedIndexBuffer)
        {
            IndexBuffer = GetLandscapeBackgroundSharedIndexBuffer(Width, Height);
        }
        else
        {
            unsigned long indexBytes = static_cast<unsigned long>(PolyCountIncDegenerates) * 6;
            IndexBuffer = AllocateLandscapeBackgroundIndexBuffer(this, indexBytes, 0x65, 4);
            if (IndexBuffer == 0)
                return false;
            fable_u32 compressedIndexBytes = 0;
            if (!input.Read32(compressedIndexBytes)) return false;
            fable_u8* compressedIndices = static_cast<fable_u8*>(
                operator new(compressedIndexBytes == 0 ? 1 : compressedIndexBytes));
            if (compressedIndices == 0) return false;
            if (!input.ReadBytes(compressedIndices, compressedIndexBytes))
            {
                operator delete(compressedIndices);
                return false;
            }
            void* indices = LockLandscapeBackgroundIndexBuffer(IndexBuffer);
            if (indices == 0)
            {
                operator delete(compressedIndices);
                return false;
            }
            bool indicesReady = DecompressLandscapeRange(
                compressedIndices,
                compressedIndexBytes,
                static_cast<unsigned long>(PolyCountIncDegenerates) * 3,
                2,
                indices);
            operator delete(compressedIndices);
            UnlockLandscapeBackgroundIndexBuffer(IndexBuffer);
            if (!indicesReady)
                return false;
        }
    }

    for (int edge = 0; edge < 4; ++edge)
    {
        if (!EdgeStrips[edge].Load(input, this))
            return false;
    }

    fable_u8 hasWater = 0;
    if (!input.Read8(hasWater)) return false;
    if (hasWater != 0)
    {
        CEngineLandscapeMap* map = Parent->LandscapeMap;
        WaterSubPatch = CreateLandscapeWaterBackgroundSubPatch(map);
        if (WaterSubPatch == 0)
            return false;
        CEngineLandscapeRuntime* runtime = map->Runtime;
        if (!LoadLandscapeWaterBackgroundSubPatch(
                WaterSubPatch,
                input,
                Width,
                Height,
                static_cast<unsigned long>(runtime->BaseMapX) + MapX,
                static_cast<unsigned long>(runtime->BaseMapY) + MapY,
                MinZ,
                MaxZ))
            return false;
    }
    return true;
}
