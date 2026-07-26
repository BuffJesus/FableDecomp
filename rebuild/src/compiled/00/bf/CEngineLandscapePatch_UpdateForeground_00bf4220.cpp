#include "candidates/landscape_patch_update_foreground_compiled.h"

static __forceinline float ClampForegroundCoordinate(float value, float minimum, float maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}

LS_LOADING_STATUS CEngineLandscapePatch::UpdateForeground(
    CCountedPointer<CBankFileAsyncEntry> bankEntry)
{
    if ((Flags & 0x10) != 0)
    {
        const float cameraX = *reinterpret_cast<float*>(DAT_01436ea0 + 0x54);
        const float cameraY = *reinterpret_cast<float*>(DAT_01436ea0 + 0x58);
        const float cameraZ = *reinterpret_cast<float*>(DAT_01436ea0 + 0x5C);

        float nearestX = ClampForegroundCoordinate(cameraX, BoundsMinX, BoundsMaxX);
        float nearestY = ClampForegroundCoordinate(cameraY, BoundsMinY, BoundsMaxY);
        float nearestZ = ClampForegroundCoordinate(cameraZ, BoundsMinZ, BoundsMaxZ);
        float deltaX = cameraX - nearestX;
        float deltaY = cameraY - nearestY;
        float deltaZ = cameraZ - nearestZ;
        float distanceSquared = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;

        float discardDistance = *reinterpret_cast<float*>(DAT_01436ea8 + 0x618);
        if (discardDistance * discardDistance <= distanceSquared)
        {
            if ((Flags & 0x08) != 0)
            {
                if (ForegroundDataSize > 0)
                    DeleteLayerMeshes();
                if (WaterPatch != 0)
                    WaterPatch->ClearTextures();
                AsyncData.DiscardAsyncData();
                Flags = static_cast<fable_u8>(Flags & 0xF2);
            }
        }
        else
        {
            fable_u8 oldFlags = Flags;
            Flags = static_cast<fable_u8>(oldFlags | 0x08);
            if ((oldFlags & 0x04) != 0)
            {
                if ((oldFlags & 0x02) != 0)
                    LoadTextures(distanceSquared);
            }
            else
            {
                if ((oldFlags & 0x02) == 0 && ForegroundDataSize > 0)
                {
                    LS_LOADING_STATUS loadStatus = LoadForegroundPatch(bankEntry);
                    if (loadStatus == LS_LOADING_FAILED)
                    {
                        DeleteLayerMeshes();
                        if (WaterPatch != 0)
                            WaterPatch->ClearTextures();
                        return LS_LOADING_FAILED;
                    }
                    if (loadStatus == LS_LOADING_PENDING)
                        return LS_LOADING_PENDING;
                }

                if ((Flags & 0x01) == 0)
                {
                    LoadTextures(distanceSquared);
                    if ((Flags & 0x01) == 0)
                        return LS_LOADING_READY;
                }

                if (!LandscapeMap->IsBackgroundAtFullDetailInArea(
                        PatchX, PatchY, 0x10, 0x10))
                    return LS_LOADING_READY;
                Flags = static_cast<fable_u8>(Flags | 0x04);
            }
        }
    }
    return LS_LOADING_READY;
}
