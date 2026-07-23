bool CLandscapeBackgroundPatch::Load(CDataInputStream& in, float minZ, float maxZ)
{
    struct CDataInputStreamOverlay
    {
        std::byte pad00[0x04];
        std::uint32_t m_uBytesRead;    // 0x04
        std::byte pad08[0x04];
        std::uint8_t* m_pSrcCursor;    // 0x0C
        std::byte pad10[0x04];
        int m_nSrcBytesRemaining;      // 0x14
        int m_nSourceChunkBaseAddress; // 0x18
    };

    struct CLandscapeBackgroundTreeNodeOverlay
    {
        std::byte pad00[0xA4];
        CEngineLandscapeMap* m_pLandscapeMap; // 0xA4
    };

    struct CEngineLandscapeMapOverlay
    {
        std::byte pad00[0x14];
        void* m_pRuntime14; // 0x14
    };

    struct LandscapeMapRuntimeOverlay
    {
        std::byte pad00[0x60];
        std::uint16_t m_uBaseMapX; // 0x60
        std::uint16_t m_uBaseMapY; // 0x62
    };

    struct TVertexLandscapeBackgroundOverlay
    {
        std::uint16_t m_uX;     // 0x00
        std::uint16_t m_uY;     // 0x02
        std::uint32_t m_uZ;     // 0x04
        std::byte pad08[0x09];
        std::uint8_t m_uByte11; // 0x11
        std::uint8_t m_uByte12; // 0x12
        std::byte pad13[0x05];
    };

    struct OpaqueBoolCall32
    {
        std::uint32_t raw;
    };

    static_assert(offsetof(CDataInputStreamOverlay, m_uBytesRead) == 0x04);
    static_assert(offsetof(CDataInputStreamOverlay, m_pSrcCursor) == 0x0C);
    static_assert(offsetof(CDataInputStreamOverlay, m_nSrcBytesRemaining) == 0x14);
    static_assert(offsetof(CDataInputStreamOverlay, m_nSourceChunkBaseAddress) == 0x18);

    static_assert(offsetof(CLandscapeBackgroundPatch, Parent) == 0x04);
    static_assert(offsetof(CLandscapeBackgroundPatch, EdgeStrips) == 0x08);
    static_assert(offsetof(CLandscapeBackgroundPatch, Texture) == 0xB8);
    static_assert(offsetof(CLandscapeBackgroundPatch, IndexBuffer) == 0xBC);
    static_assert(offsetof(CLandscapeBackgroundPatch, VertexBuffer) == 0xC0);
    static_assert(offsetof(CLandscapeBackgroundPatch, WaterSubPatch) == 0xF4);
    static_assert(offsetof(CLandscapeBackgroundPatch, MinZ) == 0xF8);
    static_assert(offsetof(CLandscapeBackgroundPatch, MaxZ) == 0xFC);
    static_assert(offsetof(CLandscapeBackgroundPatch, VertexCount) == 0x104);
    static_assert(offsetof(CLandscapeBackgroundPatch, PolyCountIncDegenerates) == 0x106);
    static_assert(offsetof(CLandscapeBackgroundPatch, TextureWidthNeeded) == 0x108);
    static_assert(offsetof(CLandscapeBackgroundPatch, TextureHeightNeeded) == 0x10A);
    static_assert(offsetof(CLandscapeBackgroundPatch, MapX) == 0x10C);
    static_assert(offsetof(CLandscapeBackgroundPatch, MapY) == 0x10E);
    static_assert(offsetof(CLandscapeBackgroundPatch, Width) == 0x110);
    static_assert(offsetof(CLandscapeBackgroundPatch, Height) == 0x112);
    static_assert(offsetof(CLandscapeBackgroundPatch, LODBand) == 0x118);
    static_assert(offsetof(CLandscapeBackgroundPatch, SharedIndexBuffer) == 0x119);
    static_assert(offsetof(CLandscapeBackgroundPatch, WaterOnlyPatch) == 0x11A);
    static_assert(offsetof(CLandscapeBackgroundPatch, IsDefaultSection) == 0x11B);

    static_assert(offsetof(CLandscapeBackgroundTreeNodeOverlay, m_pLandscapeMap) == 0xA4);
    static_assert(offsetof(CEngineLandscapeMapOverlay, m_pRuntime14) == 0x14);
    static_assert(offsetof(LandscapeMapRuntimeOverlay, m_uBaseMapX) == 0x60);
    static_assert(offsetof(LandscapeMapRuntimeOverlay, m_uBaseMapY) == 0x62);
    static_assert(offsetof(TVertexLandscapeBackgroundOverlay, m_uByte11) == 0x11);
    static_assert(offsetof(TVertexLandscapeBackgroundOverlay, m_uByte12) == 0x12);
    static_assert(sizeof(TVertexLandscapeBackgroundOverlay) == 0x18);

    auto& stream = reinterpret_cast<CDataInputStreamOverlay&>(in);
    auto* const landscapeMemoryManager =
        *reinterpret_cast<CEngineLandscapeMemoryManager**>(DAT_01436ea8 + 0x6B0);

    const auto readU8 = [&](std::uint8_t& value) -> void
    {
        if (stream.m_uBytesRead + 1U < 0x80000000U)
        {
            if (stream.m_nSrcBytesRemaining < 1)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&value), 1);
            }
            else
            {
                value = *stream.m_pSrcCursor;
                ++stream.m_pSrcCursor;
                --stream.m_nSrcBytesRemaining;
                ++stream.m_uBytesRead;
            }
        }
    };

    const auto readU16 = [&](std::uint16_t& value) -> void
    {
        if (stream.m_uBytesRead + 2U < 0x80000000U)
        {
            if (stream.m_nSrcBytesRemaining < 2)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&value), 2);
            }
            else
            {
                value = *reinterpret_cast<std::uint16_t*>(stream.m_pSrcCursor);
                stream.m_pSrcCursor += 2;
                stream.m_nSrcBytesRemaining -= 2;
                stream.m_uBytesRead += 2;
            }
        }
    };

    const auto readU32 = [&](std::uint32_t& value) -> void
    {
        if (stream.m_uBytesRead + 4U < 0x80000000U)
        {
            if (stream.m_nSrcBytesRemaining < 4)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&value), 4);
            }
            else
            {
                value = *reinterpret_cast<std::uint32_t*>(stream.m_pSrcCursor);
                stream.m_pSrcCursor += 4;
                stream.m_nSrcBytesRemaining -= 4;
                stream.m_uBytesRead += 4;
            }
        }
    };

    const auto readF32 = [&](float& value) -> void
    {
        if (stream.m_uBytesRead + 4U < 0x80000000U)
        {
            if (stream.m_nSrcBytesRemaining < 4)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(&value), 4);
            }
            else
            {
                value = *reinterpret_cast<float*>(stream.m_pSrcCursor);
                stream.m_pSrcCursor += 4;
                stream.m_nSrcBytesRemaining -= 4;
                stream.m_uBytesRead += 4;
            }
        }
    };

    const auto callVertexBufferOpaque = [&](CThingFilter_IsValid* object, CThing* thing) -> OpaqueBoolCall32
    {
        OpaqueBoolCall32 result{};
        result.raw = static_cast<std::uint8_t>(
            CThingFilter_IsValid::operator()(object, thing));
        return result;
    };

    MaxZ = maxZ;
    MinZ = minZ;

    readU16(Width);
    readU16(Height);
    readU16(MapX);
    readU16(MapY);

    {
        std::uint8_t value = 0;
        readU8(value);
        WaterOnlyPatch = (value != 0);
    }

    {
        std::uint8_t value = 0;
        readU8(value);
        LODBand = value;
    }

    if (!WaterOnlyPatch)
    {
        readU16(PolyCountIncDegenerates);
        readU16(VertexCount);

        {
            std::uint8_t value = 0;
            readU8(value);
            TextureWidthNeeded = static_cast<std::uint16_t>(value);
        }

        {
            std::uint8_t value = 0;
            readU8(value);
            TextureHeightNeeded = static_cast<std::uint16_t>(value);
        }

        ulong local_4c[2]{0xFFFFFFFFUL, 0UL};

        {
            std::uint8_t value = 0;
            readU8(value);
            if (value == 0)
            {
                CDisplayManager::GetBestAlphaTextureFormat(
                    DAT_01436e1c,
                    0x10,
                    reinterpret_cast<CPixelFormat*>(local_4c));
            }
            else
            {
                CPixelFormat::Initialise(reinterpret_cast<CPixelFormat*>(local_4c), 0x31545844);
            }
        }

        std::uint32_t local_44 = static_cast<std::uint32_t>(TextureWidthNeeded);
        auto* local_40 =
            reinterpret_cast<CEngineLandscapeMemoryManager*>(static_cast<std::uintptr_t>(TextureHeightNeeded));

        Texture = CEngineLandscapeMemoryManager::AllocateTexture(
            landscapeMemoryManager,
            reinterpret_cast<CMovableResource*>(this),
            reinterpret_cast<C2DExtentsI*>(&local_44),
            1,
            reinterpret_cast<CPixelFormat*>(local_4c),
            0,
            false);
        if (Texture == nullptr)
        {
            return false;
        }

        CTexture::LoadFromDataStreamToPreallocatedSurface(Texture, &in);

        {
            const ulong usage = CEngineResourceManager::GetIndexBufferUsage(DAT_01436e98);
            VertexBuffer = CEngineLandscapeMemoryManager::AllocateVertexBuffer(
                landscapeMemoryManager,
                reinterpret_cast<CMovableResource*>(this),
                0,
                static_cast<std::uint32_t>(VertexCount),
                0x18,
                usage);
        }
        if (VertexBuffer == nullptr)
        {
            return false;
        }

        float compressedVertexByteCount = 0.0f;
        readF32(compressedVertexByteCount);

        CDataInputStream* local_38 = nullptr;
        CDataInputStream* local_34 = nullptr;
        CDataInputStream* local_30 = nullptr;
        void* pvStack_2c = nullptr;
        std::uint8_t* local_28 = nullptr;
        std::uint8_t* local_24 = nullptr;
        std::uint32_t uStack_20 = 0;
        CMemoryDataInputStream local_1c[4]{};
        ulong* puStack_18 = nullptr;
        int iStack_10 = 0;

        std__vector___Init(static_cast<std::uint32_t>(VertexCount) << 4);
        std__vector___Init(compressedVertexByteCount);

        {
            auto* dst = local_34;
            if (local_30 == local_34)
            {
                dst = reinterpret_cast<CDataInputStream*>(&local_34);
            }

            const int byteCount = static_cast<int>(compressedVertexByteCount);
            if ((0 < byteCount) &&
                (static_cast<std::uint32_t>(static_cast<int>(stream.m_uBytesRead) + byteCount) < 0x80000000U))
            {
                if (stream.m_nSrcBytesRemaining < byteCount)
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(dst), byteCount);
                }
                else
                {
                    auto* src = reinterpret_cast<CDataInputStream*>(stream.m_pSrcCursor);
                    auto* write = dst;

                    for (std::uint32_t count = static_cast<std::uint32_t>(byteCount) >> 2; count != 0; --count)
                    {
                        *reinterpret_cast<std::uint32_t*>(write) = *reinterpret_cast<std::uint32_t*>(src);
                        write = reinterpret_cast<CDataInputStream*>(reinterpret_cast<std::uint8_t*>(write) + 4);
                        src = reinterpret_cast<CDataInputStream*>(reinterpret_cast<std::uint8_t*>(src) + 4);
                    }

                    for (std::uint32_t count = static_cast<std::uint32_t>(byteCount) & 3U; count != 0; --count)
                    {
                        *reinterpret_cast<std::uint8_t*>(write) = *reinterpret_cast<std::uint8_t*>(src);
                        write = reinterpret_cast<CDataInputStream*>(reinterpret_cast<std::uint8_t*>(write) + 1);
                        src = reinterpret_cast<CDataInputStream*>(reinterpret_cast<std::uint8_t*>(src) + 1);
                    }

                    stream.m_pSrcCursor += byteCount;
                    stream.m_nSrcBytesRemaining -= byteCount;
                    stream.m_uBytesRead += byteCount;
                }
            }
        }

        {
            void* decompressedWrite = local_28;
            if (local_24 == local_28)
            {
                decompressedWrite = &local_28;
            }

            auto* compressedRead = local_34;
            if (local_30 == local_34)
            {
                compressedRead = reinterpret_cast<CDataInputStream*>(&local_34);
            }

            CRangeCompressor::Decompress(
                reinterpret_cast<CRangeCompressor*>(&local_38),
                reinterpret_cast<uchar*>(compressedRead),
                static_cast<std::uint32_t>(VertexCount),
                0x10,
                decompressedWrite);
        }

        {
            const ulong decompressedSize =
                static_cast<ulong>(reinterpret_cast<std::uintptr_t>(local_24) -
                                   reinterpret_cast<std::uintptr_t>(local_28));
            if (decompressedSize == 0)
            {
                local_28 = reinterpret_cast<std::uint8_t*>(&local_28);
            }

            CMemoryDataInputStream::CMemoryDataInputStream(local_1c, local_28, decompressedSize);

            auto& vertexStream = reinterpret_cast<CDataInputStreamOverlay&>(local_1c[0]);
            puStack_18 = reinterpret_cast<ulong*>(vertexStream.m_pSrcCursor);
            iStack_10 = vertexStream.m_nSrcBytesRemaining;
            uStack_20 = vertexStream.m_uBytesRead;

            const auto readVertexU16 = [&](std::uint16_t& value) -> void
            {
                if (uStack_20 + 2U < 0x80000000U)
                {
                    if (iStack_10 < 2)
                    {
                        CDataInputStream::ReadWithSrcChunkOverflow(
                            reinterpret_cast<CDataInputStream*>(&local_24),
                            reinterpret_cast<uchar*>(&value),
                            2);
                        puStack_18 = reinterpret_cast<ulong*>(vertexStream.m_pSrcCursor);
                        iStack_10 = vertexStream.m_nSrcBytesRemaining;
                        uStack_20 = vertexStream.m_uBytesRead;
                    }
                    else
                    {
                        value = static_cast<std::uint16_t>(*puStack_18);
                        puStack_18 = reinterpret_cast<ulong*>(reinterpret_cast<std::uint8_t*>(puStack_18) + 2);
                        iStack_10 -= 2;
                        uStack_20 += 2;
                        vertexStream.m_pSrcCursor = reinterpret_cast<std::uint8_t*>(puStack_18);
                        vertexStream.m_nSrcBytesRemaining = iStack_10;
                        vertexStream.m_uBytesRead = uStack_20;
                    }
                }
            };

            const auto readVertexU32 = [&](std::uint32_t& value) -> void
            {
                if (uStack_20 + 4U < 0x80000000U)
                {
                    if (iStack_10 < 4)
                    {
                        CDataInputStream::ReadWithSrcChunkOverflow(
                            reinterpret_cast<CDataInputStream*>(&local_24),
                            reinterpret_cast<uchar*>(&value),
                            4);
                        puStack_18 = reinterpret_cast<ulong*>(vertexStream.m_pSrcCursor);
                        iStack_10 = vertexStream.m_nSrcBytesRemaining;
                        uStack_20 = vertexStream.m_uBytesRead;
                    }
                    else
                    {
                        value = static_cast<std::uint32_t>(*puStack_18);
                        ++puStack_18;
                        iStack_10 -= 4;
                        uStack_20 += 4;
                        vertexStream.m_pSrcCursor = reinterpret_cast<std::uint8_t*>(puStack_18);
                        vertexStream.m_nSrcBytesRemaining = iStack_10;
                        vertexStream.m_uBytesRead = uStack_20;
                    }
                }
            };

            const OpaqueBoolCall32 vertexLockResult =
                callVertexBufferOpaque(reinterpret_cast<CThingFilter_IsValid*>(VertexBuffer), static_cast<CThing*>(nullptr));
            auto* puVar12 = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(vertexLockResult.raw) + 4U);

            int iVar8 = 0;
            if (VertexCount != 0)
            {
                do
                {
                    auto* const vertex =
                        reinterpret_cast<TVertexLandscapeBackgroundOverlay*>(puVar12 - 1);

                    readVertexU16(vertex->m_uX);
                    readVertexU16(vertex->m_uY);
                    readVertexU32(vertex->m_uZ);

                    {
                        std::uint32_t packedNormal = 0;
                        readVertexU32(packedNormal);
                        SetTargetVertexNormal(
                            reinterpret_cast<CTVertexLandscapeBackground*>(vertex),
                            packedNormal);
                    }

                    {
                        std::uint16_t value = 0;
                        readVertexU16(value);
                        vertex->m_uByte12 = static_cast<std::uint8_t>(value);
                    }

                    {
                        std::uint16_t value = 0;
                        readVertexU16(value);
                        vertex->m_uByte11 = static_cast<std::uint8_t>(value);
                    }

                    ++iVar8;
                    puVar12 += 6;
                } while (iVar8 < static_cast<int>(VertexCount));
            }

            CThingFilter_IsValid::operator()(
                reinterpret_cast<CThingFilter_IsValid*>(VertexBuffer),
                static_cast<CThing*>(nullptr));

            SharedIndexBuffer = (LODBand == 1);
            if (LODBand == 1)
            {
                IndexBuffer =
                    *reinterpret_cast<CIndexBuffer**>(
                        DAT_01436ea8 + 0x14 +
                        ((static_cast<std::uint32_t>(Width) * 0x11U) +
                         static_cast<std::uint32_t>(Height)) * 4U);
            }
            else
            {
                const EPrimitiveType primitiveType = static_cast<EPrimitiveType>(4);
                const EIndexBufferFormat indexFormat = static_cast<EIndexBufferFormat>(0x65);
                const ulong usage = CEngineResourceManager::GetIndexBufferUsage(DAT_01436e98);

                IndexBuffer = CEngineLandscapeMemoryManager::AllocateIndexBuffer(
                    local_40,
                    reinterpret_cast<CMovableResource*>(this),
                    static_cast<std::uint32_t>(PolyCountIncDegenerates) * 6U,
                    usage,
                    indexFormat,
                    primitiveType);
                if (IndexBuffer == nullptr)
                {
                    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&uStack_20));
                    if (local_38 != nullptr)
                    {
                        free(local_38);
                    }
                    if (pvStack_2c == nullptr)
                    {
                        return false;
                    }
                    free(pvStack_2c);
                    return false;
                }

                void* const lockedIndexData = ::CIndexBuffer::Lock(IndexBuffer, 0, 0, 0);

                std::uint32_t compressedIndexBytes = 0;
                readU32(compressedIndexBytes);

                {
                    std::uint32_t maskedIndexBytes = compressedIndexBytes & 0xFFFFFF00U;
                    if (reinterpret_cast<CDataInputStream*>(compressedIndexBytes) <
                        reinterpret_cast<CDataInputStream*>(
                            reinterpret_cast<std::uintptr_t>(local_34) -
                            reinterpret_cast<std::uintptr_t>(local_38)))
                    {
                        local_34 = reinterpret_cast<CDataInputStream*>(
                            reinterpret_cast<std::uintptr_t>(local_38) + compressedIndexBytes);
                    }
                    else
                    {
                        std__deque__insert(
                            local_34,
                            reinterpret_cast<CDataInputStream*>(
                                reinterpret_cast<std::uintptr_t>(local_38) +
                                (compressedIndexBytes -
                                 (reinterpret_cast<std::uintptr_t>(local_34) -
                                  reinterpret_cast<std::uintptr_t>(local_38)))),
                            &maskedIndexBytes);
                    }
                }

                {
                    auto* dst = local_38;
                    if (local_34 == local_38)
                    {
                        dst = reinterpret_cast<CDataInputStream*>(&local_38);
                    }

                    const int byteCount = static_cast<int>(compressedIndexBytes);
                    if ((0 < byteCount) &&
                        (static_cast<std::uint32_t>(byteCount + static_cast<int>(stream.m_uBytesRead)) < 0x80000000U))
                    {
                        if (stream.m_nSrcBytesRemaining < byteCount)
                        {
                            CDataInputStream::ReadWithSrcChunkOverflow(&in, reinterpret_cast<uchar*>(dst), byteCount);
                        }
                        else
                        {
                            auto* src = reinterpret_cast<CDataInputStream*>(stream.m_pSrcCursor);
                            auto* write = dst;

                            for (std::uint32_t count = compressedIndexBytes >> 2; count != 0; --count)
                            {
                                *reinterpret_cast<std::uint32_t*>(write) = *reinterpret_cast<std::uint32_t*>(src);
                                write = reinterpret_cast<CDataInputStream*>(reinterpret_cast<std::uint8_t*>(write) + 4);
                                src = reinterpret_cast<CDataInputStream*>(reinterpret_cast<std::uint8_t*>(src) + 4);
                            }

                            for (std::uint32_t count = compressedIndexBytes & 3U; count != 0; --count)
                            {
                                *reinterpret_cast<std::uint8_t*>(write) = *reinterpret_cast<std::uint8_t*>(src);
                                write = reinterpret_cast<CDataInputStream*>(reinterpret_cast<std::uint8_t*>(write) + 1);
                                src = reinterpret_cast<CDataInputStream*>(reinterpret_cast<std::uint8_t*>(src) + 1);
                            }

                            stream.m_pSrcCursor += byteCount;
                            stream.m_nSrcBytesRemaining -= byteCount;
                            stream.m_uBytesRead += byteCount;
                        }
                    }
                }

                {
                    auto* compressedRead = local_38;
                    if (local_34 == local_38)
                    {
                        compressedRead = reinterpret_cast<CDataInputStream*>(&local_38);
                    }

                    CRangeCompressor::Decompress(
                        reinterpret_cast<CRangeCompressor*>(&landscapeMemoryManager),
                        reinterpret_cast<uchar*>(compressedRead),
                        static_cast<std::uint32_t>(PolyCountIncDegenerates) * 3U,
                        2,
                        lockedIndexData);
                }

                CVertexBufferWin32::DoUnlock(reinterpret_cast<CVertexBufferWin32*>(IndexBuffer));
            }

            CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&uStack_20));
        }

        if (local_38 != nullptr)
        {
            free(local_38);
        }
        if (pvStack_2c != nullptr)
        {
            free(pvStack_2c);
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        if (!CPatchTesselationEdgeStrip::Load(&EdgeStrips[i], &in, reinterpret_cast<CMovableResource*>(this)))
        {
            return false;
        }
    }

    {
        std::uint8_t hasWaterSubPatch = 0;
        readU8(hasWaterSubPatch);

        if (static_cast<char>(hasWaterSubPatch) != '\0')
        {
            union
            {
                std::pair<unsigned long, CAnimationEntry>* in_stack_ffffff98;
                CLandscapeLayerMesh* this_04;
            } waterCtorTemp{};

            waterCtorTemp.in_stack_ffffff98 = nullptr;

            std::_Cons_val<
                std::allocator<std::pair<unsigned long, CAnimationEntry>_>,
                std::pair<unsigned long, CAnimationEntry>,
                std::pair<unsigned long, CAnimationEntry>&>(
                    reinterpret_cast<std::allocator<std::pair<unsigned long, CAnimationEntry>_>*>(0x60),
                    reinterpret_cast<std::pair<unsigned long, CAnimationEntry>*>(this),
                    waterCtorTemp.in_stack_ffffff98);

            CEngineWaterBackgroundSubPatch* this_05 = nullptr;
            if (waterCtorTemp.this_04 == nullptr)
            {
                this_05 = nullptr;
            }
            else
            {
                auto* const parentOverlay =
                    reinterpret_cast<CLandscapeBackgroundTreeNodeOverlay*>(Parent);
                this_05 = reinterpret_cast<CEngineWaterBackgroundSubPatch*>(
                    CLandscapeLayerMesh::CLandscapeLayerMesh(
                        waterCtorTemp.this_04,
                        parentOverlay->m_pLandscapeMap));
            }

            WaterSubPatch = this_05;
            if (WaterSubPatch == nullptr)
            {
                return false;
            }

            auto* const parentOverlay =
                reinterpret_cast<CLandscapeBackgroundTreeNodeOverlay*>(Parent);
            auto* const mapOverlay =
                reinterpret_cast<CEngineLandscapeMapOverlay*>(parentOverlay->m_pLandscapeMap);
            auto* const runtimeOverlay =
                reinterpret_cast<LandscapeMapRuntimeOverlay*>(mapOverlay->m_pRuntime14);

            if (!CEngineWaterBackgroundSubPatch::Load(
                    WaterSubPatch,
                    &in,
                    Width,
                    Height,
                    static_cast<std::uint32_t>(runtimeOverlay->m_uBaseMapX) + static_cast<std::uint32_t>(MapX),
                    static_cast<std::uint32_t>(runtimeOverlay->m_uBaseMapY) + static_cast<std::uint32_t>(MapY),
                    MinZ,
                    MaxZ))
            {
                return false;
            }
        }
    }

    return true;
}