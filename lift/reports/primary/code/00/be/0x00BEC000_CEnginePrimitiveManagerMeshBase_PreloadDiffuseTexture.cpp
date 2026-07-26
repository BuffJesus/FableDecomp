void __fastcall CEnginePrimitiveManagerMeshBase::PreloadDiffuseTexture(
    const C3DMeshMaterial* pMaterial,
    CEngineInternalPrimitiveMorphInfo* pMorphInfo,
    unsigned long appearanceId,
    bool& outPendingLoad,
    const CGraphicDataBank* pGraphicDataBank)
{
    struct TextureKeyOverlay
    {
        long m_TextureId;          // 0x00
        std::int32_t m_SubIndex;   // 0x04
    };

    struct TextureSurfaceEntryOverlay
    {
        long m_TextureId;          // 0x00
        std::int32_t m_SubIndex;   // 0x04
        std::byte m_Pad08[0x08];
        std::int32_t* m_pBegin;    // 0x10
        std::int32_t* m_pEnd;      // 0x14
    };

    struct TextureSurfaceMapOverlay
    {
        TextureSurfaceEntryOverlay* m_pBegin; // 0x00
        TextureSurfaceEntryOverlay* m_pEnd;   // 0x04
    };

    struct MorphInfoOverlay
    {
        TextureSurfaceMapOverlay m_TextureSurfaceMap; // 0x00
        std::byte m_Pad08[0x08];
        std::byte m_ComposedSurfaceInfoMap[0x10];     // 0x10
    };

    struct MaterialOverlay
    {
        std::byte m_Pad00[0x0C];
        std::int32_t m_SubIndex;   // 0x0C
        long m_TextureId;          // 0x10
    };

    struct GraphicDataBankEntryOverlay
    {
        std::byte m_Pad00[0x28];
        CGraphicInfoFrame* m_pInfoFrame; // 0x28
    };

    struct GraphicDataBankOverlay
    {
        std::byte m_Pad00[0x1E0];
        GraphicDataBankEntryOverlay* m_pEntriesBegin; // 0x1E0
        GraphicDataBankEntryOverlay* m_pEntriesEnd;   // 0x1E4
    };

    static_assert(offsetof(TextureSurfaceEntryOverlay, m_pBegin) == 0x10);
    static_assert(offsetof(TextureSurfaceEntryOverlay, m_pEnd) == 0x14);
    static_assert(offsetof(TextureSurfaceMapOverlay, m_pEnd) == 0x04);
    static_assert(offsetof(MorphInfoOverlay, m_ComposedSurfaceInfoMap) == 0x10);
    static_assert(offsetof(MaterialOverlay, m_SubIndex) == 0x0C);
    static_assert(offsetof(MaterialOverlay, m_TextureId) == 0x10);
    static_assert(offsetof(GraphicDataBankEntryOverlay, m_pInfoFrame) == 0x28);
    static_assert(offsetof(GraphicDataBankOverlay, m_pEntriesBegin) == 0x1E0);
    static_assert(offsetof(GraphicDataBankOverlay, m_pEntriesEnd) == 0x1E4);

    auto* const pMaterialOverlay = reinterpret_cast<const MaterialOverlay*>(pMaterial);
    const long textureId = pMaterialOverlay->m_TextureId;
    if (textureId == 0)
    {
        return;
    }

    auto* const pGraphicDataBankOverlay =
        reinterpret_cast<const GraphicDataBankOverlay*>(pGraphicDataBank);

    auto GetGraphicInfoFrameByIndex =
        [pGraphicDataBankOverlay](const std::uint32_t index) -> CGraphicInfoFrame*
    {
        const std::uint32_t frameCount = static_cast<std::uint32_t>(
            pGraphicDataBankOverlay->m_pEntriesEnd - pGraphicDataBankOverlay->m_pEntriesBegin);

        if (index < frameCount)
        {
            const GraphicDataBankEntryOverlay* const pEntry =
                pGraphicDataBankOverlay->m_pEntriesBegin + index;
            if (pEntry != nullptr)
            {
                return pEntry->m_pInfoFrame;
            }
        }

        return nullptr;
    };

    auto UpdateResourcePriority =
        [](CResource* const pResource) -> void
    {
        if ((pResource != nullptr) &&
            (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) != 0) &&
            (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) !=
             *reinterpret_cast<std::int32_t*>(
                 *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C)))
        {
            CResource::ListRemove(pResource);
            CResource::ListInsert(
                pResource,
                *reinterpret_cast<CResource**>(
                    *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x10),
                reinterpret_cast<CResource*>(
                    *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 4));
            *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) =
                *reinterpret_cast<std::int32_t*>(
                    *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C);
        }
    };

    auto TryPreloadInfoFrame =
        [&](CGraphicInfoFrame* const pInfoFrame, const unsigned long lod) -> bool
    {
        if (pInfoFrame == nullptr)
        {
            return false;
        }

        const unsigned long acceptableMipmap = GetAcceptablePreloadMipmap(lod, pInfoFrame);
        CFrameData* pFrameData =
            *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08);

        if (pFrameData == nullptr)
        {
            pFrameData = reinterpret_cast<CFrameData*>(::operator_new(0x44));
            if (pFrameData == nullptr)
            {
                pFrameData = nullptr;
            }
            else
            {
                pFrameData = reinterpret_cast<CFrameData*>(
                    CGraphicInfoFrame::CFrameData::CFrameData(pFrameData, pInfoFrame));
            }

            int* const pOld = *reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08);
            if (pOld != nullptr)
            {
                int* const pRefCount = pOld + 1;
                *pRefCount = *pRefCount - 1;
                if (*pRefCount == 0)
                {
                    (reinterpret_cast<void(__thiscall*)(int*)>((*reinterpret_cast<void***>(pOld))[1]))(pOld);
                }
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) = nullptr;
            }

            *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) = pFrameData;
            CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, lod, 0);
            return true;
        }

        if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x1C) == 0)
        {
            CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, lod, 0);
            return true;
        }

        const std::int32_t priority =
            *reinterpret_cast<std::int32_t*>(
                *reinterpret_cast<std::uint8_t**>(
                    *reinterpret_cast<std::uint8_t**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x0C) + 0x20) +
                0x194);

        if ((*reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3C) != priority) ||
            (lod <= static_cast<std::uint32_t>(
                static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3A)))))
        {
            *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3C) = priority;

            CResource* pResource =
                *reinterpret_cast<CResource**>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x24);
            UpdateResourcePriority(pResource);

            pResource = reinterpret_cast<CResource*>(pFrameData);
            if (lod <= static_cast<std::uint32_t>(
                           static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x38))))
            {
                UpdateResourcePriority(pResource);
            }

            if (lod < static_cast<std::uint32_t>(
                          static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3A))))
            {
                *reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3A) =
                    static_cast<std::int8_t>(lod);
            }
        }

        if (lod < static_cast<std::uint32_t>(
                      static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x38))))
        {
            CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, lod, 0);
        }

        const std::uint32_t currentMipmap =
            (*reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) == nullptr)
                ? 0xFFFFFFFFUL
                : static_cast<std::uint32_t>(
                      static_cast<std::int32_t>(*reinterpret_cast<std::int8_t*>(
                          reinterpret_cast<std::uint8_t*>(
                              *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08)) +
                          0x38)));

        if (currentMipmap <= acceptableMipmap)
        {
            return false;
        }

        return true;
    };

    long composedTextureKey = textureId;

    if (pMorphInfo != nullptr)
    {
        auto* const pMorphInfoOverlay = reinterpret_cast<MorphInfoOverlay*>(pMorphInfo);

        TextureKeyOverlay textureKey;
        textureKey.m_TextureId = textureId;
        textureKey.m_SubIndex = pMaterialOverlay->m_SubIndex;

        TextureSurfaceEntryOverlay* pIt = std::lower_bound(
            pMorphInfoOverlay->m_TextureSurfaceMap.m_pBegin,
            pMorphInfoOverlay->m_TextureSurfaceMap.m_pEnd,
            textureKey,
            [](const TextureSurfaceEntryOverlay& lhs, const TextureKeyOverlay& rhs) -> bool
            {
                if (lhs.m_TextureId < rhs.m_TextureId)
                {
                    return true;
                }
                if (rhs.m_TextureId < lhs.m_TextureId)
                {
                    return false;
                }
                return lhs.m_SubIndex < rhs.m_SubIndex;
            });

        TextureSurfaceEntryOverlay* const pEnd = pMorphInfoOverlay->m_TextureSurfaceMap.m_pEnd;
        if ((pIt == pEnd) ||
            (textureId < pIt->m_TextureId) ||
            ((textureId <= pIt->m_TextureId) && (pMaterialOverlay->m_SubIndex < pIt->m_SubIndex)))
        {
            pIt = pEnd;
        }

        if ((pIt != pEnd) && (pIt != reinterpret_cast<TextureSurfaceEntryOverlay*>(-8)))
        {
            std::int32_t remaining = static_cast<std::int32_t>((pIt->m_pEnd - pIt->m_pBegin) >> 2);
            if (remaining > 0)
            {
                std::int32_t* pSurfaceIndex = pIt->m_pBegin;
                do
                {
                    if (TryPreloadInfoFrame(GetGraphicInfoFrameByIndex(static_cast<std::uint32_t>(*pSurfaceIndex)), appearanceId))
                    {
                        outPendingLoad = true;
                    }

                    pSurfaceIndex = reinterpret_cast<std::int32_t*>(
                        reinterpret_cast<std::uint8_t*>(pSurfaceIndex) + 0x10);
                    remaining = remaining - 1;
                } while (remaining != 0);
            }
        }

        if (GetComposedTexture(
                composedTextureKey,
                *reinterpret_cast<CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo>>*>(
                    pMorphInfoOverlay->m_ComposedSurfaceInfoMap),
                appearanceId,
                false,
                pGraphicDataBank) != nullptr)
        {
            return;
        }
    }

    if (TryPreloadInfoFrame(
            GetGraphicInfoFrameByIndex(static_cast<std::uint32_t>(composedTextureKey)),
            appearanceId))
    {
        outPendingLoad = true;
    }
}