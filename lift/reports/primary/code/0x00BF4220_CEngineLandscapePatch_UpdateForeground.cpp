LS_LOADING_STATUS __thiscall CEngineLandscapePatch::UpdateForeground(
    CCountedPointer<CBankFileAsyncEntry> bankEntry,
    int* param_3)
{
    // Exact field mapping used by this reconstruction:
    //   0x04 = m_pLandscapeMap
    //   0x14 = m_pBackgroundPatch
    //   0x1c = m_LayerMeshCount
    //   0x20 = m_BoundsMinX
    //   0x24 = m_BoundsMinY
    //   0x28 = m_BoundsMinZ
    //   0x2c = m_BoundsMaxX
    //   0x30 = m_BoundsMaxY
    //   0x34 = m_BoundsMaxZ
    //   0x38 = m_PatchX
    //   0x3a = m_PatchY
    //   0x3c = m_Flags
    //   0x40 = m_AsyncData
    //
    // Bounding-box SIMD layout in the packed branch matches the binary exactly:
    //   min pack = (m_BoundsMinZ, 0.0f, m_BoundsMinX, m_BoundsMinY)
    //   max pack = (m_BoundsMaxZ, 0.0f, m_BoundsMaxX, m_BoundsMaxY)
    // extraction order after clamp is z, x, y.

    LS_LOADING_STATUS result = static_cast<LS_LOADING_STATUS>(1);

    if ((this->m_Flags & 0x10) != 0)
    {
        const float cameraX = *reinterpret_cast<const float*>(DAT_01436ea0 + 0x54);
        const float cameraY = *reinterpret_cast<const float*>(DAT_01436ea0 + 0x58);
        float distanceSquared;

        if (DAT_013d2880 == '\0')
        {
            float nearestX = cameraX;
            float nearestY = cameraY;
            float nearestZ = *reinterpret_cast<const float*>(DAT_01436ea0 + 0x5C);

            if (this->m_BoundsMinX <= nearestX)
            {
                if (this->m_BoundsMaxX < nearestX)
                {
                    nearestX = this->m_BoundsMaxX;
                }
            }
            else
            {
                nearestX = this->m_BoundsMinX;
            }

            if (this->m_BoundsMinY <= nearestY)
            {
                if (this->m_BoundsMaxY < nearestY)
                {
                    nearestY = this->m_BoundsMaxY;
                }
            }
            else
            {
                nearestY = this->m_BoundsMinY;
            }

            if (this->m_BoundsMinZ <= nearestZ)
            {
                if (this->m_BoundsMaxZ < nearestZ)
                {
                    nearestZ = this->m_BoundsMaxZ;
                }
            }
            else
            {
                nearestZ = this->m_BoundsMinZ;
            }

            nearestY = *reinterpret_cast<const float*>(DAT_01436ea0 + 0x58) - nearestY;
            nearestZ = *reinterpret_cast<const float*>(DAT_01436ea0 + 0x5C) - nearestZ;
            distanceSquared =
                nearestY * nearestY +
                (*reinterpret_cast<const float*>(DAT_01436ea0 + 0x54) - nearestX) *
                    (*reinterpret_cast<const float*>(DAT_01436ea0 + 0x54) - nearestX) +
                nearestZ * nearestZ;
        }
        else
        {
            float clampedZ = this->m_BoundsMinZ;
            float clampedX = this->m_BoundsMinX;
            float clampedY = this->m_BoundsMinY;

            if (clampedZ < *reinterpret_cast<const float*>(DAT_01436ea0 + 0x5C))
            {
                clampedZ = *reinterpret_cast<const float*>(DAT_01436ea0 + 0x5C);
            }
            if (clampedX < *reinterpret_cast<const float*>(DAT_01436ea0 + 0x54))
            {
                clampedX = *reinterpret_cast<const float*>(DAT_01436ea0 + 0x54);
            }
            if (clampedY < *reinterpret_cast<const float*>(DAT_01436ea0 + 0x58))
            {
                clampedY = *reinterpret_cast<const float*>(DAT_01436ea0 + 0x58);
            }

            if (this->m_BoundsMaxZ < clampedZ)
            {
                clampedZ = this->m_BoundsMaxZ;
            }
            if (this->m_BoundsMaxX < clampedX)
            {
                clampedX = this->m_BoundsMaxX;
            }
            if (this->m_BoundsMaxY < clampedY)
            {
                clampedY = this->m_BoundsMaxY;
            }

            const float deltaZ = clampedZ - *reinterpret_cast<const float*>(DAT_01436ea0 + 0x5C);
            const float deltaX = clampedX - *reinterpret_cast<const float*>(DAT_01436ea0 + 0x54);
            const float deltaY = clampedY - *reinterpret_cast<const float*>(DAT_01436ea0 + 0x58);
            distanceSquared = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
        }

        if ((*reinterpret_cast<const float*>(DAT_01436ea8 + 0x618) *
             *reinterpret_cast<const float*>(DAT_01436ea8 + 0x618)) <= distanceSquared)
        {
            if (((this->m_Flags >> 3) & 1) != 0)
            {
                if (this->m_LayerMeshCount > 0)
                {
                    DeleteLayerMeshes();
                }

                if (this->m_pBackgroundPatch != nullptr)
                {
                    this->m_pBackgroundPatch->ClearTextures();
                }

                CBankFileAsyncData::DiscardAsyncData(&this->m_AsyncData);
                this->m_Flags &= 0xF2;
            }
        }
        else
        {
            const unsigned char oldFlags = this->m_Flags;
            this->m_Flags = static_cast<unsigned char>(oldFlags | 0x08);

            if ((oldFlags & 0x04) != 0)
            {
                if ((oldFlags & 0x02) != 0)
                {
                    LoadTextures(distanceSquared);
                }
            }
            else
            {
                if ((oldFlags & 0x02) == 0)
                {
                    if (this->m_LayerMeshCount > 0)
                    {
                        CCountedPointer<CBankFileAsyncEntry> foregroundEntry(bankEntry);
                        const LS_LOADING_STATUS loadStatus = LoadForegroundPatch();

                        if (loadStatus == static_cast<LS_LOADING_STATUS>(3))
                        {
                            DeleteLayerMeshes();

                            if (this->m_pBackgroundPatch != nullptr)
                            {
                                this->m_pBackgroundPatch->ClearTextures();
                            }

                            foregroundEntry.~CCountedPointer<CBankFileAsyncEntry>();
                            result = static_cast<LS_LOADING_STATUS>(3);
                            goto epilogue;
                        }

                        if (loadStatus == static_cast<LS_LOADING_STATUS>(2))
                        {
                            foregroundEntry.~CCountedPointer<CBankFileAsyncEntry>();
                            result = static_cast<LS_LOADING_STATUS>(2);
                            goto epilogue;
                        }

                        foregroundEntry.~CCountedPointer<CBankFileAsyncEntry>();
                    }
                }

                if ((this->m_Flags & 0x01) == 0)
                {
                    LoadTextures(distanceSquared);

                    if ((this->m_Flags & 0x01) == 0)
                    {
                        goto epilogue;
                    }
                }

                const bool backgroundReady = this->m_pLandscapeMap->IsBackgroundAtFullDetailInArea(
                    this->m_PatchX,
                    this->m_PatchY,
                    0x10,
                    0x10);

                if (!backgroundReady)
                {
                    goto epilogue;
                }

                this->m_Flags = static_cast<unsigned char>(this->m_Flags | 0x04);
            }
        }
    }

epilogue:
    if (param_3 != nullptr)
    {
        *param_3 = *param_3 - 1;
        if (*param_3 == 0)
        {
            (reinterpret_cast<void(__thiscall*)(int*)>(param_3[1]))(param_3);
            operator delete(param_3);
        }
    }

    return result;
}