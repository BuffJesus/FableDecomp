LS_LOADING_STATUS CEngineLandscapePatch::LoadForegroundPatch(
    CCountedPointer<CBankFileAsyncEntry> bankEntry)
{
    struct CRefCount
    {
        int m_nRefCount;   // 0x00
        void* m_pDestroy;  // 0x04
    };

    template <typename T>
    struct CCountedPointerLayout
    {
        T* m_pObject;            // 0x00
        CRefCount* m_pRefCount;  // 0x04
    };

    struct CDataInputStreamLayout
    {
        std::byte m_Pad00[0x04];
        unsigned int m_uBytesRead;     // 0x04  (uStack_18 / uStack_34)
        std::byte m_Pad08[0x04];
        unsigned int* m_pCursor32;     // 0x0C  (puStack_10 / puStack_2c)
        std::byte m_Pad10[0x04];
        int m_nBytesRemaining;         // 0x14  (iStack_8 / iStack_24)
        int m_nChunkBaseAddress;       // 0x18  (iStack_4)
    };

    struct CLandscapeLayerMeshLayout
    {
        std::byte m_Pad00[0x04];
        int m_nPrimaryTextureIndex;    // 0x04
        std::byte m_Pad08[0x04];
        int m_nSecondaryTextureIndex;  // 0x0C
        std::byte m_Pad10[0x2C];
        CLandscapeLayerMesh* m_pNext;  // 0x3C
    };

    struct CPatchLayout
    {
        std::byte m_Pad00[0x04];
        CEngineLandscapeMap* m_pLandscapeMap;                           // 0x04
        CLandscapeLayerMesh* m_pForegroundHead;                         // 0x08
        void* m_pForegroundTextureIndexBuffer;                          // 0x0C
        unsigned short m_nForegroundTextureIndexCount;                  // 0x10
        CWaterPatchMesh* m_pWaterPatchMesh;                             // 0x14
        unsigned long m_uForegroundChunkOffset;                         // 0x18
        unsigned long m_uForegroundChunkSize;                           // 0x1C
        std::byte m_Pad20[0x1C];
        unsigned char m_Flags;                                          // 0x3C
        CCountedPointerLayout<CBankFileAsyncData> m_ForegroundAsync;    // 0x40
    };

    static_assert(offsetof(CPatchLayout, m_pLandscapeMap) == 0x04);
    static_assert(offsetof(CPatchLayout, m_pForegroundHead) == 0x08);
    static_assert(offsetof(CPatchLayout, m_pForegroundTextureIndexBuffer) == 0x0C);
    static_assert(offsetof(CPatchLayout, m_nForegroundTextureIndexCount) == 0x10);
    static_assert(offsetof(CPatchLayout, m_pWaterPatchMesh) == 0x14);
    static_assert(offsetof(CPatchLayout, m_uForegroundChunkOffset) == 0x18);
    static_assert(offsetof(CPatchLayout, m_uForegroundChunkSize) == 0x1C);
    static_assert(offsetof(CPatchLayout, m_Flags) == 0x3C);
    static_assert(offsetof(CPatchLayout, m_ForegroundAsync) == 0x40);

    static_assert(offsetof(CCountedPointerLayout<CBankFileAsyncData>, m_pObject) == 0x00);
    static_assert(offsetof(CCountedPointerLayout<CBankFileAsyncData>, m_pRefCount) == 0x04);

    static_assert(offsetof(CDataInputStreamLayout, m_uBytesRead) == 0x04);
    static_assert(offsetof(CDataInputStreamLayout, m_pCursor32) == 0x0C);
    static_assert(offsetof(CDataInputStreamLayout, m_nBytesRemaining) == 0x14);
    static_assert(offsetof(CDataInputStreamLayout, m_nChunkBaseAddress) == 0x18);

    static_assert(offsetof(CLandscapeLayerMeshLayout, m_nPrimaryTextureIndex) == 0x04);
    static_assert(offsetof(CLandscapeLayerMeshLayout, m_nSecondaryTextureIndex) == 0x0C);
    static_assert(offsetof(CLandscapeLayerMeshLayout, m_pNext) == 0x3C);

    auto& self = *reinterpret_cast<CPatchLayout*>(this);
    auto& bankEntryLayout = reinterpret_cast<CCountedPointerLayout<CBankFileAsyncEntry>&>(bankEntry);

    auto CallDeletingDtor = [](void* object) -> void
    {
        using DeletingDtor = void(__thiscall*)(void*, int);
        auto** vftable = *reinterpret_cast<void***>(object);
        reinterpret_cast<DeletingDtor>(vftable[0])(object, 1);
    };

    auto ReleaseIncomingParamEarly = [&]() -> void
    {
        if (bankEntryLayout.m_pRefCount != nullptr)
        {
            bankEntryLayout.m_pRefCount->m_nRefCount = bankEntryLayout.m_pRefCount->m_nRefCount + -1;
            if (bankEntryLayout.m_pRefCount->m_nRefCount == 0)
            {
                reinterpret_cast<void(__thiscall*)(CRefCount*)>(bankEntryLayout.m_pRefCount->m_pDestroy)(
                    bankEntryLayout.m_pRefCount);
                operator delete(bankEntryLayout.m_pRefCount);
            }
        }
    };

    auto DestroyIncomingParamOnFailure = [&]() -> void
    {
        std::_Dest_val<
            std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>>,
            CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>>(
                reinterpret_cast<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>>*>(
                    &bankEntry),
                reinterpret_cast<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>*>(nullptr));
    };

    auto ReadU32 = [](CDataInputStream* stream) -> unsigned long
    {
        auto& s = *reinterpret_cast<CDataInputStreamLayout*>(stream);
        unsigned long value = 0;

        if (s.m_uBytesRead + 4 < 0x80000000U)
        {
            if (s.m_nBytesRemaining < 4)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(stream, reinterpret_cast<uchar*>(&value), 4);
            }
            else
            {
                value = *s.m_pCursor32;
                s.m_pCursor32 = s.m_pCursor32 + 1;
                s.m_nBytesRemaining = s.m_nBytesRemaining + -4;
                s.m_uBytesRead = s.m_uBytesRead + 4;
            }
        }

        return value;
    };

    auto ReadU16 = [](CDataInputStream* stream) -> unsigned long
    {
        auto& s = *reinterpret_cast<CDataInputStreamLayout*>(stream);
        unsigned long value = 0;

        if (s.m_uBytesRead + 2 < 0x80000000U)
        {
            if (s.m_nBytesRemaining < 2)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(stream, reinterpret_cast<uchar*>(&value), 2);
            }
            else
            {
                const unsigned int raw = *s.m_pCursor32;
                s.m_pCursor32 = reinterpret_cast<unsigned int*>(
                    reinterpret_cast<int>(s.m_pCursor32) + 2);
                s.m_nBytesRemaining = s.m_nBytesRemaining + -2;
                value = (value & 0xFFFF0000UL) | static_cast<unsigned short>(raw);
                s.m_uBytesRead = s.m_uBytesRead + 2;
            }
        }

        return value;
    };

    auto ReadU8 = [](CDataInputStream* stream) -> unsigned long
    {
        auto& s = *reinterpret_cast<CDataInputStreamLayout*>(stream);
        unsigned long value = 0;

        if (s.m_uBytesRead + 1 < 0x80000000U)
        {
            if (s.m_nBytesRemaining < 1)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(stream, reinterpret_cast<uchar*>(&value), 1);
            }
            else
            {
                const unsigned int raw = *s.m_pCursor32;
                s.m_pCursor32 = reinterpret_cast<unsigned int*>(
                    reinterpret_cast<int>(s.m_pCursor32) + 1);
                s.m_nBytesRemaining = s.m_nBytesRemaining + -1;
                value = (value & 0xFFFFFF00UL) | static_cast<unsigned char>(raw);
                s.m_uBytesRead = s.m_uBytesRead + 1;
            }
        }

        return value;
    };

    CMemoryDataInputStream compressedStream(
        nullptr,
        0);
    CTemporaryBuffer decompressedBuffer(
        0,
        nullptr,
        reinterpret_cast<CScratchBuffer*>(&DAT_013cb498));
    CMemoryDataInputStream foregroundStream(
        nullptr,
        0);

    if (self.m_pForegroundHead != nullptr)
    {
        ReleaseIncomingParamEarly();
        return static_cast<LS_LOADING_STATUS>(1);
    }

    if (self.m_ForegroundAsync.m_pObject == nullptr)
    {
        CCountedPointer<CDiskFileWin32> tempAsyncRead;
        CCountedPointer<CDiskFileWin32>* returnedAsyncRead =
            CBankFileAsyncEntry::BeginReadDataAutoAlloc(
                bankEntryLayout.m_pObject,
                reinterpret_cast<unsigned long*>(&tempAsyncRead),
                self.m_uForegroundChunkOffset,
                *reinterpret_cast<EThreadedFilePriority*>(&self.m_uForegroundChunkSize));

        CCountedPointer<CDiskFileWin32>::operator=(
            reinterpret_cast<CCountedPointer<CDiskFileWin32>*>(&self.m_ForegroundAsync),
            returnedAsyncRead);

        auto& tempAsyncReadLayout = reinterpret_cast<CCountedPointerLayout<CDiskFileWin32>&>(tempAsyncRead);
        if (tempAsyncReadLayout.m_pRefCount != nullptr)
        {
            tempAsyncReadLayout.m_pRefCount->m_nRefCount = tempAsyncReadLayout.m_pRefCount->m_nRefCount + -1;
            if (tempAsyncReadLayout.m_pRefCount->m_nRefCount == 0)
            {
                reinterpret_cast<void(__thiscall*)(CRefCount*)>(tempAsyncReadLayout.m_pRefCount->m_pDestroy)(
                    tempAsyncReadLayout.m_pRefCount);
                operator delete(tempAsyncReadLayout.m_pRefCount);
            }
        }
    }

    if (!CBankFileAsyncData::IsFinished(self.m_ForegroundAsync.m_pObject))
    {
        ReleaseIncomingParamEarly();
        return static_cast<LS_LOADING_STATUS>(2);
    }

    {
        void* const data = CBankFileAsyncData::GetData(self.m_ForegroundAsync.m_pObject);
        CMemoryDataInputStream::CMemoryDataInputStream(&compressedStream, data, self.m_uForegroundChunkSize);
    }

    unsigned long decompressedSize = ReadU32(reinterpret_cast<CDataInputStream*>(&compressedStream));
    unsigned long compressedSize = ReadU32(reinterpret_cast<CDataInputStream*>(&compressedStream));
    unsigned long tempBufferSize = decompressedSize + 3;

    CTemporaryBuffer::CTemporaryBuffer(
        &decompressedBuffer,
        tempBufferSize,
        *reinterpret_cast<CHandleBasedMemoryPool**>(*(int*)(DAT_01436ea8 + 0x6B0) + 8),
        reinterpret_cast<CScratchBuffer*>(&DAT_013cb498));

    void* decompressedBytes = *reinterpret_cast<void**>(reinterpret_cast<std::byte*>(&decompressedBuffer) + 0x04);
    unsigned long lzssOutSize = tempBufferSize;

    int compressedPayloadAddress = 0;
    if (0 < static_cast<int>(compressedSize))
    {
        const int currentPosition =
            (**reinterpret_cast<int(__thiscall***)(CMemoryDataInputStream*)>(
                reinterpret_cast<std::byte*>(&compressedStream) + 8))(&compressedStream);

        if (currentPosition + compressedSize < 0x80000000UL)
        {
            compressedPayloadAddress =
                reinterpret_cast<CDataInputStreamLayout*>(&compressedStream)->m_nChunkBaseAddress + currentPosition;
            CDataInputStream::SetPositionAssumeWithinSourceChunk(
                reinterpret_cast<CDataInputStream*>(&compressedStream),
                currentPosition + static_cast<int>(compressedSize));
        }
    }

    LZSS_Decompress(
        compressedPayloadAddress,
        compressedSize,
        decompressedBytes,
        &lzssOutSize,
        0);

    CMemoryDataInputStream::CMemoryDataInputStream(&foregroundStream, decompressedBytes, decompressedSize);

    unsigned long layerCountWord = ReadU16(reinterpret_cast<CDataInputStream*>(&foregroundStream));
    unsigned long layerCount = layerCountWord & 0xFFFFUL;
    int layerIndex = 0;
    self.m_nForegroundTextureIndexCount = 0;

    if (layerCount != 0)
    {
        CLandscapeLayerMesh* previousLayer = nullptr;

        do
        {
            std::_Cons_val<
                std::allocator<std::pair<unsigned long, CAnimationEntry>>,
                std::pair<unsigned long, CAnimationEntry>,
                std::pair<unsigned long, CAnimationEntry>&>(
                    reinterpret_cast<std::allocator<std::pair<unsigned long, CAnimationEntry>>*>(&DAT_0000004c),
                    reinterpret_cast<std::pair<unsigned long, CAnimationEntry>*>(this),
                    *reinterpret_cast<std::pair<unsigned long, CAnimationEntry>*>(nullptr));

            CLandscapeLayerMesh* layer = static_cast<CLandscapeLayerMesh*>(operator new(0x4C));
            if (layer == nullptr)
            {
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
                CTemporaryBuffer::~CTemporaryBuffer(&decompressedBuffer);
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
                ReleaseIncomingParamEarly();
                return static_cast<LS_LOADING_STATUS>(3);
            }

            layer = CLandscapeLayerMesh::CLandscapeLayerMesh(layer, self.m_pLandscapeMap);
            if (layer == nullptr)
            {
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
                CTemporaryBuffer::~CTemporaryBuffer(&decompressedBuffer);
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
                ReleaseIncomingParamEarly();
                return static_cast<LS_LOADING_STATUS>(3);
            }

            if (!CLandscapeLayerMesh::LoadForeground(
                    layer,
                    reinterpret_cast<CDataInputStream*>(&foregroundStream),
                    reinterpret_cast<CEngineTexturePalette*>(
                        reinterpret_cast<std::byte*>(self.m_pLandscapeMap) + 0x40)))
            {
                CallDeletingDtor(layer);
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
                CTemporaryBuffer::~CTemporaryBuffer(&decompressedBuffer);
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
                DestroyIncomingParamOnFailure();
                return static_cast<LS_LOADING_STATUS>(3);
            }

            auto& layerLayout = *reinterpret_cast<CLandscapeLayerMeshLayout*>(layer);

            if (0 < layerLayout.m_nPrimaryTextureIndex)
            {
                self.m_nForegroundTextureIndexCount = self.m_nForegroundTextureIndexCount + 1;
            }

            if (0 < layerLayout.m_nSecondaryTextureIndex)
            {
                self.m_nForegroundTextureIndexCount = self.m_nForegroundTextureIndexCount + 1;
            }

            if (previousLayer == nullptr)
            {
                self.m_pForegroundHead = layer;
            }
            else
            {
                reinterpret_cast<CLandscapeLayerMeshLayout*>(previousLayer)->m_pNext = layer;
            }

            layerIndex = layerIndex + 1;
            previousLayer = layer;
        } while (layerIndex < static_cast<int>(layerCount));
    }

    self.m_pForegroundTextureIndexBuffer =
        CEngineLandscapeMemoryManager::AllocVirtual(
            *reinterpret_cast<CEngineLandscapeMemoryManager**>(DAT_01436ea8 + 0x6B0),
            static_cast<unsigned int>(self.m_nForegroundTextureIndexCount) << 2,
            reinterpret_cast<CMovableResource*>(this));

    if (self.m_pForegroundTextureIndexBuffer == nullptr)
    {
        CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
        CTemporaryBuffer::~CTemporaryBuffer(&decompressedBuffer);
        CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
        ReleaseIncomingParamEarly();
        return static_cast<LS_LOADING_STATUS>(3);
    }

    {
        CLandscapeLayerMesh* layer = self.m_pForegroundHead;
        if (layer != nullptr)
        {
            int writeOffset = 0;
            do
            {
                auto& layerLayout = *reinterpret_cast<CLandscapeLayerMeshLayout*>(layer);

                if (0 < layerLayout.m_nPrimaryTextureIndex)
                {
                    *reinterpret_cast<short*>(
                        reinterpret_cast<std::byte*>(self.m_pForegroundTextureIndexBuffer) + writeOffset) =
                        static_cast<short>(layerLayout.m_nPrimaryTextureIndex);
                    writeOffset = writeOffset + 2;
                }

                if (0 < layerLayout.m_nSecondaryTextureIndex)
                {
                    *reinterpret_cast<short*>(
                        reinterpret_cast<std::byte*>(self.m_pForegroundTextureIndexBuffer) + writeOffset) =
                        static_cast<short>(layerLayout.m_nSecondaryTextureIndex);
                    writeOffset = writeOffset + 2;
                }

                layer = layerLayout.m_pNext;
            } while (layer != nullptr);
        }
    }

    {
        const unsigned long hasWaterPatchWord =
            ReadU8(reinterpret_cast<CDataInputStream*>(&foregroundStream));

        if (static_cast<char>(hasWaterPatchWord) != '\0')
        {
            if (self.m_pWaterPatchMesh != nullptr)
            {
                if (self.m_pWaterPatchMesh != nullptr)
                {
                    CallDeletingDtor(self.m_pWaterPatchMesh);
                }
                self.m_pWaterPatchMesh = nullptr;
            }

            const unsigned long waterX = ReadU32(reinterpret_cast<CDataInputStream*>(&foregroundStream));
            const unsigned long waterY = ReadU32(reinterpret_cast<CDataInputStream*>(&foregroundStream));

            unsigned int waterCoords[2];
            waterCoords[0] = static_cast<unsigned int>(waterX);
            waterCoords[1] = static_cast<unsigned int>(waterY);

            std::_Cons_val<
                std::allocator<std::pair<unsigned long, CAnimationEntry>>,
                std::pair<unsigned long, CAnimationEntry>,
                std::pair<unsigned long, CAnimationEntry>&>(
                    reinterpret_cast<std::allocator<std::pair<unsigned long, CAnimationEntry>>*>(&DAT_00000020),
                    reinterpret_cast<std::pair<unsigned long, CAnimationEntry>*>(this),
                    *reinterpret_cast<std::pair<unsigned long, CAnimationEntry>*>(nullptr));

            CWaterPatchMesh* waterPatch = static_cast<CWaterPatchMesh*>(operator new(0x20));
            int waterPatchResult = 0;

            if (waterPatch == nullptr)
            {
                waterPatchResult = 0;
            }
            else
            {
                waterPatchResult = CWaterPatchMesh::CWaterPatchMesh(
                    waterPatch,
                    self.m_pLandscapeMap,
                    reinterpret_cast<CDataInputStream*>(&foregroundStream),
                    reinterpret_cast<C2DCoordI*>(waterCoords));
            }

            self.m_pWaterPatchMesh = reinterpret_cast<CWaterPatchMesh*>(waterPatchResult);
            if (waterPatchResult == 0)
            {
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
                CTemporaryBuffer::~CTemporaryBuffer(&decompressedBuffer);
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
                DestroyIncomingParamOnFailure();
                return static_cast<LS_LOADING_STATUS>(3);
            }
        }
    }

    {
        auto& asyncLayout = self.m_ForegroundAsync;
        if (asyncLayout.m_pRefCount != nullptr)
        {
            asyncLayout.m_pRefCount->m_nRefCount = asyncLayout.m_pRefCount->m_nRefCount + -1;
            if (asyncLayout.m_pRefCount->m_nRefCount == 0)
            {
                reinterpret_cast<void(__thiscall*)(CRefCount*)>(asyncLayout.m_pRefCount->m_pDestroy)(
                    asyncLayout.m_pRefCount);
                operator delete(asyncLayout.m_pRefCount);
            }
        }
        asyncLayout.m_pRefCount = nullptr;
        asyncLayout.m_pObject = nullptr;
    }

    self.m_Flags = static_cast<unsigned char>(self.m_Flags | 2);

    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
    CTemporaryBuffer::~CTemporaryBuffer(&decompressedBuffer);
    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));

    ReleaseIncomingParamEarly();
    return static_cast<LS_LOADING_STATUS>(1);
}