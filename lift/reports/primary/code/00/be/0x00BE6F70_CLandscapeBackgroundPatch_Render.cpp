bool __thiscall CLandscapeBackgroundPatch::Render(bool /*unused*/)
{
    auto* const self = reinterpret_cast<std::uint8_t*>(this);

    auto*& m_pLandscape = *reinterpret_cast<void**>(self + 0x04);
    auto*& m_pStaticVertexBuffer = *reinterpret_cast<void**>(self + 0x0B8);
    auto*& m_pStaticIndexBuffer = *reinterpret_cast<void**>(self + 0x0BC);
    auto*& m_pBaseTexture = *reinterpret_cast<void**>(self + 0x0C0);
    auto*& m_pLayerIndexBuffer0 = *reinterpret_cast<void**>(self + 0x0C4);
    auto*& m_pLayerIndexBuffer1 = *reinterpret_cast<void**>(self + 0x0C8);
    auto*& m_pLayerIndexBuffer2 = *reinterpret_cast<void**>(self + 0x0CC);
    auto*& m_pLayerIndexBuffer3 = *reinterpret_cast<void**>(self + 0x0D0);
    auto*& m_pLayerVertexBuffer0 = *reinterpret_cast<void**>(self + 0x0D4);
    auto*& m_pLayerVertexBuffer1 = *reinterpret_cast<void**>(self + 0x0D8);
    auto*& m_pLayerVertexBuffer2 = *reinterpret_cast<void**>(self + 0x0DC);
    auto*& m_pLayerVertexBuffer3 = *reinterpret_cast<void**>(self + 0x0E0);
    auto& m_LayerIndexCount0 = *reinterpret_cast<std::uint16_t*>(self + 0x0E4);
    auto& m_LayerIndexCount1 = *reinterpret_cast<std::uint16_t*>(self + 0x0E6);
    auto& m_LayerIndexCount2 = *reinterpret_cast<std::uint16_t*>(self + 0x0E8);
    auto& m_LayerIndexCount3 = *reinterpret_cast<std::uint16_t*>(self + 0x0EA);
    auto& m_IndexCount = *reinterpret_cast<std::uint16_t*>(self + 0x104);
    auto& m_VertexCount = *reinterpret_cast<std::uint16_t*>(self + 0x106);
    auto& m_SectionX = *reinterpret_cast<std::uint16_t*>(self + 0x10C);
    auto& m_SectionY = *reinterpret_cast<std::uint16_t*>(self + 0x10E);
    auto& m_ShaderVariant = *reinterpret_cast<std::uint8_t*>(self + 0x118);
    auto& m_DisableSpecialShaderPath = *reinterpret_cast<std::uint8_t*>(self + 0x11B);

    CRenderStateManager* const pRenderStateManager = DAT_013cb508;
    if (m_VertexCount == 0) {
        return false;
    }

    bool bUseSpecialShaderPath = false;
    if ((m_DisableSpecialShaderPath == 0) && (m_ShaderVariant == 1)) {
        const int tableBase = *reinterpret_cast<int*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(m_pLandscape) + 0xA4) + 0x08);
        if (tableBase != 0) {
            const int cell = tableBase +
                ((static_cast<std::uint32_t>(m_SectionY) >> 4) *
                 *reinterpret_cast<int*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(m_pLandscape) + 0xA4) + 0x0C) +
                 (static_cast<std::uint32_t>(m_SectionX) >> 4)) * 0x48;
            if ((cell != 0) && ((*reinterpret_cast<std::uint8_t*>(cell + 0x3C) & 4) != 0)) {
                bUseSpecialShaderPath = true;
            }
        }
    }

    {
        auto* const captureIndex = reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pRenderStateManager) + 0x2808);
        *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pRenderStateManager) + (*captureIndex) * 8 + 8) =
            reinterpret_cast<std::uint8_t*>(pRenderStateManager) + 0x3A24;
        *captureIndex = *captureIndex + 1;
        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pRenderStateManager) + 0x2814) =
            *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pRenderStateManager) + 0x2814) << 1;
    }

    CRenderManagerCore* pRenderManager = DAT_01436e18;
    int staticVB = reinterpret_cast<int>(m_pStaticVertexBuffer);

    {
        CRenderManagerCore* mgr = DAT_01436e18;
        int value = DAT_01396dcc;
        auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x2EE4;
        if (*reinterpret_cast<int*>(state + 0x04) != value) {
            if ((*reinterpret_cast<unsigned int*>(state + 0x08) &
                 *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814)) == 0) {
                *reinterpret_cast<unsigned int*>(state + 0x08) =
                    *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) |
                    *reinterpret_cast<unsigned int*>(state + 0x08);
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                    *reinterpret_cast<std::uint32_t*>(state + 0x04);
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
            }
            *reinterpret_cast<int*>(state + 0x04) = value;
            if (*(state + 0x10) == 0) {
                *(state + 0x10) = 1;
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
            }
        }
    }
    {
        CRenderManagerCore* mgr = DAT_01436e18;
        int value = DAT_01396dd0;
        auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x2F84;
        if (*reinterpret_cast<int*>(state + 0x04) != value) {
            if ((*reinterpret_cast<unsigned int*>(state + 0x08) &
                 *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814)) == 0) {
                *reinterpret_cast<unsigned int*>(state + 0x08) =
                    *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) |
                    *reinterpret_cast<unsigned int*>(state + 0x08);
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                    *reinterpret_cast<std::uint32_t*>(state + 0x04);
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
            }
            *reinterpret_cast<int*>(state + 0x04) = value;
            if (*(state + 0x10) == 0) {
                *(state + 0x10) = 1;
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
            }
        }
    }
    {
        CRenderManagerCore* mgr = DAT_01436e18;
        int value = DAT_01396dd0;
        auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x2EF8;
        if (*reinterpret_cast<int*>(state + 0x04) != value) {
            if ((*reinterpret_cast<unsigned int*>(state + 0x08) &
                 *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814)) == 0) {
                *reinterpret_cast<unsigned int*>(state + 0x08) =
                    *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) |
                    *reinterpret_cast<unsigned int*>(state + 0x08);
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                    *reinterpret_cast<std::uint32_t*>(state + 0x04);
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
            }
            *reinterpret_cast<int*>(state + 0x04) = value;
            if (*(state + 0x10) == 0) {
                *(state + 0x10) = 1;
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
            }
        }
    }
    {
        CRenderManagerCore* mgr = DAT_01436e18;
        int value = DAT_01396dd0;
        auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x2F98;
        if (*reinterpret_cast<int*>(state + 0x04) != value) {
            if ((*reinterpret_cast<unsigned int*>(state + 0x08) &
                 *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814)) == 0) {
                *reinterpret_cast<unsigned int*>(state + 0x08) =
                    *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) |
                    *reinterpret_cast<unsigned int*>(state + 0x08);
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                    *reinterpret_cast<std::uint32_t*>(state + 0x04);
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
            }
            *reinterpret_cast<int*>(state + 0x04) = value;
            if (*(state + 0x10) == 0) {
                *(state + 0x10) = 1;
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
            }
        }
    }

    CShaderRenderManager::ApplyPixelShader(
        DAT_01436e14,
        reinterpret_cast<CPixelShader*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x560));

    CGraphicFrameData* pGraphicFrameData = nullptr;
    if (staticVB == 0) {
        const int frameMgr = *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(DAT_01436e98) + 0x954);
        const std::uint32_t frameIndex = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e24) + 0xA4);
        const int begin = *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(frameMgr) + 0x1E0);

        CGraphicInfoFrame* pInfoFrame;
        if ((frameIndex < static_cast<std::uint32_t>((*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(frameMgr) + 0x1E4) - begin) / 0x2C)) &&
            ((frameIndex * 0x2C + begin) != 0)) {
            pInfoFrame = *reinterpret_cast<CGraphicInfoFrame**>(frameIndex * 0x2C + begin + 0x28);
        } else {
            pInfoFrame = nullptr;
        }

        CFrameData* pFrameData = *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08);
        if (pFrameData == nullptr) {
            pFrameData = reinterpret_cast<CFrameData*>(::operator_new(0x44));
            if (pFrameData == nullptr) {
                pFrameData = nullptr;
            } else {
                pFrameData = reinterpret_cast<CFrameData*>(CGraphicInfoFrame::CFrameData::CFrameData(pFrameData, pInfoFrame));
            }

            int* piVar14 = *reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08);
            if (piVar14 != nullptr) {
                int* piVar1 = piVar14 + 1;
                *piVar1 = *piVar1 - 1;
                if (*piVar1 == 0) {
                    reinterpret_cast<void(*)()>(*reinterpret_cast<int*>(*piVar14 + 4))();
                }
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) = 0;
            }

            *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) = pFrameData;
LAB_00be735f:
            CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, 0, 1);
            if (CGraphicInfoFrame::InternalGetFrameForcedLoading(pInfoFrame, 1)) {
LAB_00be7375:
                pGraphicFrameData = *reinterpret_cast<CGraphicFrameData**>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) + 0x1C);
                goto LAB_00be737b;
            }
        } else {
            if (*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x1C) == 0) {
                goto LAB_00be735f;
            }

            *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x3C) =
                *reinterpret_cast<std::uint32_t*>(*reinterpret_cast<int*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x0C) + 0x20) + 0x194);

            CResource* pResource = *reinterpret_cast<CResource**>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) + 0x24);
            if ((pResource != nullptr) &&
                (*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) != 0) &&
                (*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) !=
                 *reinterpret_cast<int*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C))) {
                CResource::ListRemove(pResource);
                CResource::ListInsert(
                    pResource,
                    *reinterpret_cast<CResource**>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x10),
                    reinterpret_cast<CResource*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 4));
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) =
                    *reinterpret_cast<std::uint32_t*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C);
            }

            pResource = *reinterpret_cast<CResource**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08);
            if ((*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) != 0) &&
                (*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) !=
                 *reinterpret_cast<int*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C))) {
                CResource::ListRemove(pResource);
                CResource::ListInsert(
                    pResource,
                    *reinterpret_cast<CResource**>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x10),
                    reinterpret_cast<CResource*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 4));
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x18) =
                    *reinterpret_cast<std::uint32_t*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pResource) + 0x08) + 0x2C);
            }

            if (*reinterpret_cast<char*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) + 0x3A) != '\0') {
                *reinterpret_cast<std::uint8_t*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08) + 0x3A) = 0;
            }

            pFrameData = *reinterpret_cast<CFrameData**>(reinterpret_cast<std::uint8_t*>(pInfoFrame) + 0x08);
            if (*reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x38) != 0) {
                CGraphicInfoFrame::CFrameData::BeginLoading(pFrameData, 0, 1);
                goto LAB_00be7375;
            }
            pGraphicFrameData = *reinterpret_cast<CGraphicFrameData**>(reinterpret_cast<std::uint8_t*>(pFrameData) + 0x1C);
LAB_00be737b:
            if (pGraphicFrameData != nullptr) {
                *reinterpret_cast<int*>(pGraphicFrameData) = *reinterpret_cast<int*>(pGraphicFrameData) + 1;
            }
        }

        {
            CRenderManagerCore* mgr = DAT_01436e18;
            int value = DAT_01396dc8;
            auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x2EE4;
            if (*reinterpret_cast<int*>(state + 0x04) != value) {
                if ((*reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) &
                     *reinterpret_cast<unsigned int*>(state + 0x08)) == 0) {
                    *reinterpret_cast<unsigned int*>(state + 0x08) =
                        *reinterpret_cast<unsigned int*>(state + 0x08) |
                        *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814);
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                        *reinterpret_cast<std::uint32_t*>(state + 0x04);
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
                }
                *reinterpret_cast<int*>(state + 0x04) = value;
                if (*(state + 0x10) == 0) {
                    *(state + 0x10) = 1;
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
                }
            }
        }
        {
            CRenderManagerCore* mgr = DAT_01436e18;
            int value = DAT_01396dc8;
            auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x2F84;
            if (*reinterpret_cast<int*>(state + 0x04) != value) {
                if ((*reinterpret_cast<unsigned int*>(state + 0x08) &
                     *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814)) == 0) {
                    *reinterpret_cast<unsigned int*>(state + 0x08) =
                        *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) |
                        *reinterpret_cast<unsigned int*>(state + 0x08);
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                        *reinterpret_cast<std::uint32_t*>(state + 0x04);
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
                }
                *reinterpret_cast<int*>(state + 0x04) = value;
                if (*(state + 0x10) == 0) {
                    *(state + 0x10) = 1;
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
                }
            }
        }

        *reinterpret_cast<int*>(pGraphicFrameData) = *reinterpret_cast<int*>(pGraphicFrameData) - 1;
        if (*reinterpret_cast<int*>(pGraphicFrameData) == 0) {
            if ((*reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x1A) & 0x10) != 0) {
                CGraphicFrameData::Uninitialise(pGraphicFrameData);
            }

            int* piVar14 = *reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C);
            if (piVar14 != nullptr) {
                *piVar14 = *piVar14 - 1;
                if (**reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C) == 0) {
                    reinterpret_cast<void(*)()>((*reinterpret_cast<int**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C))[1])();
                    operator_delete(*reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C));
                }
            }

            *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x08) = 0;
            *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(pGraphicFrameData) + 0x0C) = 0;
            operator_delete(pGraphicFrameData);
        }
    }

    if (bUseSpecialShaderPath) {
        CShaderRenderManager::ApplyVertexShader(
            DAT_01436e14,
            reinterpret_cast<CVertexShader*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x4CC),
            false);

        {
            CRenderManagerCore* mgr = DAT_01436e18;
            int value = DAT_01396f78;
            auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x291C;
            if (*reinterpret_cast<int*>(state + 0x04) != value) {
                if ((*reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) &
                     *reinterpret_cast<unsigned int*>(state + 0x08)) == 0) {
                    *reinterpret_cast<unsigned int*>(state + 0x08) =
                        *reinterpret_cast<unsigned int*>(state + 0x08) |
                        *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814);
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                        *reinterpret_cast<std::uint32_t*>(state + 0x04);
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
                }
                *reinterpret_cast<int*>(state + 0x04) = value;
                if (*(state + 0x10) == 0) {
                    *(state + 0x10) = 1;
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
                }
            }
        }
        {
            CRenderManagerCore* mgr = DAT_01436e18;
            int value = DAT_01396f68;
            auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x2930;
            if (*reinterpret_cast<int*>(state + 0x04) != value) {
                if ((*reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) &
                     *reinterpret_cast<unsigned int*>(state + 0x08)) == 0) {
                    *reinterpret_cast<unsigned int*>(state + 0x08) =
                        *reinterpret_cast<unsigned int*>(state + 0x08) |
                        *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814);
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                        *reinterpret_cast<std::uint32_t*>(state + 0x04);
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
                }
                *reinterpret_cast<int*>(state + 0x04) = value;
                if (*(state + 0x10) == 0) {
                    *(state + 0x10) = 1;
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
                }
            }
        }
        {
            CRenderManagerCore* mgr = DAT_01436e18;
            auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x28B8;
            if (*reinterpret_cast<int*>(state + 0x04) != 1) {
                if ((*reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) &
                     *reinterpret_cast<unsigned int*>(state + 0x08)) == 0) {
                    *reinterpret_cast<unsigned int*>(state + 0x08) =
                        *reinterpret_cast<unsigned int*>(state + 0x08) |
                        *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814);
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                    *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                        *reinterpret_cast<std::uint32_t*>(state + 0x04);
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
                }
                *reinterpret_cast<std::uint32_t*>(state + 0x04) = 1;
                if (*(state + 0x10) == 0) {
                    *(state + 0x10) = 1;
                    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                        *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
                }
            }
        }
    } else {
        CShaderRenderManager::ApplyVertexShader(
            DAT_01436e14,
            reinterpret_cast<CVertexShader*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x4C4),
            false);
    }

    CShaderRenderManager::SetUserVertexShaderConstant(
        DAT_01436e14,
        0,
        reinterpret_cast<C4DVector*>(reinterpret_cast<std::uint8_t*>(DAT_01436ea8) + 0x62C));

    std::uint32_t c1[4];
    c1[0] = 0;
    c1[1] = 0;
    c1[2] = 0;
    c1[3] = 0x3F800000;
    CShaderRenderManager::SetUserVertexShaderConstant(DAT_01436e14, 1, reinterpret_cast<C4DVector*>(c1));

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

    {
        CRenderManagerCore* mgr = DAT_01436e18;
        auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x29E4;
        if (*reinterpret_cast<int*>(state + 0x04) != 0) {
            if ((*reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) &
                 *reinterpret_cast<unsigned int*>(state + 0x08)) == 0) {
                *reinterpret_cast<unsigned int*>(state + 0x08) =
                    *reinterpret_cast<unsigned int*>(state + 0x08) |
                    *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814);
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                    *reinterpret_cast<std::uint32_t*>(state + 0x04);
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
            }
            *reinterpret_cast<std::uint32_t*>(state + 0x04) = 0;
            if (*(state + 0x10) == 0) {
                *(state + 0x10) = 1;
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
            }
        }
    }
    {
        CRenderManagerCore* mgr = DAT_01436e18;
        auto* state = reinterpret_cast<std::uint8_t*>(mgr) + 0x2854;
        if (*reinterpret_cast<int*>(state + 0x04) != 1) {
            if ((*reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814) &
                 *reinterpret_cast<unsigned int*>(state + 0x08)) == 0) {
                *reinterpret_cast<unsigned int*>(state + 0x08) =
                    *reinterpret_cast<unsigned int*>(state + 0x08) |
                    *reinterpret_cast<unsigned int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2814);
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 8) = state;
                *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) * 8 + 0x0C) =
                    *reinterpret_cast<std::uint32_t*>(state + 0x04);
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x2808) + 1;
            }
            *reinterpret_cast<std::uint32_t*>(state + 0x04) = 1;
            if (*(state + 0x10) == 0) {
                *(state + 0x10) = 1;
                *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(mgr) +
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) * 4 + 0x2008) = state;
                *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) =
                    *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(mgr) + 0x280C) + 1;
            }
        }
    }

    std::uint8_t resultAlpha;
    if (m_pBaseTexture == nullptr) {
        resultAlpha = 0;
    } else {
        resultAlpha = static_cast<std::uint8_t>(static_cast<std::uint32_t>(*reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(m_pBaseTexture) + 4)) >> 24);
    }

    auto* const device = *reinterpret_cast<void***>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF0);
    reinterpret_cast<void(__thiscall*)(void*, int)>((*reinterpret_cast<void***>(device))[100])(device, 0);

    *reinterpret_cast<void**>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF4) = m_pStaticIndexBuffer;
    std::uint32_t indexHandle;
    if (m_pStaticIndexBuffer == nullptr) {
        indexHandle = 0;
    } else {
        indexHandle = *reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(m_pStaticIndexBuffer) + 4);
    }
    reinterpret_cast<void(__thiscall*)(void*, std::uint32_t)>((*reinterpret_cast<void***>(device))[104])(device, indexHandle);

    CShaderRenderManager* const pShaderRenderManager = DAT_01436e14;
    auto* const shaderFlags = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 0x3D8);

    if ((*reinterpret_cast<std::uint8_t*>(shaderFlags) & 4) != 0) {
        const int ctx = *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 4);
        int* piVar14 = *reinterpret_cast<int**>(ctx + 0x140);
        if (((*reinterpret_cast<int*>(ctx + 0x144) - reinterpret_cast<int>(piVar14)) >> 4) == 0) {
            piVar14 = reinterpret_cast<int*>(ctx + 0x140);
        }
        CShaderRenderManager::SetVSConstants(
            DAT_01436e14,
            *reinterpret_cast<long long*>(*reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(DAT_01436e14) + 4) + 8),
            piVar14,
            (*reinterpret_cast<int*>(ctx + 0x144) - *reinterpret_cast<int*>(ctx + 0x140)) >> 4);
    }
    if ((*reinterpret_cast<std::uint8_t*>(shaderFlags) & 1) != 0) {
        CShaderRenderManager::UpdatePixelShader(pShaderRenderManager);
    }
    if ((*reinterpret_cast<std::uint8_t*>(shaderFlags) & 2) != 0) {
        CShaderRenderManager::UpdatePixelShaderConstants(pShaderRenderManager);
    }
    if ((*shaderFlags & 0x2000) != 0) {
        CShaderRenderManager::UpdateWorldSpaceFogTransform(pShaderRenderManager);
    }
    if ((*reinterpret_cast<std::uint8_t*>(shaderFlags) & 4) != 0) {
        CShaderRenderManager::UpdateAmbient(pShaderRenderManager);
    }
    if ((*shaderFlags & 0x20000) != 0) {
        CShaderRenderManager::UpdateFogColour(pShaderRenderManager);
    }
    if ((*shaderFlags & 0x400) != 0) {
        CShaderRenderManager::UpdateLightGlobals(pShaderRenderManager);
    }
    if ((*shaderFlags & 0x200) != 0) {
        CShaderRenderManager::UpdateWorldSpaceLighting(pShaderRenderManager);
    }
    if (static_cast<char>(*reinterpret_cast<std::uint8_t*>(shaderFlags)) < 0) {
        CShaderRenderManager::UpdateCombinedProjectionTransform(pShaderRenderManager);
    }
    if ((*reinterpret_cast<std::uint8_t*>(shaderFlags) & 8) != 0) {
        CShaderRenderManager::UpdateAmbient(pShaderRenderManager);
    }

    CTexture* pTexture = nullptr;
    CRenderManagerCore::DrawCurrentBuffersIndexed(
        DAT_01436e18,
        &pTexture,
        1,
        0,
        static_cast<std::uint32_t>(m_IndexCount),
        0,
        static_cast<std::uint32_t>(m_VertexCount));

    void** pLayerVB = &m_pLayerVertexBuffer0;
    std::uint16_t* pLayerCount = &m_LayerIndexCount0;
    int i = 4;
    do {
        int layerVB = reinterpret_cast<int>(*pLayerVB);
        if (layerVB != 0) {
            reinterpret_cast<void(__thiscall*)(void*, int, std::uint32_t, int, std::uint8_t)>((*reinterpret_cast<void***>(device))[100])(
                device,
                0,
                *reinterpret_cast<std::uint32_t*>(layerVB + 4),
                0,
                *reinterpret_cast<std::uint8_t*>(layerVB + 0x10));

            int layerIB = reinterpret_cast<int>(*(pLayerVB - 4));
            *reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(DAT_01436e18) + 0x3CF4) = layerIB;
            if (layerIB == 0) {
                indexHandle = 0;
            } else {
                indexHandle = *reinterpret_cast<std::uint32_t*>(layerIB + 4);
            }
            reinterpret_cast<void(__thiscall*)(void*, std::uint32_t)>((*reinterpret_cast<void***>(device))[104])(device, indexHandle);

            CRenderManagerCore::DrawCurrentBuffersAsIndexedTris(
                DAT_01436e18,
                &pTexture,
                1,
                0,
                *reinterpret_cast<int*>(layerVB + 8) / static_cast<int>(static_cast<std::uint32_t>(*reinterpret_cast<std::uint8_t*>(layerVB + 0x10))),
                0,
                static_cast<std::uint32_t>(*pLayerCount));
        }
        pLayerCount = pLayerCount + 1;
        pLayerVB = pLayerVB + 1;
        i = i - 1;
    } while (i != 0);

    CRenderStateManager::RestoreCaptureBlock(DAT_013cb508);
    return resultAlpha != 0;
}