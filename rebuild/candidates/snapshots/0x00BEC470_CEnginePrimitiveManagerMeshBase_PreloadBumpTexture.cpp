void __fastcall CEnginePrimitiveManagerMeshBase::PreloadBumpTexture(
    const C3DMeshMaterial* pMaterial,
    CEngineInternalPrimitiveMorphInfo* pMorphInfo,
    unsigned long appearanceId,
    bool& outPendingLoad,
    const CGraphicDataBank* pGraphicDataBank)
{
    struct MaterialOverlay
    {
        std::byte m_Pad00[0x14];
        std::uint32_t m_BumpTextureId; // 0x14
    };

    struct MorphInfoOverlay
    {
        std::byte m_Pad00[0x10];
        std::byte m_ComposedSurfaceInfoMap[0x10]; // 0x10
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

    static_assert(offsetof(MaterialOverlay, m_BumpTextureId) == 0x14);
    static_assert(offsetof(MorphInfoOverlay, m_ComposedSurfaceInfoMap) == 0x10);
    static_assert(offsetof(GraphicDataBankEntryOverlay, m_pInfoFrame) == 0x28);
    static_assert(offsetof(GraphicDataBankOverlay, m_pEntriesBegin) == 0x1E0);
    static_assert(offsetof(GraphicDataBankOverlay, m_pEntriesEnd) == 0x1E4);

    const auto* const pMaterialOverlay = reinterpret_cast<const MaterialOverlay*>(pMaterial);
    std::uint32_t localTextureKey = pMaterialOverlay->m_BumpTextureId;
    if (localTextureKey == 0)
    {
        return;
    }

    if (pMorphInfo != nullptr)
    {
        auto* const pMorphInfoOverlay = reinterpret_cast<MorphInfoOverlay*>(pMorphInfo);
        using GetComposedTextureFn = const CTexture*(__fastcall*)(
            long*,
            CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo>>*,
            unsigned long,
            bool,
            const CGraphicDataBank*);
        const auto pGetComposedTexture =
            reinterpret_cast<GetComposedTextureFn>(&CEnginePrimitiveManagerMeshBase::GetComposedTexture);

        if (pGetComposedTexture(
                reinterpret_cast<long*>(&localTextureKey),
                reinterpret_cast<CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo>>*>(
                    pMorphInfoOverlay->m_ComposedSurfaceInfoMap),
                appearanceId,
                true,
                pGraphicDataBank) != nullptr)
        {
            return;
        }
    }

    const auto* const pGraphicDataBankOverlay =
        reinterpret_cast<const GraphicDataBankOverlay*>(pGraphicDataBank);
    if (localTextureKey >= static_cast<std::uint32_t>(
                               pGraphicDataBankOverlay->m_pEntriesEnd - pGraphicDataBankOverlay->m_pEntriesBegin))
    {
        return;
    }

    const auto* const pEntry = pGraphicDataBankOverlay->m_pEntriesBegin + localTextureKey;
    if (pEntry == nullptr)
    {
        return;
    }

    CGraphicInfoFrame* const pInfoFrame = pEntry->m_pInfoFrame;
    if (pInfoFrame == nullptr)
    {
        return;
    }

    const unsigned long acceptablePreloadMipmap = GetAcceptablePreloadMipmap(appearanceId, pInfoFrame);
    CFrameData* pFrameData = *reinterpret_cast<CFrameData**>(reinterpret_cast<std::byte*>(pInfoFrame) + 0x08);
    if (pFrameData == nullptr)
    {
        pFrameData = reinterpret_cast<CFrameData*>(::operator_new(0x44));
        if (pFrameData != nullptr)
        {
            pFrameData =
                reinterpret_cast<CFrameData*>(CGraphicInfoFrame::CFrameData::CFrameData(pFrameData, pInfoFrame));
        }

        int* const pOldFrameData = *reinterpret_cast<int**>(reinterpret_cast<std::byte*>(pInfoFrame) + 0x08);
        if (pOldFrameData != nullptr)
        {
            int* const pRefCount = pOldFrameData + 1;
            *pRefCount = *pRefCount - 1;
            if (*pRefCount == 0)
            {
                using VtableSlot1Fn = void(__thiscall*)(void*);
                const std::uintptr_t vtable = *reinterpret_cast<std::uintptr_t*>(pOldFrameData);
                const auto pFn = *reinterpret_cast<VtableSlot1Fn*>(vtable + 0x4);
                pFn(pOldFrameData);
            }
            *reinterpret_cast<void**>(reinterpret_cast<std::byte*>(pInfoFrame) + 0x08) = nullptr;
        }

        *reinterpret_cast<CFrameData**>(reinterpret_cast<std::byte*>(pInfoFrame) + 0x08) = pFrameData;
        CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, appearanceId, 0);
        outPendingLoad = true;
        return;
    }

    if (*reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pFrameData) + 0x1C) == 0)
    {
        CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, appearanceId, 0);
        outPendingLoad = true;
        return;
    }

    const int resourcePriority = *reinterpret_cast<int*>(
        *reinterpret_cast<std::byte**>(*reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pInfoFrame) + 0x0C) +
                                       0x20) +
        0x194);

    if ((*reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pFrameData) + 0x3C) != resourcePriority) ||
        (appearanceId <= static_cast<std::uint32_t>(
                             static_cast<int>(*reinterpret_cast<signed char*>(reinterpret_cast<std::byte*>(pFrameData) +
                                                                             0x3A)))))
    {
        *reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pFrameData) + 0x3C) = resourcePriority;

        CResource* pResource = *reinterpret_cast<CResource**>(reinterpret_cast<std::byte*>(pFrameData) + 0x24);
        if ((pResource != nullptr) &&
            (*reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pResource) + 0x08) != 0) &&
            (*reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pResource) + 0x18) !=
             *reinterpret_cast<int*>(*reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pResource) + 0x08) +
                                     0x2C)))
        {
            CResource::ListRemove(pResource);
            CResource::ListInsert(
                pResource,
                *reinterpret_cast<CResource**>(*reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pResource) +
                                                                              0x08) +
                                               0x10),
                reinterpret_cast<CResource*>(
                    *reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pResource) + 0x08) + 0x04));
            *reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pResource) + 0x18) =
                *reinterpret_cast<int*>(*reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pResource) + 0x08) +
                                        0x2C);
        }

        pResource = reinterpret_cast<CResource*>(pFrameData);
        if ((appearanceId <= static_cast<std::uint32_t>(
                                 static_cast<int>(*reinterpret_cast<signed char*>(reinterpret_cast<std::byte*>(pResource) +
                                                                                 0x38)))) &&
            (*reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pResource) + 0x08) != 0) &&
            (*reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pResource) + 0x18) !=
             *reinterpret_cast<int*>(*reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pResource) + 0x08) +
                                     0x2C)))
        {
            CResource::ListRemove(pResource);
            CResource::ListInsert(
                pResource,
                *reinterpret_cast<CResource**>(*reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pResource) +
                                                                              0x08) +
                                               0x10),
                reinterpret_cast<CResource*>(
                    *reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pResource) + 0x08) + 0x04));
            *reinterpret_cast<int*>(reinterpret_cast<std::byte*>(pResource) + 0x18) =
                *reinterpret_cast<int*>(*reinterpret_cast<std::byte**>(reinterpret_cast<std::byte*>(pResource) + 0x08) +
                                        0x2C);
        }

        if (appearanceId < static_cast<std::uint32_t>(
                               static_cast<int>(*reinterpret_cast<signed char*>(reinterpret_cast<std::byte*>(pFrameData) +
                                                                               0x3A))))
        {
            *reinterpret_cast<signed char*>(reinterpret_cast<std::byte*>(pFrameData) + 0x3A) =
                static_cast<signed char>(appearanceId);
        }
    }

    if (appearanceId < static_cast<std::uint32_t>(
                           static_cast<int>(*reinterpret_cast<signed char*>(reinterpret_cast<std::byte*>(pFrameData) +
                                                                           0x38))))
    {
        CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, appearanceId, 0);
    }

    std::uint32_t currentPreloadMipmap;
    if (*reinterpret_cast<void**>(reinterpret_cast<std::byte*>(pInfoFrame) + 0x08) == nullptr)
    {
        currentPreloadMipmap = 0xFFFFFFFFu;
    }
    else
    {
        currentPreloadMipmap = static_cast<std::uint32_t>(
            static_cast<int>(*reinterpret_cast<signed char*>(
                reinterpret_cast<std::byte*>(*reinterpret_cast<void**>(reinterpret_cast<std::byte*>(pInfoFrame) + 0x08)) +
                0x38)));
    }

    if (acceptablePreloadMipmap < currentPreloadMipmap)
    {
        outPendingLoad = true;
    }
}