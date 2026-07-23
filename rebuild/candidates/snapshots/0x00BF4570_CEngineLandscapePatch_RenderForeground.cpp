void CEngineLandscapePatch::RenderForeground()
{
    struct ForegroundRenderEntry
    {
        std::byte pad00[0x04];
        std::int32_t m_DiffuseLodIndex;        // 0x04
        std::byte pad08[0x04];
        std::int32_t m_ProjectedLodIndex;      // 0x0C
        float m_GlobalLightScale;              // 0x10
        CGraphicInfoFrame* m_pDiffuseFrame;    // 0x14
        std::byte pad18[0x04];
        CGraphicInfoFrame* m_pProjectedFrame;  // 0x1C
        std::byte pad20[0x08];
        std::int32_t m_ShaderVariantIndex;     // 0x28
        std::byte pad2C[0x08];
        void* m_pStreamBinding;                // 0x34
        void* m_pSamplerBinding;               // 0x38
        ForegroundRenderEntry* m_pNext;        // 0x3C
        std::byte pad40[0x04];
        std::uint16_t m_BaseVertexIndex;       // 0x44
        std::uint16_t m_PrimitiveCount;        // 0x46
    };

    struct CountedAsyncDataRef
    {
        void* m_pObject;   // 0x00
        void* m_pRefData;  // 0x04
    };

    struct PatchOverlay
    {
        std::byte pad00[0x04];
        void* m_pLandscapeMap;                 // 0x04
        ForegroundRenderEntry* m_pForeground;  // 0x08
        std::byte pad0C[0x08];
        void* m_pBackgroundPatch;              // 0x14
        std::byte pad18[0x04];
        std::int32_t m_LayerMeshCount;         // 0x1C
        float m_BoundsMinX;                    // 0x20
        float m_BoundsMinY;                    // 0x24
        float m_BoundsMinZ;                    // 0x28
        float m_BoundsMaxX;                    // 0x2C
        float m_BoundsMaxY;                    // 0x30
        float m_BoundsMaxZ;                    // 0x34
        std::uint16_t m_PatchX;                // 0x38
        std::uint16_t m_PatchY;                // 0x3A
        std::uint8_t m_Flags;                  // 0x3C
        CountedAsyncDataRef m_AsyncData;       // 0x40
    };

    struct ShaderLookupBlock
    {
        float m_Lookup[4];
        float m_Z;
    };

    static_assert(offsetof(ForegroundRenderEntry, m_DiffuseLodIndex) == 0x04);
    static_assert(offsetof(ForegroundRenderEntry, m_ProjectedLodIndex) == 0x0C);
    static_assert(offsetof(ForegroundRenderEntry, m_GlobalLightScale) == 0x10);
    static_assert(offsetof(ForegroundRenderEntry, m_pDiffuseFrame) == 0x14);
    static_assert(offsetof(ForegroundRenderEntry, m_pProjectedFrame) == 0x1C);
    static_assert(offsetof(ForegroundRenderEntry, m_ShaderVariantIndex) == 0x28);
    static_assert(offsetof(ForegroundRenderEntry, m_pStreamBinding) == 0x34);
    static_assert(offsetof(ForegroundRenderEntry, m_pSamplerBinding) == 0x38);
    static_assert(offsetof(ForegroundRenderEntry, m_pNext) == 0x3C);
    static_assert(offsetof(ForegroundRenderEntry, m_BaseVertexIndex) == 0x44);
    static_assert(offsetof(ForegroundRenderEntry, m_PrimitiveCount) == 0x46);
    static_assert(offsetof(PatchOverlay, m_pForeground) == 0x08);
    static_assert(offsetof(PatchOverlay, m_BoundsMinX) == 0x20);
    static_assert(offsetof(PatchOverlay, m_Flags) == 0x3C);
    static_assert(offsetof(PatchOverlay, m_AsyncData) == 0x40);
    static_assert(offsetof(ShaderLookupBlock, m_Z) == 0x10);

    auto& self = *reinterpret_cast<PatchOverlay*>(this);
    CRenderStateManager* const pRenderStateManager = DAT_013cb508;

    auto QueueTrackedIntState =
        [](CRenderManagerCore* const mgr, const std::ptrdiff_t stateOffset, const std::int32_t value) -> void
    {
        auto* const state = reinterpret_cast<std::uint8_t*>(mgr) + stateOffset;
        if (*reinterpret_cast<std::int32_t*>(state + 0x04) != value)
        {
            if ((*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) &
                 *reinterpret_cast<std::uint32_t*>(state + 0x08)) == 0)
            {
                *reinterpret_cast<std::uint32_t*>(state + 0x08) =
                    *reinterpret_cast<std::uint32_t*>(state + 0x08) |
                    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814);
                *reinterpret_cast<void**>(
                    reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                *reinterpret_cast<std::uint32_t*>(
                    reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                    *reinterpret_cast<std::uint32_t*>(state + 0x04);
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
            }
            *reinterpret_cast<std::int32_t*>(state + 0x04) = value;
            if (*(state + 0x10) == 0)
            {
                *(state + 0x10) = 1;
                *reinterpret_cast<void**>(
                    reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
            }
        }
    };

    auto DestroyGraphicFrameData =
        [](CGraphicFrameData* pGraphicFrameData) -> void
    {
        if ((reinterpret_cast<std::uint8_t*>(pGraphicFrameData)[0x1A] & 0x10) != 0)
        {
            CGraphicFrameData::Uninitialise(pGraphicFrameData);
        }

        int* const pOwner = *reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C);
        if (pOwner != nullptr)
        {
            *pOwner = *pOwner - 1;
            if (**reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C) == 0)
            {
                (reinterpret_cast<void(__thiscall*)(int*)>((*reinterpret_cast<void***>(pOwner))[1]))(pOwner);
                operator_delete(*reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C));
            }
        }

        *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x08) = nullptr;
        *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C) = nullptr;
        operator_delete(pGraphicFrameData);
    };

    if ((self.m_Flags & 4) == 0)
    {
        return;
    }

    {
        auto* const pCaptureIndex =
            reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pRenderStateManager) + 0x2808);
        *reinterpret_cast<void**>(
            reinterpret_cast<std::uint8_t*>(pRenderStateManager) + *pCaptureIndex * 8 + 8) =
            reinterpret_cast<std::uint8_t*>(pRenderStateManager) + 0x3A24;
        *pCaptureIndex = *pCaptureIndex + 1;
        *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pRenderStateManager) + 0x2814) =
            *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pRenderStateManager) + 0x2814) << 1;
    }

    const CColorBufferDepthPackParameters savedVertexShaderState = DAT_013bc800;
    const char savedPixelShaderState = DAT_013bc801;

    if (DAT_013bc800 != static_cast<CColorBufferDepthPackParameters>(1))
    {
        CShaderRenderManager::EnableVertexShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
    }
    if (DAT_013bc801 != '\x01')
    {
        CShaderRenderManager::EnablePixelShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
    }

    if (!ClipPatch(this))
    {
        if (savedVertexShaderState != DAT_013bc800)
        {
            if (savedVertexShaderState == static_cast<CColorBufferDepthPackParameters>(0))
            {
                CShaderRenderManager::DisableVertexShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
            }
            else
            {
                CShaderRenderManager::EnableVertexShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
            }
        }

        if (savedPixelShaderState != DAT_013bc801)
        {
            if (savedPixelShaderState != '\0')
            {
                CShaderRenderManager::EnablePixelShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
                CRenderStateManager::RestoreCaptureBlock(DAT_013cb508);
                return;
            }

            CShaderRenderManager::DisablePixelShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
        }

        CRenderStateManager::RestoreCaptureBlock(DAT_013cb508);
        return;
    }

    C3DBoundingBox* const pBounds =
        reinterpret_cast<C3DBoundingBox*>(reinterpret_cast<std::uint8_t*>(this) + 0x20);
    CEnginePrimitiveThemeChannelDesc themeChannelDesc =
        static_cast<CEnginePrimitiveThemeChannelDesc>(0);
    std::uint8_t themeByte0 = 0;
    std::uint8_t themeByte1 = 0;

    CEngineLocalLightArray* const pLocalLights =
        CEngineLightingManager::GetLightsInArea(DAT_01436e9c, pBounds, 5, &themeChannelDesc);
    CEngineLightingManager::SetupShaderPointLights(DAT_01436e9c, pLocalLights);
    CShaderRenderManager::SetUserVertexShaderConstant(
        DAT_01436e14,
        4,
        reinterpret_cast<C4DVector*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x61C));

    {
        std::uint32_t world[12];
        world[7] = 0;
        world[6] = 0;
        world[5] = 0;
        world[3] = 0;
        world[2] = 0;
        world[1] = 0;
        world[8] = 0x3F800000;
        world[4] = 0x3F800000;
        world[0] = 0x3F800000;
        world[9] = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea0) + 0x54);
        world[10] = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea0) + 0x58);
        world[11] = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea0) + 0x5C);
        CShaderRenderManager::SetWorldTransform(DAT_01436e14, reinterpret_cast<CMatrix3x4*>(world));
    }

    std::int32_t projectedTextureStage = 2;
    CShadowRendererSceneData shadowSceneData;
    CShadowRendererSceneData::CShadowRendererSceneData(
        &shadowSceneData,
        reinterpret_cast<CShadowRendererSceneData*>(reinterpret_cast<std::uint8_t*>(DAT_01436e60) + 0x0C));

    std::int32_t projectedPass = 0;
    std::int32_t diffusePass = 0;

    {
        const auto* const pShadowBytes = reinterpret_cast<const std::uint8_t*>(&shadowSceneData);
        const char shadowValid = *reinterpret_cast<const char*>(pShadowBytes + 0x23C);
        const std::int32_t planeCount = *reinterpret_cast<const std::int32_t*>(pShadowBytes + 0x224);
        float shadowX = *reinterpret_cast<const float*>(pShadowBytes + 0x228);
        float shadowY = *reinterpret_cast<const float*>(pShadowBytes + 0x22C);
        const float shadowZIn = *reinterpret_cast<const float*>(pShadowBytes + 0x230);
        const float shadowRadius = *reinterpret_cast<const float*>(pShadowBytes + 0x234);
        const float* pPlanes = reinterpret_cast<const float*>(pShadowBytes + 0x1A0);
        float fVar28;

        if (shadowValid != '\0')
        {
            if (DAT_013d2880 == '\0')
            {
                float nearestX = shadowX;
                float nearestY = shadowY;
                float nearestZ;

                if (self.m_BoundsMinX <= shadowX)
                {
                    if (self.m_BoundsMaxX < shadowX)
                    {
                        nearestX = self.m_BoundsMaxX;
                    }
                }
                else
                {
                    nearestX = self.m_BoundsMinX;
                }

                if (self.m_BoundsMinY <= shadowY)
                {
                    if (self.m_BoundsMaxY < shadowY)
                    {
                        nearestY = self.m_BoundsMaxY;
                    }
                }
                else
                {
                    nearestY = self.m_BoundsMinY;
                }

                if (self.m_BoundsMinZ <= shadowZIn)
                {
                    nearestZ = shadowZIn;
                    if (self.m_BoundsMaxZ < shadowZIn)
                    {
                        nearestZ = self.m_BoundsMaxZ;
                    }
                }
                else
                {
                    nearestZ = self.m_BoundsMinZ;
                }

                fVar28 =
                    (shadowY - nearestY) * (shadowY - nearestY) +
                    (shadowX - nearestX) * (shadowX - nearestX) +
                    (shadowZIn - nearestZ) * (shadowZIn - nearestZ);
            }
            else
            {
                float clampZ = self.m_BoundsMinZ;
                float clampX = self.m_BoundsMinX;
                float clampY = self.m_BoundsMinY;

                if (clampZ < shadowZIn) clampZ = shadowZIn;
                if (clampX < shadowX) clampX = shadowX;
                if (clampY < shadowY) clampY = shadowY;
                if (self.m_BoundsMaxZ < clampZ) clampZ = self.m_BoundsMaxZ;
                if (self.m_BoundsMaxX < clampX) clampX = self.m_BoundsMaxX;
                if (self.m_BoundsMaxY < clampY) clampY = self.m_BoundsMaxY;

                shadowX = clampX - shadowX;
                shadowY = clampY - shadowY;
                const float dz = clampZ - shadowZIn;
                fVar28 = shadowX * shadowX + shadowY * shadowY + dz * dz;
            }

            if (shadowRadius * shadowRadius < fVar28)
            {
                projectedPass = 1;
                diffusePass = 1;
                const std::uint8_t wantShadowState =
                    static_cast<std::uint8_t>(
                        *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x4694) == 1);
                if (wantShadowState != reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xCE])
                {
                    reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xCE] = wantShadowState;
                    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE4) |= 1;
                }
                goto LAB_PASSES_DONE;
            }
        }

        for (std::int32_t i = 0; i < planeCount; ++i)
        {
            const float* const pPlane = pPlanes + i * 4;
            const float x = (pPlane[0] <= _DAT_0122dedc) ? self.m_BoundsMaxX : self.m_BoundsMinX;
            const float y = (pPlane[1] <= _DAT_0122dedc) ? self.m_BoundsMaxY : self.m_BoundsMinY;
            const float z = (pPlane[2] <= _DAT_0122dedc) ? self.m_BoundsMaxZ : self.m_BoundsMinZ;

            if (pPlane[3] < x * pPlane[0] + y * pPlane[1] + z * pPlane[2])
            {
                projectedPass = 1;
                diffusePass = 1;
                const std::uint8_t wantShadowState =
                    static_cast<std::uint8_t>(
                        *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x4694) == 1);
                if (wantShadowState != reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xCE])
                {
                    reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xCE] = wantShadowState;
                    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE4) |= 1;
                }
                goto LAB_PASSES_DONE;
            }
        }

        if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x4694) == 2)
        {
            CEngineLightingManager::SetupShaderSpotLight(DAT_01436e9c);
            diffusePass = 5;
            projectedPass = 5;
        }
        else
        {
            projectedPass = 3;
            diffusePass = (reinterpret_cast<std::uint8_t*>(DAT_01436ea8)[0x63F] != 0) + 3;
        }

        projectedTextureStage = 3;
        {
            std::uint32_t shadowFuncStates[3];
            shadowFuncStates[0] = 1;
            shadowFuncStates[1] = 2;
            shadowFuncStates[2] = 5;
            CEngineShadowRenderer::SetupShadowFuncRenderStates(
                DAT_01436e60,
                2,
                reinterpret_cast<CColorBufferDepthPackParameters*>(shadowFuncStates));
        }
    }

LAB_PASSES_DONE:
    QueueTrackedIntState(DAT_01436e18, projectedTextureStage * 0x14 + 0x2EE4, DAT_01396dc8);
    QueueTrackedIntState(DAT_01436e18, projectedTextureStage * 0x14 + 0x2F84, DAT_01396dc8);

    ShaderLookupBlock shaderLookup;
    shaderLookup.m_Lookup[0] = static_cast<float>(__ftol2(self.m_BoundsMinX * _DAT_01230a08));
    shaderLookup.m_Lookup[1] = static_cast<float>(__ftol2(self.m_BoundsMinY * _DAT_01230a08));
    shaderLookup.m_Lookup[2] = -shaderLookup.m_Lookup[0];
    shaderLookup.m_Lookup[3] = -shaderLookup.m_Lookup[1];
    shaderLookup.m_Z = static_cast<float>(__ftol2(self.m_BoundsMinZ * _DAT_01230a08));

    QueueTrackedIntState(DAT_01436e18, 0x2854, 0);
    QueueTrackedIntState(DAT_01436e18, 0x28CC, 1);
    QueueTrackedIntState(DAT_01436e18, 0x28A4, DAT_01396de8);
    QueueTrackedIntState(DAT_01436e18, 0x28E0, 1);
    QueueTrackedIntState(DAT_01436e18, 0x291C, DAT_01396f78);
    QueueTrackedIntState(DAT_01436e18, 0x2930, DAT_01396f6c);
    QueueTrackedIntState(DAT_01436e18, 0x28B8, 1);

    float currentVariantFloat = -std::numeric_limits<float>::quiet_NaN();
    C3DBoundingBox* currentProjectedMode = nullptr;
    std::int32_t iVar26 = 0;
    float local_2e4 = 0.0f;
    float local_2e0 = 0.0f;
    std::uint32_t local_2dc = 0;
    float lastShaderVariantAsFloat = -std::numeric_limits<float>::quiet_NaN();

    for (ForegroundRenderEntry* pEntry = self.m_pForeground; ; )
    {
        if (pEntry == nullptr)
        {
LAB_CLEANUP:
            if (reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xCE] != 0)
            {
                reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xCE] = 0;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE4) |= 1;
            }

            if ((reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xE0] != 0) ||
                (reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xE1] != 0) ||
                (reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xE2] != 0))
            {
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE0) = 0;
                if (reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xE2] == 0xFF)
                {
                    reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xE0] =
                        reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xE1];
                    reinterpret_cast<std::uint8_t*>(DAT_01436e9c)[0xE2] = 0;
                }
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE4) |= 0x0F;
            }

            if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE8) != 0)
            {
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE8) = 0;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE4) |= 2;
            }

            {
                auto* const pDevice =
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF0);
                (**reinterpret_cast<void(__thiscall***)(void*, int, int, int, int)>(pDevice))[100](
                    pDevice, 0, 0, 0, 0);
                (**reinterpret_cast<void(__thiscall***)(void*, int)>(pDevice))[104](pDevice, 0);
            }

            {
                std::int32_t stage = 0;
                if (0 < *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8))
                {
                    auto* pStageHandle = reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3D00;
                    do
                    {
                        if (*reinterpret_cast<std::int32_t*>(pStageHandle) != 0)
                        {
                            *reinterpret_cast<std::int32_t*>(pStageHandle) = 0;
                            auto* const pDevice =
                                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF0);
                            (**reinterpret_cast<void(__thiscall***)(void*, int, int)>(pDevice))[0x41](pDevice, stage, 0);
                        }
                        ++stage;
                        pStageHandle += 4;
                    } while (stage < *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8));
                }
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8) = 0;
            }

            CEngineLightingManager::DisablePointLights(DAT_01436e9c);
            RenderForegroundWater(nullptr);

            if (savedVertexShaderState != DAT_013bc800)
            {
                if (savedVertexShaderState == static_cast<CColorBufferDepthPackParameters>(0))
                {
                    CShaderRenderManager::DisableVertexShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
                }
                else
                {
                    CShaderRenderManager::EnableVertexShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
                }
            }

            if (savedPixelShaderState != DAT_013bc801)
            {
                if (savedPixelShaderState == '\0')
                {
                    CShaderRenderManager::DisablePixelShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
                }
                else
                {
                    CShaderRenderManager::EnablePixelShaders(reinterpret_cast<CShaderRenderManager*>(&DAT_013bc470));
                }
            }

            CRenderStateManager::RestoreCaptureBlock(DAT_013cb508);
            return;
        }

        {
            const std::int32_t roundedLightScale =
                static_cast<std::int32_t>(std::round(pEntry->m_GlobalLightScale * _DAT_01230a08));
            if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE8) != roundedLightScale)
            {
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE8) = roundedLightScale;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0xE4) |= 2;
            }
        }

        CEngineLightingManager::UpdateGlobalLightingStates(DAT_01436e9c);

        std::uint32_t local_294 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x48);
        std::uint32_t local_290 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x4C);
        std::uint32_t local_28c = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x50);
        std::uint32_t local_288 = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x54);

        std::int32_t iVar22 = diffusePass;
        if (pEntry->m_pProjectedFrame == nullptr)
        {
            iVar22 = projectedPass;
        }

        if (iVar22 != iVar26)
        {
            switch (iVar22)
            {
            case 1:
                reinterpret_cast<void(__thiscall*)(CShaderRenderManager*, void*)>(0x0098812A)(
                    DAT_01436e14,
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x5A0));
                reinterpret_cast<void*(__thiscall*)(void*, void*, int)>(0x00BF5CED)(
                    DAT_01436ea8,
                    *reinterpret_cast<void**>(pLocalLights),
                    0);
                break;

            case 2:
            {
                reinterpret_cast<void(__thiscall*)(CShaderRenderManager*, void*)>(0x00988124)(
                    DAT_01436e14,
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x5A4));
                void* const case2Value =
                    reinterpret_cast<void*(__thiscall*)(void*, void*, int)>(0x00BF5CB2)(
                        DAT_01436ea8,
                        *reinterpret_cast<void**>(pLocalLights),
                        0);
                reinterpret_cast<void(__thiscall*)(CShaderRenderManager*, void*)>(0x00987F74)(
                    DAT_01436e14,
                    case2Value);

                std::uint32_t case2Block[4];
                case2Block[0] = local_294;
                case2Block[1] = local_290;
                case2Block[2] = local_28c;
                case2Block[3] = local_288;
                CShaderRenderManager::SetUserVertexShaderConstant(
                    DAT_01436e14,
                    0,
                    reinterpret_cast<C4DVector*>(case2Block));

                std::uint32_t case2Extra[4];
                case2Extra[0] = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x78);
                case2Extra[1] = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x7C);
                case2Extra[2] = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e9c) + 0x80);
                case2Extra[3] = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 0x3CC);
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 0x3D8) |= 9;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 0x50) = case2Extra[0];
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 0x54) = case2Extra[1];
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 0x58) = case2Extra[2];
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 0x5C) = case2Extra[3];
                break;
            }

            case 3:
                reinterpret_cast<void(__thiscall*)(CShaderRenderManager*, void*)>(0x00988002)(
                    DAT_01436e14,
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x5A0));
                reinterpret_cast<void*(__thiscall*)(void*, void*, int)>(0x00BF5C15)(
                    DAT_01436ea8,
                    *reinterpret_cast<void**>(pLocalLights),
                    0);
                break;

            case 4:
                reinterpret_cast<void(__thiscall*)(CShaderRenderManager*, void*)>(0x009880A9)(
                    DAT_01436e14,
                    reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x568);
                reinterpret_cast<void*(__thiscall*)(void*, void*, int)>(0x00BF5B87)(
                    DAT_01436ea8,
                    *reinterpret_cast<void**>(pLocalLights),
                    0);
                break;

            case 5:
                reinterpret_cast<void(__thiscall*)(CShaderRenderManager*, void*)>(0x0098809B)(
                    DAT_01436e14,
                    reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x568);
                reinterpret_cast<void*(__thiscall*)(void*, void*, int)>(0x00BF5BB9)(
                    DAT_01436ea8,
                    *reinterpret_cast<void**>(pLocalLights),
                    0);
                break;

            default:
                break;
            }

            iVar26 = iVar22;
        }

        iVar26 = pEntry->m_ShaderVariantIndex;
        if (static_cast<float>(iVar26) != lastShaderVariantAsFloat)
        {
            const int* const pTextureSlot =
                reinterpret_cast<const int*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x5BC + iVar26 * 8);
            const std::int32_t textureHandle = (pTextureSlot != nullptr) ? *pTextureSlot : 0;
            lastShaderVariantAsFloat = static_cast<float>(iVar26);

            if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3D00) != textureHandle)
            {
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3D00) = textureHandle;
                auto* const pDevice =
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF0);
                (**reinterpret_cast<void(__thiscall***)(void*, int, int)>(pDevice))[0x41](pDevice, 0, textureHandle);
            }

            if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8) < 1)
            {
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8) = 1;
            }

            const std::int32_t idx0 =
                *reinterpret_cast<const std::int32_t*>(reinterpret_cast<std::uint8_t*>(&DAT_0139c590) + iVar26 * 4);
            const std::int32_t idx1 =
                *reinterpret_cast<const std::int32_t*>(reinterpret_cast<std::uint8_t*>(&DAT_0139c5A4) + iVar26 * 4);

            CShaderRenderManager::SetUserVertexShaderConstant(
                DAT_01436e14,
                2,
                reinterpret_cast<C3DVector*>(reinterpret_cast<std::uint8_t*>(&DAT_0139c5D8) + iVar26 * 0x0C),
                reinterpret_cast<float*>(&shaderLookup)[idx0]);
            CShaderRenderManager::SetUserVertexShaderConstant(
                DAT_01436e14,
                3,
                reinterpret_cast<C3DVector*>(reinterpret_cast<std::uint8_t*>(&DAT_0139c614) + pEntry->m_ShaderVariantIndex * 0x0C),
                reinterpret_cast<float*>(&shaderLookup)[idx1]);
        }

        if (iVar22 == 4)
        {
            const std::int32_t projectedMode = (lastShaderVariantAsFloat != 0.0f) + 1;
            if (reinterpret_cast<C3DBoundingBox*>(static_cast<std::intptr_t>(projectedMode)) != currentProjectedMode)
            {
                local_2e4 = 0.0f;
                if (projectedMode == 1)
                {
                    local_2e0 = 1.0f;
                    local_2dc = 0;
                }
                else
                {
                    local_2e0 = 0.0f;
                    local_2dc = 0xBF800000;
                }
                currentProjectedMode = reinterpret_cast<C3DBoundingBox*>(static_cast<std::intptr_t>(projectedMode));
                CShaderRenderManager::SetUserVertexShaderConstant(
                    DAT_01436e14,
                    1,
                    reinterpret_cast<C3DVector*>(&local_2e4),
                    0.0f);
            }

            CGraphicInfoFrame* const pProjectedInfoFrame = pEntry->m_pProjectedFrame;
            const std::uint8_t projectedLod =
                *reinterpret_cast<std::uint8_t*>(
                    *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x6A4) +
                    pEntry->m_ProjectedLodIndex);
            const std::uint32_t projectedLodU = projectedLod;

            if (*reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x08) == nullptr)
            {
                CFrameData* pNewFrameData = reinterpret_cast<CFrameData*>(::operator_new(0x44));
                std::uint32_t newFrameDataValue;
                if (pNewFrameData == nullptr)
                {
                    newFrameDataValue = 0;
                }
                else
                {
                    newFrameDataValue = reinterpret_cast<std::uint32_t>(
                        CGraphicInfoFrame::CFrameData::CFrameData(pNewFrameData, pProjectedInfoFrame));
                }

                int* const pOld = *reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x08);
                if (pOld != nullptr)
                {
                    int* const pRef = pOld + 1;
                    *pRef = *pRef - 1;
                    if (*pRef == 0)
                    {
                        (reinterpret_cast<void(__thiscall*)(int*)>((*reinterpret_cast<void***>(pOld))[1]))(pOld);
                    }
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x08) = nullptr;
                }

                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x08) = newFrameDataValue;
                CGraphicInfoFrame::CFrameData::BeginLoading(
                    *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x08),
                    projectedLodU,
                    0);
                self.m_Flags = static_cast<std::uint8_t>(self.m_Flags & 0xFB);
                goto LAB_CLEANUP;
            }

            if (*reinterpret_cast<std::int32_t*>(
                    reinterpret_cast<std::uint8_t*>(*reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x08)) + 0x1C) == 0)
            {
                CGraphicInfoFrame::CFrameData::BeginLoading(
                    *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x08),
                    projectedLodU,
                    0);
                self.m_Flags = static_cast<std::uint8_t>(self.m_Flags & 0xFB);
                goto LAB_CLEANUP;
            }

            {
                const std::int32_t priority =
                    *reinterpret_cast<std::int32_t*>(
                        *reinterpret_cast<std::uint8_t**>(
                            *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x0C) + 0x20) + 0x194);
                auto* const pFrameData =
                    *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pProjectedInfoFrame) + 0x08);

                if ((*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3C) != priority) ||
                    (projectedLodU <= static_cast<std::uint32_t>(
                        static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3A)))))
                {
                    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3C) = priority;

                    CResource* pResource =
                        *reinterpret_cast<CResource**>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x24);
                    if ((pResource != nullptr) &&
                        (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) != 0) &&
                        (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) !=
                         *reinterpret_cast<std::int32_t*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C)))
                    {
                        CResource::ListRemove(pResource);
                        CResource::ListInsert(
                            pResource,
                            *reinterpret_cast<CResource**>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x10),
                            reinterpret_cast<CResource*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 4));
                        *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) =
                            *reinterpret_cast<std::int32_t*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C);
                    }

                    pResource = reinterpret_cast<CResource*>(pFrameData);
                    if ((projectedLodU <= static_cast<std::uint32_t>(
                             static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x38)))) &&
                        (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) != 0) &&
                        (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) !=
                         *reinterpret_cast<std::int32_t*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C)))
                    {
                        CResource::ListRemove(pResource);
                        CResource::ListInsert(
                            pResource,
                            *reinterpret_cast<CResource**>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x10),
                            reinterpret_cast<CResource*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 4));
                        *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) =
                            *reinterpret_cast<std::int32_t*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C);
                    }

                    if (projectedLodU < static_cast<std::uint32_t>(
                                            static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3A))))
                    {
                        *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3A) = projectedLod;
                    }
                }

                CGraphicFrameData* pProjectedGraphicFrameData;
                if (projectedLodU < static_cast<std::uint32_t>(
                                        static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x38))))
                {
                    CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, projectedLodU, 0);
                    pProjectedGraphicFrameData =
                        *reinterpret_cast<CGraphicFrameData**>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x1C);
                }
                else
                {
                    pProjectedGraphicFrameData =
                        *reinterpret_cast<CGraphicFrameData**>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x1C);
                }

                if (pProjectedGraphicFrameData != nullptr)
                {
                    *reinterpret_cast<std::int32_t*>(pProjectedGraphicFrameData) =
                        *reinterpret_cast<std::int32_t*>(pProjectedGraphicFrameData) + 1;
                }

                std::int32_t projectedHandle = 0;
                if (*reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pProjectedGraphicFrameData) + 0x08) != nullptr)
                {
                    projectedHandle =
                        **reinterpret_cast<std::int32_t**>(reinterpret_cast<std::uint8_t*>(pProjectedGraphicFrameData) + 0x08);
                }

                if (*reinterpret_cast<std::int32_t*>(
                        reinterpret_cast<std::uint8_t*>(DAT_01436e18) + projectedTextureStage * 4 + 0x3D00) != projectedHandle)
                {
                    *reinterpret_cast<std::int32_t*>(
                        reinterpret_cast<std::uint8_t*>(DAT_01436e18) + projectedTextureStage * 4 + 0x3D00) = projectedHandle;
                    auto* const pDevice =
                        *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF0);
                    (**reinterpret_cast<void(__thiscall***)(void*, int, int)>(pDevice))[0x41](
                        pDevice,
                        projectedTextureStage,
                        projectedHandle);
                }

                if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8) <
                    projectedTextureStage + 1)
                {
                    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8) =
                        projectedTextureStage + 1;
                }

                if (pProjectedGraphicFrameData != nullptr)
                {
                    const std::int32_t projectedRefCount = *reinterpret_cast<std::int32_t*>(pProjectedGraphicFrameData);
                    *reinterpret_cast<std::int32_t*>(pProjectedGraphicFrameData) = projectedRefCount - 1;
                    if (projectedRefCount - 1 == 0)
                    {
                        DestroyGraphicFrameData(pProjectedGraphicFrameData);
                    }
                }
            }
        }

        CGraphicInfoFrame* const pDiffuseInfoFrame = pEntry->m_pDiffuseFrame;
        const std::uint8_t diffuseLod =
            *reinterpret_cast<std::uint8_t*>(
                *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x6A4) +
                pEntry->m_DiffuseLodIndex);
        const std::uint32_t diffuseLodU = diffuseLod;

        if (*reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x08) == nullptr)
        {
            CFrameData* pNewFrameData = reinterpret_cast<CFrameData*>(::operator_new(0x44));
            std::uint32_t newFrameDataValue;
            if (pNewFrameData == nullptr)
            {
                newFrameDataValue = 0;
            }
            else
            {
                newFrameDataValue = reinterpret_cast<std::uint32_t>(
                    CGraphicInfoFrame::CFrameData::CFrameData(pNewFrameData, pDiffuseInfoFrame));
            }

            int* const pOld = *reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x08);
            if (pOld != nullptr)
            {
                int* const pRef = pOld + 1;
                *pRef = *pRef - 1;
                if (*pRef == 0)
                {
                    (reinterpret_cast<void(__thiscall*)(int*)>((*reinterpret_cast<void***>(pOld))[1]))(pOld);
                }
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x08) = nullptr;
            }

            *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x08) = newFrameDataValue;
            CGraphicInfoFrame::CFrameData::BeginLoading(
                *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x08),
                diffuseLodU,
                0);
            self.m_Flags = static_cast<std::uint8_t>(self.m_Flags & 0xFB);
            goto LAB_CLEANUP;
        }

        if (*reinterpret_cast<std::int32_t*>(
                reinterpret_cast<std::uint8_t*>(*reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x08)) + 0x1C) == 0)
        {
            CGraphicInfoFrame::CFrameData::BeginLoading(
                *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x08),
                diffuseLodU,
                0);
            self.m_Flags = static_cast<std::uint8_t>(self.m_Flags & 0xFB);
            goto LAB_CLEANUP;
        }

        const std::int32_t diffusePriority =
            *reinterpret_cast<std::int32_t*>(
                *reinterpret_cast<std::uint8_t**>(
                    *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x0C) + 0x20) + 0x194);
        auto* const pDiffuseFrameData =
            *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pDiffuseInfoFrame) + 0x08);

        if ((*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x3C) != diffusePriority) ||
            (diffuseLodU <= static_cast<std::uint32_t>(
                static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x3A)))))
        {
            *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x3C) = diffusePriority;

            CResource* pResource =
                *reinterpret_cast<CResource**>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x24);
            if ((pResource != nullptr) &&
                (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) != 0) &&
                (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) !=
                 *reinterpret_cast<std::int32_t*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C)))
            {
                CResource::ListRemove(pResource);
                CResource::ListInsert(
                    pResource,
                    *reinterpret_cast<CResource**>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x10),
                    reinterpret_cast<CResource*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 4));
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) =
                    *reinterpret_cast<std::int32_t*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C);
            }

            pResource = reinterpret_cast<CResource*>(pDiffuseFrameData);
            if ((diffuseLodU <= static_cast<std::uint32_t>(
                     static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x38)))) &&
                (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) != 0) &&
                (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) !=
                 *reinterpret_cast<std::int32_t*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C)))
            {
                CResource::ListRemove(pResource);
                CResource::ListInsert(
                    pResource,
                    *reinterpret_cast<CResource**>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x10),
                    reinterpret_cast<CResource*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 4));
                *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) =
                    *reinterpret_cast<std::int32_t*>(*reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C);
            }

            if (diffuseLodU < static_cast<std::uint32_t>(
                                  static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x3A))))
            {
                *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x3A) = diffuseLod;
            }
        }

        CGraphicFrameData* pDiffuseGraphicFrameData;
        if (diffuseLodU < static_cast<std::uint32_t>(
                              static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x38))))
        {
            CGraphicInfoFrame::CFrameData::BeginLoading(pDiffuseFrameData, diffuseLodU, 0);
            pDiffuseGraphicFrameData =
                *reinterpret_cast<CGraphicFrameData**>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x1C);
        }
        else
        {
            pDiffuseGraphicFrameData =
                *reinterpret_cast<CGraphicFrameData**>(reinterpret_cast<std::uint8_t*>(pDiffuseFrameData) + 0x1C);
        }

        if (pDiffuseGraphicFrameData != nullptr)
        {
            *reinterpret_cast<std::int32_t*>(pDiffuseGraphicFrameData) =
                *reinterpret_cast<std::int32_t*>(pDiffuseGraphicFrameData) + 1;
        }

        std::int32_t diffuseHandle = 0;
        if (*reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pDiffuseGraphicFrameData) + 0x08) != nullptr)
        {
            diffuseHandle =
                **reinterpret_cast<std::int32_t**>(reinterpret_cast<std::uint8_t*>(pDiffuseGraphicFrameData) + 0x08);
        }

        if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3D04) != diffuseHandle)
        {
            *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3D04) = diffuseHandle;
            auto* const pDevice =
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF0);
            (**reinterpret_cast<void(__thiscall***)(void*, int, int)>(pDevice))[0x41](pDevice, 1, diffuseHandle);
        }

        if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8) < 2)
        {
            *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CB8) = 2;
        }

        std::uint32_t samplerHandle = 0;
        std::uint8_t samplerFlags = 0;
        if (pEntry->m_pSamplerBinding != nullptr)
        {
            samplerFlags = *reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint8_t*>(pEntry->m_pSamplerBinding) + 0x10);
            samplerHandle = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pEntry->m_pSamplerBinding) + 0x04);
        }

        {
            auto* const pDevice =
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF0);
            (**reinterpret_cast<void(__thiscall***)(void*, int, std::uint32_t, int, std::uint8_t)>(pDevice))[100](
                pDevice, 0, samplerHandle, 0, samplerFlags);
        }

        *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF4) = pEntry->m_pStreamBinding;
        std::uint32_t streamHandle = 0;
        if (pEntry->m_pStreamBinding != nullptr)
        {
            streamHandle = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pEntry->m_pStreamBinding) + 0x04);
        }

        {
            auto* const pDevice =
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF0);
            (**reinterpret_cast<void(__thiscall***)(void*, std::uint32_t)>(pDevice))[104](pDevice, streamHandle);
        }

        {
            CShaderRenderManager* const pShaderMgr = DAT_01436e14;
            auto* const pDirtyFlags =
                reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 0x3D8);

            if ((*reinterpret_cast<std::uint8_t*>(pDirtyFlags) & 4) != 0)
            {
                const std::int32_t ctx =
                    *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 4);
                int* pVS = *reinterpret_cast<int**>(ctx + 0x140);
                if (((*reinterpret_cast<std::int32_t*>(ctx + 0x144) - reinterpret_cast<std::intptr_t>(pVS)) >> 4) == 0)
                {
                    pVS = reinterpret_cast<int*>(ctx + 0x140);
                }

                CShaderRenderManager::SetVSConstants(
                    DAT_01436e14,
                    *reinterpret_cast<long long*>(*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 4) + 8),
                    pVS,
                    (*reinterpret_cast<std::int32_t*>(ctx + 0x144) - *reinterpret_cast<std::int32_t*>(ctx + 0x140)) >> 4);
            }
            if ((*reinterpret_cast<std::uint8_t*>(pDirtyFlags) & 1) != 0)
            {
                CShaderRenderManager::UpdatePixelShader(pShaderMgr);
            }
            if ((*reinterpret_cast<std::uint8_t*>(pDirtyFlags) & 2) != 0)
            {
                CShaderRenderManager::UpdatePixelShaderConstants(pShaderMgr);
            }
            if ((*pDirtyFlags & 0x2000) != 0)
            {
                CShaderRenderManager::UpdateWorldSpaceFogTransform(pShaderMgr);
            }
            if ((*reinterpret_cast<std::uint8_t*>(pDirtyFlags) & 4) != 0)
            {
                CShaderRenderManager::UpdateAmbient(pShaderMgr);
            }
            if ((*pDirtyFlags & 0x20000) != 0)
            {
                CShaderRenderManager::UpdateFogColour(pShaderMgr);
            }
            if ((*pDirtyFlags & 0x400) != 0)
            {
                CShaderRenderManager::UpdateLightGlobals(pShaderMgr);
            }
            if ((*pDirtyFlags & 0x200) != 0)
            {
                CShaderRenderManager::UpdateWorldSpaceLighting(pShaderMgr);
            }
            if (static_cast<char>(*reinterpret_cast<std::uint8_t*>(pDirtyFlags)) < 0)
            {
                CShaderRenderManager::UpdateCombinedProjectionTransform(pShaderMgr);
            }
            if ((*reinterpret_cast<std::uint8_t*>(pDirtyFlags) & 8) != 0)
            {
                CShaderRenderManager::UpdateAmbient(pShaderMgr);
            }
        }

        CRenderManagerCore::DrawCurrentBuffersIndexed(
            DAT_01436e18,
            0,
            static_cast<std::uint32_t>(pEntry->m_BaseVertexIndex),
            0,
            static_cast<std::uint32_t>(pEntry->m_PrimitiveCount));

        {
            const std::int32_t diffuseRefCount =
                (pDiffuseGraphicFrameData != nullptr) ? *reinterpret_cast<std::int32_t*>(pDiffuseGraphicFrameData) : 0;
            if (pDiffuseGraphicFrameData != nullptr)
            {
                *reinterpret_cast<std::int32_t*>(pDiffuseGraphicFrameData) = diffuseRefCount - 1;
                if (diffuseRefCount - 1 == 0)
                {
                    DestroyGraphicFrameData(pDiffuseGraphicFrameData);
                }
            }
        }

        ForegroundRenderEntry* const pNext = pEntry->m_pNext;
        iVar26 = iVar22;
        pEntry = pNext;
    }
}