#include <new>
#include "candidates/landscape_map_update_foreground_compiled.h"

static __forceinline float ClampForegroundMapCoordinate(float value, float minimum, float maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}

static CEngineLandscapePatch* AllocateForegroundPatchArray(int count)
{
    unsigned long bytes = sizeof(int) + static_cast<unsigned long>(count) * sizeof(CEngineLandscapePatch);
    int* allocation = static_cast<int*>(operator new(bytes));
    if (allocation == 0)
        return 0;

    *allocation = count;
    CEngineLandscapePatch* patches = reinterpret_cast<CEngineLandscapePatch*>(allocation + 1);
    for (int index = 0; index < count; ++index)
        new (patches + index) CEngineLandscapePatch;
    return patches;
}

static void DestroyForegroundPatchArray(CEngineLandscapePatch* patches)
{
    int* allocation = reinterpret_cast<int*>(patches) - 1;
    int count = *allocation;
    for (int index = count; index > 0; --index)
        patches[index - 1].~CEngineLandscapePatch();
    operator delete(allocation);
}

static __forceinline LS_LOADING_STATUS FinishForegroundUpdate(
    CEngineLandscapeMap* map,
    LS_LOADING_STATUS status)
{
    map->UpdateForegroundActive = false;
    return status;
}

LS_LOADING_STATUS CEngineLandscapeMap::UpdateForeground()
{
    UpdateForegroundActive = true;

    bool foregroundNear = false;
    if (BackgroundTree != 0)
    {
        const float cameraX = *reinterpret_cast<float*>(DAT_01436ea0 + 0x54);
        const float cameraY = *reinterpret_cast<float*>(DAT_01436ea0 + 0x58);
        const float cameraZ = *reinterpret_cast<float*>(DAT_01436ea0 + 0x5C);
        const float nearestX = ClampForegroundMapCoordinate(cameraX, BackgroundTree->MinX, BackgroundTree->MaxX);
        const float nearestY = ClampForegroundMapCoordinate(cameraY, BackgroundTree->MinY, BackgroundTree->MaxY);
        const float nearestZ = ClampForegroundMapCoordinate(cameraZ, BackgroundTree->MinZ, BackgroundTree->MaxZ);
        const float deltaX = cameraX - nearestX;
        const float deltaY = cameraY - nearestY;
        const float deltaZ = cameraZ - nearestZ;
        const float distanceSquared = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
        const float range = *reinterpret_cast<float*>(DAT_01436ea8 + 0x618);
        foregroundNear = distanceSquared < range * range;
    }

    if (foregroundNear)
    {
        if (ForegroundPatches == 0 && BankEntry != 0)
        {
            if (ForegroundAsync.Pointer == 0)
            {
                if (ForegroundBlockOffset != 0)
                {
                    CMapCountedPointer<CBankFileAsyncData> read =
                        BankEntry->BeginReadDataAutoAlloc(
                            ForegroundBlockOffset,
                            static_cast<unsigned long>(PatchRows * PatchColumns * 0x24));
                    ForegroundAsync = read;
                    return FinishForegroundUpdate(this, LS_LOADING_PENDING);
                }
                return FinishForegroundUpdate(this, LS_LOADING_READY);
            }

            if (!ForegroundAsync.Pointer->IsFinished())
                return FinishForegroundUpdate(this, LS_LOADING_PENDING);

            void* data = ForegroundAsync.Pointer->GetData();
            unsigned long dataSize = ForegroundAsync.Pointer->GetDataSize();
            CMemoryDataInputStream input(data, dataSize);

            if (PatchRows < 0 || PatchColumns < 0)
                return FinishForegroundUpdate(this, LS_LOADING_FAILED);

            int patchCount = PatchRows * PatchColumns;
            if (PatchRows != 0 && patchCount / PatchRows != PatchColumns)
                return FinishForegroundUpdate(this, LS_LOADING_FAILED);

            ForegroundPatches = AllocateForegroundPatchArray(patchCount);
            if (ForegroundPatches == 0)
                return FinishForegroundUpdate(this, LS_LOADING_FAILED);

            for (int row = 0; row < PatchRows; ++row)
            {
                for (int column = 0; column < PatchColumns; ++column)
                {
                    CEngineLandscapePatch* patch = ForegroundPatches + PatchColumns * row + column;
                    patch->Initialise(this, column << 4, row << 4);
                    patch->LoadHeader(input);
                }
            }
            ForegroundAsync.Clear();
        }

        if (ForegroundPatches != 0)
        {
            LS_LOADING_STATUS result = LS_LOADING_READY;
            const int patchCount = PatchRows * PatchColumns;
            for (int index = 0; index < patchCount; ++index)
            {
                CMapCountedPointer<CBankFileAsyncEntry> bankEntry(BankEntry, BankEntryReference);
                LS_LOADING_STATUS patchStatus = ForegroundPatches[index].UpdateForeground(bankEntry);
                if (patchStatus == LS_LOADING_FAILED)
                    return FinishForegroundUpdate(this, LS_LOADING_FAILED);
                if (patchStatus == LS_LOADING_PENDING)
                    result = LS_LOADING_PENDING;
            }
            return FinishForegroundUpdate(this, result);
        }

        return FinishForegroundUpdate(this, LS_LOADING_READY);
    }

    if (ForegroundAsync.Pointer != 0)
    {
        ForegroundAsync.Pointer->DiscardAsyncData();
        ForegroundAsync.Clear();
    }

    if (ForegroundPatches != 0)
    {
        if (!KeepForegroundLoaded)
        {
            DestroyForegroundPatchArray(ForegroundPatches);
            ForegroundPatches = 0;
        }
        else
        {
            const int patchCount = PatchRows * PatchColumns;
            for (int index = 0; index < patchCount; ++index)
            {
                CMapCountedPointer<CBankFileAsyncEntry> bankEntry(BankEntry, BankEntryReference);
                ForegroundPatches[index].UpdateForeground(bankEntry);
            }
        }
    }

    return FinishForegroundUpdate(this, LS_LOADING_READY);
}
