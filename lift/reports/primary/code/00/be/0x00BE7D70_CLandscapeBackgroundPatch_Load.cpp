bool CLandscapeBackgroundPatch::Load(CDataInputStream& stream, float worldX, float worldY)
{
    struct DataInputLayout
    {
        std::byte pad00[0x04];
        std::uint32_t readPos;      // 0x04
        std::byte pad08[0x04];
        unsigned char* cursor;      // 0x0C
        std::byte pad10[0x04];
        int srcChunkRemaining;      // 0x14
    };

    struct PatchLayout
    {
        std::byte pad00[0xB8];
        CTexture* texture;                                 // 0xB8
        CIndexBuffer* indexBuffer;                         // 0xBC
        CVertexBufferWin32* vertexBuffer;                  // 0xC0
        std::byte padC4[0x30];
        CEngineWaterBackgroundSubPatch* waterSubPatch;     // 0xF4
        float patchWorldX;                                 // 0xF8
        float patchWorldY;                                 // 0xFC
        std::byte pad100[0x04];
        std::uint16_t vertexCount;                         // 0x104
        std::uint16_t triangleCount;                       // 0x106
        std::uint16_t textureWidth;                        // 0x108
        std::uint16_t textureHeight;                       // 0x10A
        std::uint16_t waterOffsetX;                        // 0x10C
        std::uint16_t waterOffsetY;                        // 0x10E
        std::uint16_t patchX;                              // 0x110
        std::uint16_t patchY;                              // 0x112
        std::byte pad114[0x04];
        std::uint8_t indexMode;                            // 0x118
        bool usesSharedIndexBuffer;                        // 0x119
        bool skipEmbeddedGeometry;                         // 0x11A
    };

    struct TVertexLandscapeBackgroundLayout
    {
        std::uint16_t positionX;     // 0x00
        std::uint16_t positionY;     // 0x02
        std::uint32_t positionZ;     // 0x04
        std::byte normalAndPadding[0x0D];
        std::uint8_t byte15;         // 0x15
        std::uint8_t byte16;         // 0x16
        std::uint8_t byte17;         // 0x17
    };

    static_assert(offsetof(PatchLayout, texture) == 0xB8);
    static_assert(offsetof(PatchLayout, indexBuffer) == 0xBC);
    static_assert(offsetof(PatchLayout, vertexBuffer) == 0xC0);
    static_assert(offsetof(PatchLayout, waterSubPatch) == 0xF4);
    static_assert(offsetof(PatchLayout, patchWorldX) == 0xF8);
    static_assert(offsetof(PatchLayout, patchWorldY) == 0xFC);
    static_assert(offsetof(PatchLayout, vertexCount) == 0x104);
    static_assert(offsetof(PatchLayout, triangleCount) == 0x106);
    static_assert(offsetof(PatchLayout, textureWidth) == 0x108);
    static_assert(offsetof(PatchLayout, textureHeight) == 0x10A);
    static_assert(offsetof(PatchLayout, waterOffsetX) == 0x10C);
    static_assert(offsetof(PatchLayout, waterOffsetY) == 0x10E);
    static_assert(offsetof(PatchLayout, patchX) == 0x110);
    static_assert(offsetof(PatchLayout, patchY) == 0x112);
    static_assert(offsetof(PatchLayout, indexMode) == 0x118);
    static_assert(offsetof(PatchLayout, usesSharedIndexBuffer) == 0x119);
    static_assert(offsetof(PatchLayout, skipEmbeddedGeometry) == 0x11A);

    static_assert(offsetof(TVertexLandscapeBackgroundLayout, byte16) == 0x16);
    static_assert(offsetof(TVertexLandscapeBackgroundLayout, byte15) == 0x15);
    static_assert(sizeof(TVertexLandscapeBackgroundLayout) == 0x18);

    auto& s = reinterpret_cast<DataInputLayout&>(stream);
    auto& self = reinterpret_cast<PatchLayout&>(*this);

    auto readU8 = [](CDataInputStream& input) -> std::uint8_t
    {
        auto& l = reinterpret_cast<DataInputLayout&>(input);
        std::uint32_t value = 0;
        if (l.readPos + 1U < 0x80000000U)
        {
            if (l.srcChunkRemaining < 1)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&input, reinterpret_cast<uchar*>(&value), 1);
            }
            else
            {
                value = (value & 0xFFFFFF00U) | *l.cursor;
                l.cursor = l.cursor + 1;
                l.srcChunkRemaining = l.srcChunkRemaining - 1;
                l.readPos = l.readPos + 1;
            }
        }
        return static_cast<std::uint8_t>(value);
    };

    auto readU16 = [](CDataInputStream& input) -> std::uint16_t
    {
        auto& l = reinterpret_cast<DataInputLayout&>(input);
        std::uint32_t value = 0;
        if (l.readPos + 2U < 0x80000000U)
        {
            if (l.srcChunkRemaining < 2)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&input, reinterpret_cast<uchar*>(&value), 2);
            }
            else
            {
                value = (value & 0xFFFF0000U) | *reinterpret_cast<std::uint16_t*>(l.cursor);
                l.cursor = l.cursor + 2;
                l.srcChunkRemaining = l.srcChunkRemaining - 2;
                l.readPos = l.readPos + 2;
            }
        }
        return static_cast<std::uint16_t>(value);
    };

    auto readU32 = [](CDataInputStream& input) -> std::uint32_t
    {
        auto& l = reinterpret_cast<DataInputLayout&>(input);
        std::uint32_t value = 0;
        if (l.readPos + 4U < 0x80000000U)
        {
            if (l.srcChunkRemaining < 4)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&input, reinterpret_cast<uchar*>(&value), 4);
            }
            else
            {
                value = *reinterpret_cast<std::uint32_t*>(l.cursor);
                l.cursor = l.cursor + 4;
                l.srcChunkRemaining = l.srcChunkRemaining - 4;
                l.readPos = l.readPos + 4;
            }
        }
        return value;
    };

    auto readF32 = [](CDataInputStream& input) -> float
    {
        auto& l = reinterpret_cast<DataInputLayout&>(input);
        float value = 0.0f;
        if (l.readPos + 4U < 0x80000000U)
        {
            if (l.srcChunkRemaining < 4)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(&input, reinterpret_cast<uchar*>(&value), 4);
            }
            else
            {
                value = *reinterpret_cast<float*>(l.cursor);
                l.cursor = l.cursor + 4;
                l.srcChunkRemaining = l.srcChunkRemaining - 4;
                l.readPos = l.readPos + 4;
            }
        }
        return value;
    };

    CEngineLandscapeMemoryManager* const landscapeMemoryManager =
        *reinterpret_cast<CEngineLandscapeMemoryManager**>(DAT_01436ea8 + 0x6B0);

    self.patchWorldY = worldY;
    self.patchWorldX = worldX;

    self.patchX = readU16(stream);
    self.patchY = readU16(stream);
    self.waterOffsetX = readU16(stream);
    self.waterOffsetY = readU16(stream);
    self.skipEmbeddedGeometry = readU8(stream) != 0;
    self.indexMode = readU8(stream);

    if (!self.skipEmbeddedGeometry)
    {
        self.triangleCount = readU16(stream);
        self.vertexCount = readU16(stream);
        self.textureWidth = static_cast<std::uint16_t>(readU8(stream));
        self.textureHeight = static_cast<std::uint16_t>(readU8(stream));

        std::uint32_t pixelFormatStorage[2];
        pixelFormatStorage[0] = 0xFFFFFFFFU;
        pixelFormatStorage[1] = 0U;

        if (readU8(stream) == 0)
        {
            CDisplayManager::GetBestAlphaTextureFormat(
                DAT_01436e1c,
                0x10,
                reinterpret_cast<CPixelFormat*>(pixelFormatStorage));
        }
        else
        {
            CPixelFormat::Initialise(reinterpret_cast<CPixelFormat*>(pixelFormatStorage), 0x31545844);
        }

        std::uint32_t extentX = static_cast<std::uint32_t>(self.textureWidth);
        auto* extentYAsManager =
            reinterpret_cast<CEngineLandscapeMemoryManager*>(static_cast<std::uintptr_t>(self.textureHeight));

        self.texture = CEngineLandscapeMemoryManager::AllocateTexture(
            landscapeMemoryManager,
            reinterpret_cast<CMovableResource*>(this),
            reinterpret_cast<C2DExtentsI*>(&extentX),
            1,
            reinterpret_cast<CPixelFormat*>(pixelFormatStorage),
            0,
            false);
        if (self.texture == nullptr)
        {
            return false;
        }

        CTexture::LoadFromDataStreamToPreallocatedSurface(self.texture, &stream);

        {
            const ulong usage = CEngineResourceManager::GetIndexBufferUsage(DAT_01436e98);
            self.vertexBuffer = CEngineLandscapeMemoryManager::AllocateVertexBuffer(
                landscapeMemoryManager,
                reinterpret_cast<CMovableResource*>(this),
                0,
                static_cast<std::uint32_t>(self.vertexCount),
                0x18,
                usage);
        }
        if (self.vertexBuffer == nullptr)
        {
            return false;
        }

        float compressedVertexBytesF = readF32(stream);

        unsigned char* local_38 = nullptr;
        unsigned char* local_34 = nullptr;
        unsigned char* local_30 = nullptr;
        void* pvStack_2c = nullptr;
        unsigned char* local_28 = nullptr;
        unsigned char* local_24 = nullptr;
        std::uint32_t uStack_20 = 0;

        std__vector___Init(&local_38, &local_34, &local_30, static_cast<std::uint32_t>(self.vertexCount) << 4);
        std__vector___Init(&local_28, &local_24, &pvStack_2c, compressedVertexBytesF);

        {
            unsigned char* compressedWrite = local_34;
            if (local_30 == local_34)
            {
                compressedWrite = reinterpret_cast<unsigned char*>(&local_34);
            }

            if ((0 < static_cast<int>(compressedVertexBytesF)) &&
                (static_cast<std::uint32_t>(s.readPos + static_cast<int>(compressedVertexBytesF)) < 0x80000000U))
            {
                if (s.srcChunkRemaining < static_cast<int>(compressedVertexBytesF))
                {
                    CDataInputStream::ReadWithSrcChunkOverflow(
                        &stream,
                        compressedWrite,
                        static_cast<long>(compressedVertexBytesF));
                }
                else
                {
                    unsigned char* src = s.cursor;
                    for (std::uint32_t i = static_cast<std::uint32_t>(compressedVertexBytesF) >> 2; i != 0; --i)
                    {
                        *reinterpret_cast<std::uint32_t*>(compressedWrite) = *reinterpret_cast<std::uint32_t*>(src);
                        src += 4;
                        compressedWrite += 4;
                    }
                    for (std::uint32_t i = static_cast<std::uint32_t>(compressedVertexBytesF) & 3U; i != 0; --i)
                    {
                        *compressedWrite = *src;
                        ++src;
                        ++compressedWrite;
                    }
                    s.cursor = s.cursor + static_cast<int>(compressedVertexBytesF);
                    s.srcChunkRemaining = s.srcChunkRemaining - static_cast<int>(compressedVertexBytesF);
                    s.readPos = s.readPos + static_cast<int>(compressedVertexBytesF);
                }
            }
        }

        {
            void* decompressedSpan = local_28;
            if (local_24 == local_28)
            {
                decompressedSpan = &local_28;
            }

            unsigned char* compressedRead = local_34;
            if (local_30 == local_34)
            {
                compressedRead = reinterpret_cast<unsigned char*>(&local_34);
            }

            CRangeCompressor::Decompress(
                reinterpret_cast<CRangeCompressor*>(&local_38),
                compressedRead,
                static_cast<std::uint32_t>(self.vertexCount),
                0x10,
                decompressedSpan);
        }

        {
            const ulong decompressedVertexBytes =
                static_cast<ulong>(reinterpret_cast<std::uintptr_t>(local_24) -
                                   reinterpret_cast<std::uintptr_t>(local_28));
            if (decompressedVertexBytes == 0)
            {
                local_28 = reinterpret_cast<unsigned char*>(&local_28);
            }

            alignas(CMemoryDataInputStream) std::byte local_1c_storage[sizeof(CMemoryDataInputStream)];
            auto* local_1c = ::new (local_1c_storage) CMemoryDataInputStream(local_28, decompressedVertexBytes);

            CThing* nullThing = nullptr;
            const bool vertexCallResult =
                CThingFilter_IsValid::operator()(
                    reinterpret_cast<CThingFilter_IsValid*>(self.vertexBuffer),
                    static_cast<CThing*>(nullptr));

            auto* vertexWriteWords = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uintptr_t>(reinterpret_cast<void*>(
                    (reinterpret_cast<std::uintptr_t>(self.vertexBuffer) & ~static_cast<std::uintptr_t>(0xFF)) |
                    static_cast<std::uintptr_t>(vertexCallResult))) + 4);

            int vertexIndex = 0;
            if (self.vertexCount != 0)
            {
                do
                {
                    auto* vertex = reinterpret_cast<TVertexLandscapeBackgroundLayout*>(vertexWriteWords - 1);

                    vertex->positionX = readU16(reinterpret_cast<CDataInputStream&>(*local_1c));
                    vertex->positionY = readU16(reinterpret_cast<CDataInputStream&>(*local_1c));
                    *vertexWriteWords = readU32(reinterpret_cast<CDataInputStream&>(*local_1c));

                    pixelFormatStorage[0] = readU32(reinterpret_cast<CDataInputStream&>(*local_1c));
                    SetTargetVertexNormal(
                        reinterpret_cast<CTVertexLandscapeBackground*>(vertex),
                        pixelFormatStorage[0]);

                    vertex->byte16 = static_cast<std::uint8_t>(readU16(reinterpret_cast<CDataInputStream&>(*local_1c)));
                    vertex->byte15 = static_cast<std::uint8_t>(readU16(reinterpret_cast<CDataInputStream&>(*local_1c)));

                    ++vertexIndex;
                    vertexWriteWords += 6;
                } while (vertexIndex < static_cast<int>(self.vertexCount));
            }

            CThingFilter_IsValid::operator()(
                reinterpret_cast<CThingFilter_IsValid*>(self.vertexBuffer),
                nullThing);

            self.usesSharedIndexBuffer = (self.indexMode == 1);
            if (self.indexMode == 1)
            {
                self.indexBuffer = *reinterpret_cast<CIndexBuffer**>(
                    DAT_01436ea8 + 0x14 +
                    ((static_cast<std::uint32_t>(self.patchX) * 0x11U +
                      static_cast<std::uint32_t>(self.patchY)) * 4U));
            }
            else
            {
                const EPrimitiveType primitiveType = static_cast<EPrimitiveType>(4);
                const EIndexBufferFormat indexFormat = static_cast<EIndexBufferFormat>(0x65);
                const ulong usage = CEngineResourceManager::GetIndexBufferUsage(DAT_01436e98);

                self.indexBuffer = CEngineLandscapeMemoryManager::AllocateIndexBuffer(
                    extentYAsManager,
                    reinterpret_cast<CMovableResource*>(this),
                    static_cast<std::uint32_t>(self.triangleCount) * 6,
                    usage,
                    indexFormat,
                    primitiveType);
                if (self.indexBuffer == nullptr)
                {
                    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&uStack_20));
                    if (local_38 != nullptr)
                    {
                        std::free(local_38);
                    }
                    if (pvStack_2c == nullptr)
                    {
                        return false;
                    }
                    std::free(pvStack_2c);
                    return false;
                }

                void* const lockedIndices = ::CIndexBuffer::Lock(self.indexBuffer, 0, 0, 0);

                const std::uint32_t rawCompressedIndexBytes = readU32(stream);
                std::uint32_t maskedInsertWord = rawCompressedIndexBytes & 0xFFFFFF00U;

                if (rawCompressedIndexBytes <
                    static_cast<std::uint32_t>(
                        reinterpret_cast<std::uintptr_t>(local_34) -
                        reinterpret_cast<std::uintptr_t>(local_38)))
                {
                    local_34 = local_38 + rawCompressedIndexBytes;
                }
                else
                {
                    std__deque__insert(
                        local_34,
                        local_38 + (rawCompressedIndexBytes -
                            static_cast<std::uint32_t>(
                                reinterpret_cast<std::uintptr_t>(local_34) -
                                reinterpret_cast<std::uintptr_t>(local_38))),
                        &maskedInsertWord);
                }

                {
                    unsigned char* compressedIndexWrite = local_38;
                    if (local_34 == local_38)
                    {
                        compressedIndexWrite = reinterpret_cast<unsigned char*>(&local_38);
                    }

                    if ((0 < static_cast<int>(rawCompressedIndexBytes)) &&
                        (static_cast<std::uint32_t>(s.readPos + static_cast<int>(rawCompressedIndexBytes)) < 0x80000000U))
                    {
                        if (s.srcChunkRemaining < static_cast<int>(rawCompressedIndexBytes))
                        {
                            CDataInputStream::ReadWithSrcChunkOverflow(
                                &stream,
                                compressedIndexWrite,
                                static_cast<long>(rawCompressedIndexBytes));
                        }
                        else
                        {
                            unsigned char* src = s.cursor;
                            for (std::uint32_t i = rawCompressedIndexBytes >> 2; i != 0; --i)
                            {
                                *reinterpret_cast<std::uint32_t*>(compressedIndexWrite) =
                                    *reinterpret_cast<std::uint32_t*>(src);
                                src += 4;
                                compressedIndexWrite += 4;
                            }
                            for (std::uint32_t i = rawCompressedIndexBytes & 3U; i != 0; --i)
                            {
                                *compressedIndexWrite = *src;
                                ++src;
                                ++compressedIndexWrite;
                            }
                            s.cursor = s.cursor + static_cast<int>(rawCompressedIndexBytes);
                            s.srcChunkRemaining = s.srcChunkRemaining - static_cast<int>(rawCompressedIndexBytes);
                            s.readPos = s.readPos + static_cast<int>(rawCompressedIndexBytes);
                        }
                    }
                }

                {
                    unsigned char* compressedIndexRead = local_38;
                    if (local_34 == local_38)
                    {
                        compressedIndexRead = reinterpret_cast<unsigned char*>(&local_38);
                    }

                    CRangeCompressor::Decompress(
                        reinterpret_cast<CRangeCompressor*>(&landscapeMemoryManager),
                        compressedIndexRead,
                        static_cast<std::uint32_t>(self.triangleCount) * 3,
                        2,
                        lockedIndices);
                }

                CVertexBufferWin32::DoUnlock(reinterpret_cast<CVertexBufferWin32*>(self.indexBuffer));
            }

            CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&uStack_20));
        }

        if (local_38 != nullptr)
        {
            std::free(local_38);
        }
        if (pvStack_2c != nullptr)
        {
            std::free(pvStack_2c);
        }
    }

    {
        int edgeIndex = 0;
        auto* edgeStrip = reinterpret_cast<CPatchTesselationEdgeStrip*>(
            reinterpret_cast<std::byte*>(this) + 0x08);
        do
        {
            if (!CPatchTesselationEdgeStrip::Load(
                    edgeStrip,
                    &stream,
                    reinterpret_cast<CMovableResource*>(this)))
            {
                return false;
            }
            ++edgeIndex;
            edgeStrip = reinterpret_cast<CPatchTesselationEdgeStrip*>(
                reinterpret_cast<std::byte*>(edgeStrip) + 0x2C);
        } while (edgeIndex < 4);
    }

    if (readU8(stream) != 0)
    {
        void* layerMeshStorage = ::operator new(0x60, std::nothrow);
        CEngineWaterBackgroundSubPatch* waterSubPatch = nullptr;

        if (layerMeshStorage != nullptr)
        {
            waterSubPatch = reinterpret_cast<CEngineWaterBackgroundSubPatch*>(
                CLandscapeLayerMesh::CLandscapeLayerMesh(
                    static_cast<CLandscapeLayerMesh*>(layerMeshStorage),
                    *reinterpret_cast<CEngineLandscapeMap**>(
                        *reinterpret_cast<std::uintptr_t*>(reinterpret_cast<std::byte*>(this) + 0x04) + 0xA4)));
        }

        self.waterSubPatch = waterSubPatch;
        if (self.waterSubPatch == nullptr)
        {
            return false;
        }

        const int mapRuntime =
            *reinterpret_cast<int*>(
                *reinterpret_cast<int*>(
                    *reinterpret_cast<int*>(reinterpret_cast<std::byte*>(this) + 0x04) + 0xA4) + 0x14);

        if (!CEngineWaterBackgroundSubPatch::Load(
                self.waterSubPatch,
                &stream,
                self.patchX,
                self.patchY,
                static_cast<std::uint32_t>(*reinterpret_cast<std::uint16_t*>(mapRuntime + 0x60)) +
                    static_cast<std::uint32_t>(self.waterOffsetX),
                static_cast<std::uint32_t>(*reinterpret_cast<std::uint16_t*>(mapRuntime + 0x62)) +
                    static_cast<std::uint32_t>(self.waterOffsetY),
                self.patchWorldX,
                self.patchWorldY))
        {
            return false;
        }
    }

    return true;
}