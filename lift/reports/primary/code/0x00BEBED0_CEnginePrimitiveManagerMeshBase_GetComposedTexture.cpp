const CTexture* __fastcall CEnginePrimitiveManagerMeshBase::GetComposedTexture(
    long& composedTextureKey,
    CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo>>& surfaceInfoMap,
    unsigned long appearanceId,
    bool useAlpha,
    const CGraphicDataBank* /*pGraphicDataBank*/)
{
    struct SurfaceSpanOverlay
    {
        std::byte m_Pad00[0x28];
        long* m_pBegin; // 0x28
        long* m_pEnd;   // 0x2C
    };

    struct CompositeSurfaceInfoOverlay
    {
        std::byte m_Pad00[0x04];
        SurfaceSpanOverlay* m_pSurfaceSpan; // 0x04
    };

    struct MapEntryOverlay
    {
        long m_Key;                              // 0x00
        CompositeSurfaceInfoOverlay m_Value;     // 0x04
    };

    struct VectorMapOverlay
    {
        std::byte m_Pad00[0x04];
        MapEntryOverlay* m_pEnd; // 0x04
    };

    struct GraphicFrameOwnerOverlay
    {
        std::int32_t m_RefCount;                 // 0x00
        void(__thiscall* m_pDestroy)(void*);     // 0x04
        void* m_pDestroyThis;                    // 0x08
    };

    struct GraphicFrameDataOverlay
    {
        std::int32_t m_RefCount;                     // 0x00
        std::byte m_Pad04[0x04];
        const CTexture* m_pTexture;                  // 0x08
        GraphicFrameOwnerOverlay* m_pOwner;          // 0x0C
        std::byte m_Pad10[0x0A];
        std::uint8_t m_Flags1A;                      // 0x1A
    };

    static_assert(offsetof(SurfaceSpanOverlay, m_pBegin) == 0x28);
    static_assert(offsetof(SurfaceSpanOverlay, m_pEnd) == 0x2C);
    static_assert(offsetof(CompositeSurfaceInfoOverlay, m_pSurfaceSpan) == 0x04);
    static_assert(offsetof(MapEntryOverlay, m_Value) == 0x04);
    static_assert(offsetof(VectorMapOverlay, m_pEnd) == 0x04);
    static_assert(offsetof(GraphicFrameOwnerOverlay, m_pDestroy) == 0x04);
    static_assert(offsetof(GraphicFrameOwnerOverlay, m_pDestroyThis) == 0x08);
    static_assert(offsetof(GraphicFrameDataOverlay, m_pTexture) == 0x08);
    static_assert(offsetof(GraphicFrameDataOverlay, m_pOwner) == 0x0C);
    static_assert(offsetof(GraphicFrameDataOverlay, m_Flags1A) == 0x1A);

    auto ReleaseGraphicFrameData = [](CGraphicFrameData* pFrameData) -> void
    {
        auto* const pFrameDataOverlay = reinterpret_cast<GraphicFrameDataOverlay*>(pFrameData);

        pFrameDataOverlay->m_RefCount = pFrameDataOverlay->m_RefCount - 1;
        if (pFrameDataOverlay->m_RefCount == 0)
        {
            if ((pFrameDataOverlay->m_Flags1A & 0x10) != 0)
            {
                CGraphicFrameData::Uninitialise(pFrameData);
            }

            GraphicFrameOwnerOverlay* const pOwner = pFrameDataOverlay->m_pOwner;
            if (pOwner != nullptr)
            {
                pOwner->m_RefCount = pOwner->m_RefCount - 1;
                if (pOwner->m_RefCount == 0)
                {
                    pOwner->m_pDestroy(pOwner->m_pDestroyThis);
                    operator_delete(pOwner);
                }
            }

            pFrameDataOverlay->m_pTexture = nullptr;
            pFrameDataOverlay->m_pOwner = nullptr;
            operator_delete(pFrameData);
        }
    };

    CGraphicFrameData* pComposedFrameData = reinterpret_cast<CGraphicFrameData*>(&composedTextureKey);

    auto* pIt = reinterpret_cast<MapEntryOverlay*>(
        CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo>>::LowerBound(
            &surfaceInfoMap,
            &composedTextureKey));

    auto* const pEnd =
        reinterpret_cast<VectorMapOverlay*>(&surfaceInfoMap)->m_pEnd;

    if ((pIt == pEnd) || (composedTextureKey < pIt->m_Key))
    {
        pIt = pEnd;
    }

    if (pIt != pEnd)
    {
        SurfaceSpanOverlay* const pSurfaceSpan = pIt->m_Value.m_pSurfaceSpan;
        if ((((reinterpret_cast<std::intptr_t>(pSurfaceSpan->m_pEnd) -
               reinterpret_cast<std::intptr_t>(pSurfaceSpan->m_pBegin)) >> 4) > 1) &&
            (*reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436eac) + 0x10D4) != 0))
        {
            pComposedFrameData = nullptr;

            const bool composed =
                CEngineSurfaceCompositionManager::GetComposedSurface(
                    DAT_01436eac,
                    reinterpret_cast<CGraphicFrame*>(&pComposedFrameData),
                    reinterpret_cast<CCompositeSurfaceInfo*>(&pIt->m_Value),
                    appearanceId,
                    useAlpha);

            CGraphicFrameData* const pReturnedFrameData = pComposedFrameData;
            if (composed)
            {
                const CTexture* const pTexture =
                    reinterpret_cast<GraphicFrameDataOverlay*>(pComposedFrameData)->m_pTexture;

                ReleaseGraphicFrameData(pReturnedFrameData);
                return pTexture;
            }

            if (pComposedFrameData != nullptr)
            {
                ReleaseGraphicFrameData(pReturnedFrameData);
            }
        }

        composedTextureKey = *pSurfaceSpan->m_pBegin;
    }

    return nullptr;
}