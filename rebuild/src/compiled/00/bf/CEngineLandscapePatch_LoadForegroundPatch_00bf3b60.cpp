#include <new>
#include "candidates/landscape_patch_load_foreground_compiled.h"

CMemoryDataInputStream::CMemoryDataInputStream(const void* data, unsigned long size)
    : Data(static_cast<const fable_u8*>(data)), Size(size), Position(0)
{
}

bool CMemoryDataInputStream::Read8(fable_u8& value)
{
    if (Position + 1 > Size)
        return false;
    value = Data[Position++];
    return true;
}

bool CMemoryDataInputStream::Read16(fable_u16& value)
{
    if (Position + 2 > Size)
        return false;
    value = static_cast<fable_u16>(Data[Position]) |
        static_cast<fable_u16>(Data[Position + 1] << 8);
    Position += 2;
    return true;
}

bool CMemoryDataInputStream::Read32(fable_u32& value)
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

LS_LOADING_STATUS CEngineLandscapePatch::LoadForegroundPatch(
    CForegroundCountedPointer<CBankFileAsyncEntry> bankEntry)
{
    if (LayerMeshList != 0)
        return LS_LOADING_READY;

    if (LoadingPatch.Pointer == 0)
    {
        if (bankEntry.Pointer == 0 || ForegroundDataSize < 0)
            return LS_LOADING_FAILED;
        CForegroundCountedPointer<CBankFileAsyncData> read =
            bankEntry.Pointer->BeginReadDataAutoAlloc(
                FilePosition,
                static_cast<unsigned long>(ForegroundDataSize));
        LoadingPatch = read;
    }

    if (LoadingPatch.Pointer == 0)
        return LS_LOADING_FAILED;
    if (!LoadingPatch.Pointer->IsFinished())
        return LS_LOADING_PENDING;

    const fable_u8* compressedData = LoadingPatch.Pointer->GetData();
    CMemoryDataInputStream compressedStream(
        compressedData,
        static_cast<unsigned long>(ForegroundDataSize));
    fable_u32 decompressedSize = 0;
    fable_u32 compressedSize = 0;
    if (!compressedStream.Read32(decompressedSize) || !compressedStream.Read32(compressedSize))
        return LS_LOADING_FAILED;
    if (compressedSize > compressedStream.Size - compressedStream.Position)
        return LS_LOADING_FAILED;

    fable_u8* decompressed = static_cast<fable_u8*>(operator new(decompressedSize + 3));
    if (decompressed == 0)
        return LS_LOADING_FAILED;
    unsigned long outputSize = decompressedSize + 3;
    bool decompressedOk = DecompressLandscapeForeground(
        compressedStream.Data + compressedStream.Position,
        compressedSize,
        decompressed,
        &outputSize);
    if (!decompressedOk)
    {
        operator delete(decompressed);
        return LS_LOADING_FAILED;
    }

    CMemoryDataInputStream foregroundStream(decompressed, decompressedSize);
    fable_u16 layerCount = 0;
    if (!foregroundStream.Read16(layerCount))
    {
        operator delete(decompressed);
        return LS_LOADING_FAILED;
    }

    FastAccessTextureIDCount = 0;
    CLandscapeLayerMesh* previous = 0;
    for (unsigned int index = 0; index < layerCount; ++index)
    {
        CLandscapeLayerMesh* layer = CreateLandscapeLayerMesh(Map);
        if (layer == 0)
        {
            operator delete(decompressed);
            return LS_LOADING_FAILED;
        }
        layer->Next = 0;
        if (!layer->LoadForeground(foregroundStream, Map->TexturePalette))
        {
            DestroyLandscapeLayerMesh(layer);
            operator delete(decompressed);
            return LS_LOADING_FAILED;
        }
        if (layer->ForegroundTextureIndex > 0)
            ++FastAccessTextureIDCount;
        if (layer->BumpMapTextureIndex > 0)
            ++FastAccessTextureIDCount;
        if (previous == 0)
            LayerMeshList = layer;
        else
            previous->Next = layer;
        previous = layer;
    }

    FastAccessTextureIDs = static_cast<fable_u16*>(
        AllocateLandscapePatchTextureIDs(
            static_cast<unsigned long>(FastAccessTextureIDCount) << 2,
            this));
    if (FastAccessTextureIDs == 0)
    {
        operator delete(decompressed);
        return LS_LOADING_FAILED;
    }

    unsigned int textureIndex = 0;
    for (CLandscapeLayerMesh* layer = LayerMeshList; layer != 0; layer = layer->Next)
    {
        if (layer->ForegroundTextureIndex > 0)
            FastAccessTextureIDs[textureIndex++] = static_cast<fable_u16>(layer->ForegroundTextureIndex);
        if (layer->BumpMapTextureIndex > 0)
            FastAccessTextureIDs[textureIndex++] = static_cast<fable_u16>(layer->BumpMapTextureIndex);
    }

    fable_u8 hasWater = 0;
    if (!foregroundStream.Read8(hasWater))
    {
        operator delete(decompressed);
        return LS_LOADING_FAILED;
    }
    if (hasWater != 0)
    {
        if (WaterPatch != 0)
        {
            DestroyWaterPatchMesh(WaterPatch);
            WaterPatch = 0;
        }
        fable_u32 waterX = 0;
        fable_u32 waterY = 0;
        if (!foregroundStream.Read32(waterX) || !foregroundStream.Read32(waterY))
        {
            operator delete(decompressed);
            return LS_LOADING_FAILED;
        }
        WaterPatch = CreateWaterPatchMesh(Map, foregroundStream, waterX, waterY);
        if (WaterPatch == 0)
        {
            operator delete(decompressed);
            return LS_LOADING_FAILED;
        }
    }

    LoadingPatch.Clear();
    Flags = static_cast<fable_u8>(Flags | 0x02);
    operator delete(decompressed);
    return LS_LOADING_READY;
}
