bool __fastcall CEnginePrimitiveManagerMeshBase::GetOverlaidTextures(
    const CEngineTextureDecal& textureDecal,
    CArray<const CTexture*>& overlaidTextures,
    unsigned long appearanceId,
    const CGraphicDataBank* pGraphicDataBank)
{
    struct OverlaidTextureEntryOverlay
    {
        std::uint32_t m_TextureId; // 0x00
        std::byte m_Pad04[0x0C];
    };

    struct EngineTextureDecalOverlay
    {
        std::byte m_Pad00[0x08];
        OverlaidTextureEntryOverlay* m_pBegin; // 0x08
        OverlaidTextureEntryOverlay* m_pEnd;   // 0x0C
    };

    struct TextureHolderOverlay
    {
        std::byte m_Pad00[0x08];
        const CTexture* m_pTexture; // 0x08
    };

    struct ResourceListHeaderOverlay
    {
    };

    struct ResourceListOwnerOverlay
    {
        std::byte m_Pad00[0x04];
        ResourceListHeaderOverlay m_ListHeader; // 0x04
        std::byte m_Pad05[0x0B];
        CResource* m_pInsertAfter;             // 0x10
        std::byte m_Pad14[0x18];
        std::int32_t m_ListStamp;              // 0x2C
    };

    struct ResourceOverlay
    {
        std::byte m_Pad00[0x08];
        ResourceListOwnerOverlay* m_pListOwner; // 0x08
        std::byte m_Pad0C[0x0C];
        std::int32_t m_ListStamp;               // 0x18
    };

    struct FrameDataOverlay
    {
        std::byte m_Pad00[0x1C];
        TextureHolderOverlay* m_pTextureHolder; // 0x1C
        std::byte m_Pad20[0x04];
        CResource* m_pDependentResource;        // 0x24
        std::byte m_Pad28[0x10];
        char m_LoadThreshold;                   // 0x38
        char m_MinAppearanceId;                 // 0x39
        char m_AppearanceLimit;                 // 0x3A
        std::byte m_Pad3B[0x01];
        std::int32_t m_ResourcePriority;        // 0x3C
    };

    struct GraphicPrioritySourceOverlay
    {
        std::byte m_Pad00[0x194];
        std::int32_t m_ResourcePriority; // 0x194
    };

    struct GraphicPriorityOwnerOverlay
    {
        std::byte m_Pad00[0x20];
        GraphicPrioritySourceOverlay* m_pPrioritySource; // 0x20
    };

    struct GraphicInfoFrameOverlay
    {
        std::byte m_Pad00[0x08];
        CFrameData* m_pFrameData;                      // 0x08
        GraphicPriorityOwnerOverlay* m_pPriorityOwner; // 0x0C
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

    struct RefCountVtableOverlay
    {
        void* m_pUnknown00;                     // 0x00
        void(__thiscall* m_pRelease)(void*);    // 0x04
    };

    struct RefCountedObjectOverlay
    {
        RefCountVtableOverlay* m_pVtable; // 0x00
        std::int32_t m_RefCount;          // 0x04
    };

    static_assert(sizeof(OverlaidTextureEntryOverlay) == 0x10);
    static_assert(offsetof(EngineTextureDecalOverlay, m_pBegin) == 0x08);
    static_assert(offsetof(EngineTextureDecalOverlay, m_pEnd) == 0x0C);
    static_assert(offsetof(TextureHolderOverlay, m_pTexture) == 0x08);
    static_assert(offsetof(ResourceListOwnerOverlay, m_ListHeader) == 0x04);
    static_assert(offsetof(ResourceListOwnerOverlay, m_pInsertAfter) == 0x10);
    static_assert(offsetof(ResourceListOwnerOverlay, m_ListStamp) == 0x2C);
    static_assert(offsetof(ResourceOverlay, m_pListOwner) == 0x08);
    static_assert(offsetof(ResourceOverlay, m_ListStamp) == 0x18);
    static_assert(offsetof(FrameDataOverlay, m_pTextureHolder) == 0x1C);
    static_assert(offsetof(FrameDataOverlay, m_pDependentResource) == 0x24);
    static_assert(offsetof(FrameDataOverlay, m_LoadThreshold) == 0x38);
    static_assert(offsetof(FrameDataOverlay, m_AppearanceLimit) == 0x3A);
    static_assert(offsetof(FrameDataOverlay, m_ResourcePriority) == 0x3C);
    static_assert(offsetof(GraphicPrioritySourceOverlay, m_ResourcePriority) == 0x194);
    static_assert(offsetof(GraphicPriorityOwnerOverlay, m_pPrioritySource) == 0x20);
    static_assert(offsetof(GraphicInfoFrameOverlay, m_pFrameData) == 0x08);
    static_assert(offsetof(GraphicInfoFrameOverlay, m_pPriorityOwner) == 0x0C);
    static_assert(offsetof(GraphicDataBankEntryOverlay, m_pInfoFrame) == 0x28);
    static_assert(sizeof(GraphicDataBankEntryOverlay) == 0x2C);
    static_assert(offsetof(GraphicDataBankOverlay, m_pEntriesBegin) == 0x1E0);
    static_assert(offsetof(GraphicDataBankOverlay, m_pEntriesEnd) == 0x1E4);
    static_assert(offsetof(RefCountVtableOverlay, m_pRelease) == 0x04);
    static_assert(offsetof(RefCountedObjectOverlay, m_RefCount) == 0x04);

    const auto* const pTextureDecalOverlay =
        reinterpret_cast<const EngineTextureDecalOverlay*>(&textureDecal);
    const auto* const pGraphicDataBankOverlay =
        reinterpret_cast<const GraphicDataBankOverlay*>(pGraphicDataBank);

    const std::uint32_t textureCount = static_cast<std::uint32_t>(
        pTextureDecalOverlay->m_pEnd - pTextureDecalOverlay->m_pBegin);

    std::uint32_t initialValue = 0;
    overlaidTextures.resize(textureCount, reinterpret_cast<const CTexture*&>(initialValue));

    bool allTexturesAvailable = true;
    if (static_cast<std::int32_t>(textureCount) > 0)
    {
        std::int32_t textureIndex = 0;
        do
        {
            const std::uint32_t textureId = pTextureDecalOverlay->m_pBegin[textureIndex].m_TextureId;
            if (textureId < static_cast<std::uint32_t>(
                                pGraphicDataBankOverlay->m_pEntriesEnd -
                                pGraphicDataBankOverlay->m_pEntriesBegin))
            {
                const std::uint32_t entryAddress = textureId * 0x2C +
                    reinterpret_cast<std::uint32_t>(pGraphicDataBankOverlay->m_pEntriesBegin);
                if (entryAddress != 0)
                {
                    const auto* const pEntry =
                        reinterpret_cast<const GraphicDataBankEntryOverlay*>(entryAddress);
                    CGraphicInfoFrame* const pInfoFrame = pEntry->m_pInfoFrame;
                    if (pInfoFrame != nullptr)
                    {
                        auto* const pInfoFrameOverlay =
                            reinterpret_cast<GraphicInfoFrameOverlay*>(pInfoFrame);
                        CFrameData* pFrameData = pInfoFrameOverlay->m_pFrameData;
                        const auto outSlot = &overlaidTextures[textureIndex];

                        if (pFrameData == nullptr)
                        {
                            pFrameData = reinterpret_cast<CFrameData*>(::operator new(0x44));
                            if (pFrameData == nullptr)
                            {
                                pFrameData = nullptr;
                            }
                            else
                            {
                                pFrameData = reinterpret_cast<CFrameData*>(
                                    CGraphicInfoFrame::CFrameData::CFrameData(pFrameData, pInfoFrame));
                            }

                            auto* const pOldFrameData =
                                reinterpret_cast<RefCountedObjectOverlay*>(pInfoFrameOverlay->m_pFrameData);
                            if (pOldFrameData != nullptr)
                            {
                                pOldFrameData->m_RefCount = pOldFrameData->m_RefCount + -1;
                                if (pOldFrameData->m_RefCount == 0)
                                {
                                    pOldFrameData->m_pVtable->m_pRelease(pOldFrameData);
                                }
                                pInfoFrameOverlay->m_pFrameData = nullptr;
                            }
                            pInfoFrameOverlay->m_pFrameData = pFrameData;
                        }
                        else if (reinterpret_cast<FrameDataOverlay*>(pFrameData)->m_pTextureHolder != nullptr)
                        {
                            auto* const pFrameDataOverlay =
                                reinterpret_cast<FrameDataOverlay*>(pFrameData);
                            const std::int32_t resourcePriority =
                                pInfoFrameOverlay->m_pPriorityOwner->m_pPrioritySource->m_ResourcePriority;

                            if ((pFrameDataOverlay->m_ResourcePriority != resourcePriority) ||
                                (appearanceId <= static_cast<std::uint32_t>(
                                    static_cast<std::int32_t>(pFrameDataOverlay->m_AppearanceLimit))))
                            {
                                reinterpret_cast<FrameDataOverlay*>(pInfoFrameOverlay->m_pFrameData)
                                    ->m_ResourcePriority = resourcePriority;

                                CResource* pResource = pFrameDataOverlay->m_pDependentResource;
                                if ((pResource != nullptr) &&
                                    (reinterpret_cast<ResourceOverlay*>(pResource)->m_pListOwner != nullptr) &&
                                    (reinterpret_cast<ResourceOverlay*>(pResource)->m_ListStamp !=
                                     reinterpret_cast<ResourceOverlay*>(pResource)->m_pListOwner->m_ListStamp))
                                {
                                    CResource::ListRemove(pResource);
                                    CResource::ListInsert(
                                        pResource,
                                        reinterpret_cast<ResourceOverlay*>(pResource)->m_pListOwner->m_pInsertAfter,
                                        reinterpret_cast<CResource*>(
                                            &reinterpret_cast<ResourceOverlay*>(pResource)
                                                 ->m_pListOwner->m_ListHeader));
                                    reinterpret_cast<ResourceOverlay*>(pResource)->m_ListStamp =
                                        reinterpret_cast<ResourceOverlay*>(pResource)->m_pListOwner->m_ListStamp;
                                }

                                pResource = reinterpret_cast<CResource*>(pInfoFrameOverlay->m_pFrameData);
                                if ((appearanceId <= static_cast<std::uint32_t>(
                                         static_cast<std::int32_t>(
                                             reinterpret_cast<FrameDataOverlay*>(pResource)->m_LoadThreshold))) &&
                                    (reinterpret_cast<ResourceOverlay*>(pResource)->m_pListOwner != nullptr) &&
                                    (reinterpret_cast<ResourceOverlay*>(pResource)->m_ListStamp !=
                                     reinterpret_cast<ResourceOverlay*>(pResource)->m_pListOwner->m_ListStamp))
                                {
                                    CResource::ListRemove(pResource);
                                    CResource::ListInsert(
                                        pResource,
                                        reinterpret_cast<ResourceOverlay*>(pResource)->m_pListOwner->m_pInsertAfter,
                                        reinterpret_cast<CResource*>(
                                            &reinterpret_cast<ResourceOverlay*>(pResource)
                                                 ->m_pListOwner->m_ListHeader));
                                    reinterpret_cast<ResourceOverlay*>(pResource)->m_ListStamp =
                                        reinterpret_cast<ResourceOverlay*>(pResource)->m_pListOwner->m_ListStamp;
                                }

                                if (appearanceId < static_cast<std::uint32_t>(
                                                       static_cast<std::int32_t>(
                                                           reinterpret_cast<FrameDataOverlay*>(
                                                               pInfoFrameOverlay->m_pFrameData)
                                                               ->m_AppearanceLimit)))
                                {
                                    reinterpret_cast<FrameDataOverlay*>(pInfoFrameOverlay->m_pFrameData)
                                        ->m_AppearanceLimit = static_cast<char>(appearanceId);
                                }
                            }

                            pFrameData = pInfoFrameOverlay->m_pFrameData;
                            if (appearanceId < static_cast<std::uint32_t>(
                                                   static_cast<std::int32_t>(
                                                       reinterpret_cast<FrameDataOverlay*>(pFrameData)
                                                           ->m_LoadThreshold)))
                            {
                                CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, appearanceId, 0);
                                *outSlot = reinterpret_cast<FrameDataOverlay*>(pInfoFrameOverlay->m_pFrameData)
                                               ->m_pTextureHolder->m_pTexture;
                            }
                            else
                            {
                                *outSlot =
                                    reinterpret_cast<FrameDataOverlay*>(pFrameData)->m_pTextureHolder->m_pTexture;
                            }
                            goto next_texture;
                        }

                        CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, appearanceId, 0);
                        allTexturesAvailable = false;
                    }
                }
            }

        next_texture:
            ++textureIndex;
        } while (textureIndex < static_cast<std::int32_t>(textureCount));
    }

    return allTexturesAvailable;
}