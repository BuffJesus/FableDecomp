using CAnimationPair = std::pair<unsigned long, CAnimationEntry>;

struct CRefCountOverlay
{
    std::int32_t RefCount;                   // +0x00
    void (__thiscall* Release)(void* self);  // +0x04
};

struct CCountedPointerAsyncDataOverlay
{
    CBankFileAsyncData* Ptr;                 // +0x00
    CRefCountOverlay* Ref;                   // +0x04
};

struct CDataInputStreamOverlay
{
    void* Vftable;                           // +0x00
    std::uint32_t BytesRead;                 // +0x04
    std::byte Pad08[0x04];
    std::uint32_t* Cursor32;                 // +0x0C
    std::byte Pad10[0x04];
    std::int32_t BytesRemaining;             // +0x14
    std::int32_t ChunkBase;                  // +0x18
};

struct CDataInputStreamVftableOverlay
{
    void* Slot00;
    void* Slot04;
    std::int32_t (__thiscall* GetPosition)(void* self);
};

struct CLandscapeLayerMeshOverlay
{
    std::byte Pad00[0x04];
    std::int32_t ForegroundTextureIndex;     // +0x04
    std::byte Pad08[0x04];
    std::int32_t BumpMapTextureIndex;        // +0x0C
    std::byte Pad10[0x2C];
    CLandscapeLayerMesh* Next;               // +0x3C
};

struct CEngineLandscapePatchOverlay
{
    std::int32_t Pad00;                      // +0x00
    CEngineLandscapeMap* Map;                // +0x04
    CLandscapeLayerMesh* LayerMeshList;      // +0x08
    std::uint16_t* FastAccessTextureIDs;     // +0x0C
    std::uint16_t FastAccessTextureIDCount;  // +0x10
    std::uint16_t Pad12;                     // +0x12
    CWaterPatchMesh* WaterPatch;             // +0x14
    std::int32_t FilePos;                    // +0x18
    std::int32_t FileDataSize;               // +0x1C
    std::byte BoundingBox[0x18];             // +0x20
    std::uint16_t MapX;                      // +0x38
    std::uint16_t MapY;                      // +0x3A
    std::uint8_t Flags;                      // +0x3C
    std::byte Pad3D[0x03];
    CCountedPointerAsyncDataOverlay LoadingPatch; // +0x40
};

struct CBeginReadDataAutoAllocTemp
{
    unsigned long Data;                      // +0x00, local_64
    int* RefCount;                           // +0x04, local_60
};

static_assert(offsetof(CEngineLandscapePatchOverlay, Map) == 0x04);
static_assert(offsetof(CEngineLandscapePatchOverlay, LayerMeshList) == 0x08);
static_assert(offsetof(CEngineLandscapePatchOverlay, FastAccessTextureIDs) == 0x0C);
static_assert(offsetof(CEngineLandscapePatchOverlay, FastAccessTextureIDCount) == 0x10);
static_assert(offsetof(CEngineLandscapePatchOverlay, WaterPatch) == 0x14);
static_assert(offsetof(CEngineLandscapePatchOverlay, FilePos) == 0x18);
static_assert(offsetof(CEngineLandscapePatchOverlay, FileDataSize) == 0x1C);
static_assert(offsetof(CEngineLandscapePatchOverlay, Flags) == 0x3C);
static_assert(offsetof(CEngineLandscapePatchOverlay, LoadingPatch) == 0x40);
static_assert(offsetof(CCountedPointerAsyncDataOverlay, Ptr) == 0x00);
static_assert(offsetof(CCountedPointerAsyncDataOverlay, Ref) == 0x04);
static_assert(offsetof(CLandscapeLayerMeshOverlay, ForegroundTextureIndex) == 0x04);
static_assert(offsetof(CLandscapeLayerMeshOverlay, BumpMapTextureIndex) == 0x0C);
static_assert(offsetof(CLandscapeLayerMeshOverlay, Next) == 0x3C);
static_assert(offsetof(CBeginReadDataAutoAllocTemp, Data) == 0x00);
static_assert(offsetof(CBeginReadDataAutoAllocTemp, RefCount) == 0x04);
static_assert(sizeof(CEngineLandscapePatch) == 0x48);

// Unresolved register-fed operands observed in the decompile.
extern "C" CAnimationPair* __cdecl Fable_UnresolvedForwarded_EBX_CAnimationPair();
extern "C" CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>* __cdecl
Fable_UnresolvedForwarded_EDX_OptimisedPrimitiveCountedPointer();

// Observed std::_Cons_val call shape:
//   _Cons_val(allocator_tag, this, unaff_EBX)
// Returns the candidate storage pointer that is immediately null-checked.
extern "C" void* __cdecl Fable_Std_ConsVal_PreserveForwardedInput(
    void* pAllocatorTag,
    void* pContext,
    CAnimationPair* pForwardedEBX);

// Observed std::_Dest_val call shape:
//   _Dest_val(&param_2, extraout_EDX)
extern "C" void __cdecl Fable_Std_DestVal_PreserveForwardedInput(
    void* pAllocatorTag,
    CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>* pForwardedEDX);

LS_LOADING_STATUS __thiscall CEngineLandscapePatch::LoadForegroundPatch(
    CBankFileAsyncEntry* pBankEntry,
    int* pBankEntryRefCount)
{
    auto& self = *reinterpret_cast<CEngineLandscapePatchOverlay*>(this);

    auto ReleaseCountedRef = [](int* pRefCount) -> void
    {
        if ((pRefCount != nullptr) && ((*pRefCount = *pRefCount + -1), *pRefCount == 0))
        {
            reinterpret_cast<void(__thiscall*)(int*)>(pRefCount[1])(pRefCount);
            operator_delete(pRefCount);
        }
    };

    CBeginReadDataAutoAllocTemp local64AndLocal60;
    unsigned long& local64 = local64AndLocal60.Data;

    if (self.LayerMeshList != nullptr)
    {
        ReleaseCountedRef(pBankEntryRefCount);
        return static_cast<LS_LOADING_STATUS>(1);
    }

    auto* const pLoadingPatchAsDisk =
        reinterpret_cast<CCountedPointer<CDiskFileWin32>*>(reinterpret_cast<std::byte*>(this) + 0x40);

    if (self.LoadingPatch.Ptr == nullptr)
    {
        CCountedPointer<CDiskFileWin32>* const pReadResult =
            CBankFileAsyncEntry::BeginReadDataAutoAlloc(
                pBankEntry,
                reinterpret_cast<unsigned long*>(&local64AndLocal60),
                static_cast<unsigned long>(self.FilePos),
                *reinterpret_cast<EThreadedFilePriority*>(&self.FileDataSize));

        CCountedPointer<CDiskFileWin32>::operator=(pLoadingPatchAsDisk, pReadResult);

        if ((local64AndLocal60.RefCount != nullptr) &&
            ((*local64AndLocal60.RefCount = *local64AndLocal60.RefCount + -1),
             *local64AndLocal60.RefCount == 0))
        {
            reinterpret_cast<void(__thiscall*)(int*)>(local64AndLocal60.RefCount[1])(local64AndLocal60.RefCount);
            operator_delete(local64AndLocal60.RefCount);
        }
    }

    if (!CBankFileAsyncData::IsFinished(self.LoadingPatch.Ptr))
    {
        ReleaseCountedRef(pBankEntryRefCount);
        return static_cast<LS_LOADING_STATUS>(2);
    }

    void* const pCompressedData = CBankFileAsyncData::GetData(self.LoadingPatch.Ptr);

    CMemoryDataInputStream compressedStream(pCompressedData, static_cast<unsigned long>(self.FileDataSize));
    auto& compressedOverlay = reinterpret_cast<CDataInputStreamOverlay&>(compressedStream);

    unsigned long carriedWord = 0;
    if (compressedOverlay.BytesRead + 4U < 0x80000000U)
    {
        if (compressedOverlay.BytesRemaining < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(&compressedStream),
                reinterpret_cast<uchar*>(&carriedWord),
                4);
        }
        else
        {
            carriedWord = *compressedOverlay.Cursor32;
            compressedOverlay.Cursor32 = compressedOverlay.Cursor32 + 1;
            compressedOverlay.BytesRemaining = compressedOverlay.BytesRemaining + -4;
            compressedOverlay.BytesRead = compressedOverlay.BytesRead + 4;
        }
    }

    unsigned long compressedSize = local64;
    if (compressedOverlay.BytesRead + 4U < 0x80000000U)
    {
        if (compressedOverlay.BytesRemaining < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(&compressedStream),
                reinterpret_cast<uchar*>(&local64),
                4);
            compressedSize = local64;
        }
        else
        {
            compressedSize = *compressedOverlay.Cursor32;
            compressedOverlay.Cursor32 = compressedOverlay.Cursor32 + 1;
            compressedOverlay.BytesRemaining = compressedOverlay.BytesRemaining + -4;
            compressedOverlay.BytesRead = compressedOverlay.BytesRead + 4;
        }
    }

    const unsigned long tempBufferSize = carriedWord + 3;
    CTemporaryBuffer tempBuffer(
        tempBufferSize,
        *reinterpret_cast<CHandleBasedMemoryPool**>(*(int*)(DAT_01436ea8 + 0x6B0) + 8),
        reinterpret_cast<CScratchBuffer*>(&DAT_013cb498));

    void* const pTempData = *reinterpret_cast<void**>(reinterpret_cast<std::byte*>(&tempBuffer) + 0x04);
    unsigned long lzssOutputSize = tempBufferSize;

    int compressedPayloadPosition = 0;
    if (0 < static_cast<int>(compressedSize))
    {
        const int position =
            reinterpret_cast<CDataInputStreamVftableOverlay*>(compressedOverlay.Vftable)->GetPosition(&compressedStream);
        if (position + compressedSize < 0x80000000UL)
        {
            compressedPayloadPosition = compressedOverlay.ChunkBase + position;
            CDataInputStream::SetPositionAssumeWithinSourceChunk(
                reinterpret_cast<CDataInputStream*>(&compressedStream),
                position + static_cast<int>(compressedSize));
        }
    }

    LZSS_Decompress(compressedPayloadPosition, compressedSize, pTempData, &lzssOutputSize, 0);

    CMemoryDataInputStream foregroundStream(pTempData, carriedWord);
    auto& foregroundOverlay = reinterpret_cast<CDataInputStreamOverlay&>(foregroundStream);

    if (foregroundOverlay.BytesRead + 2U < 0x80000000U)
    {
        if (foregroundOverlay.BytesRemaining < 2)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(&foregroundStream),
                reinterpret_cast<uchar*>(&carriedWord),
                2);
        }
        else
        {
            const std::uint32_t raw = *foregroundOverlay.Cursor32;
            foregroundOverlay.Cursor32 =
                reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(foregroundOverlay.Cursor32) + 2);
            foregroundOverlay.BytesRemaining = foregroundOverlay.BytesRemaining + -2;
            carriedWord = (carriedWord & 0xFFFF0000UL) | static_cast<std::uint16_t>(raw);
            foregroundOverlay.BytesRead = foregroundOverlay.BytesRead + 2;
        }
    }

    local64 = carriedWord & 0xFFFFUL;
    int layerIndex = 0;
    self.FastAccessTextureIDCount = 0;

    if (local64 != 0)
    {
        CLandscapeLayerMesh* pPreviousLayer = nullptr;
        do
        {
            CLandscapeLayerMesh* const pLayerStorage =
                reinterpret_cast<CLandscapeLayerMesh*>(
                    Fable_Std_ConsVal_PreserveForwardedInput(
                        reinterpret_cast<void*>(&DAT_0000004c),
                        reinterpret_cast<void*>(this),
                        Fable_UnresolvedForwarded_EBX_CAnimationPair()));

            CLandscapeLayerMesh* pLayer = nullptr;
            if ((pLayerStorage == nullptr) ||
                ((pLayer = CLandscapeLayerMesh::CLandscapeLayerMesh(pLayerStorage, self.Map)), pLayer == nullptr))
            {
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
                CTemporaryBuffer::~CTemporaryBuffer(&tempBuffer);
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
                ReleaseCountedRef(pBankEntryRefCount);
                return static_cast<LS_LOADING_STATUS>(3);
            }

            if (!CLandscapeLayerMesh::LoadForeground(
                    pLayer,
                    reinterpret_cast<CDataInputStream*>(&foregroundStream),
                    reinterpret_cast<CEngineTexturePalette*>(reinterpret_cast<std::byte*>(self.Map) + 0x40)))
            {
                reinterpret_cast<void(__thiscall*)(CLandscapeLayerMesh*, int)>(
                    **reinterpret_cast<void***>(pLayer))(pLayer, 1);

                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
                CTemporaryBuffer::~CTemporaryBuffer(&tempBuffer);
                CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
                Fable_Std_DestVal_PreserveForwardedInput(
                    &pBankEntry,
                    Fable_UnresolvedForwarded_EDX_OptimisedPrimitiveCountedPointer());
                return static_cast<LS_LOADING_STATUS>(3);
            }

            auto& layerOverlay = *reinterpret_cast<CLandscapeLayerMeshOverlay*>(pLayer);
            if (0 < layerOverlay.ForegroundTextureIndex)
            {
                self.FastAccessTextureIDCount = static_cast<std::uint16_t>(self.FastAccessTextureIDCount + 1);
            }
            if (0 < layerOverlay.BumpMapTextureIndex)
            {
                self.FastAccessTextureIDCount = static_cast<std::uint16_t>(self.FastAccessTextureIDCount + 1);
            }

            if (pPreviousLayer == nullptr)
            {
                self.LayerMeshList = pLayer;
            }
            else
            {
                reinterpret_cast<CLandscapeLayerMeshOverlay*>(pPreviousLayer)->Next = pLayer;
            }

            layerIndex = layerIndex + 1;
            pPreviousLayer = pLayer;
        } while (layerIndex < static_cast<int>(local64));
    }

    self.FastAccessTextureIDs = reinterpret_cast<std::uint16_t*>(
        CEngineLandscapeMemoryManager::AllocVirtual(
            *reinterpret_cast<CEngineLandscapeMemoryManager**>(DAT_01436ea8 + 0x6B0),
            static_cast<unsigned int>(self.FastAccessTextureIDCount) << 2,
            reinterpret_cast<CMovableResource*>(this)));

    if (self.FastAccessTextureIDs == nullptr)
    {
        CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
        CTemporaryBuffer::~CTemporaryBuffer(&tempBuffer);
        CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
        ReleaseCountedRef(pBankEntryRefCount);
        return static_cast<LS_LOADING_STATUS>(3);
    }

    CLandscapeLayerMesh* pLayer = self.LayerMeshList;
    if (pLayer != nullptr)
    {
        int writeOffset = 0;
        do
        {
            const auto& layerOverlay = *reinterpret_cast<CLandscapeLayerMeshOverlay*>(pLayer);

            if (0 < layerOverlay.ForegroundTextureIndex)
            {
                *reinterpret_cast<short*>(reinterpret_cast<std::byte*>(self.FastAccessTextureIDs) + writeOffset) =
                    static_cast<short>(layerOverlay.ForegroundTextureIndex);
                writeOffset = writeOffset + 2;
            }
            if (0 < layerOverlay.BumpMapTextureIndex)
            {
                *reinterpret_cast<short*>(reinterpret_cast<std::byte*>(self.FastAccessTextureIDs) + writeOffset) =
                    static_cast<short>(layerOverlay.BumpMapTextureIndex);
                writeOffset = writeOffset + 2;
            }

            pLayer = layerOverlay.Next;
        } while (pLayer != nullptr);
    }

    if (foregroundOverlay.BytesRead + 1U < 0x80000000U)
    {
        if (foregroundOverlay.BytesRemaining < 1)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(&foregroundStream),
                reinterpret_cast<uchar*>(&carriedWord),
                1);
        }
        else
        {
            const std::uint32_t raw = *foregroundOverlay.Cursor32;
            foregroundOverlay.Cursor32 =
                reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uintptr_t>(foregroundOverlay.Cursor32) + 1);
            foregroundOverlay.BytesRemaining = foregroundOverlay.BytesRemaining + -1;
            carriedWord = (carriedWord & 0xFFFFFF00UL) | static_cast<unsigned char>(raw);
            foregroundOverlay.BytesRead = foregroundOverlay.BytesRead + 1;
        }
    }

    if (static_cast<char>(carriedWord) != '\0')
    {
        if (self.WaterPatch != nullptr)
        {
            reinterpret_cast<void(__thiscall*)(CWaterPatchMesh*, int)>(
                **reinterpret_cast<void***>(self.WaterPatch))(self.WaterPatch, 1);
            self.WaterPatch = nullptr;
        }

        if (foregroundOverlay.BytesRead + 4U < 0x80000000U)
        {
            if (foregroundOverlay.BytesRemaining < 4)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(
                    reinterpret_cast<CDataInputStream*>(&foregroundStream),
                    reinterpret_cast<uchar*>(&local64),
                    4);
            }
            else
            {
                local64 = *foregroundOverlay.Cursor32;
                foregroundOverlay.Cursor32 = foregroundOverlay.Cursor32 + 1;
                foregroundOverlay.BytesRemaining = foregroundOverlay.BytesRemaining + -4;
                foregroundOverlay.BytesRead = foregroundOverlay.BytesRead + 4;
            }
        }

        unsigned long waterCoordX = local64;
        unsigned long waterCoordY = carriedWord;
        if (foregroundOverlay.BytesRead + 4U < 0x80000000U)
        {
            if (foregroundOverlay.BytesRemaining < 4)
            {
                CDataInputStream::ReadWithSrcChunkOverflow(
                    reinterpret_cast<CDataInputStream*>(&foregroundStream),
                    reinterpret_cast<uchar*>(&carriedWord),
                    4);
            }
            else
            {
                carriedWord = *foregroundOverlay.Cursor32;
                foregroundOverlay.Cursor32 = foregroundOverlay.Cursor32 + 1;
                foregroundOverlay.BytesRemaining = foregroundOverlay.BytesRemaining + -4;
                foregroundOverlay.BytesRead = foregroundOverlay.BytesRead + 4;
            }
            waterCoordY = carriedWord;
        }

        unsigned int waterCoords[2];
        waterCoords[0] = static_cast<unsigned int>(waterCoordX);
        waterCoords[1] = static_cast<unsigned int>(waterCoordY);

        CWaterPatchMesh* const pWaterStorage =
            reinterpret_cast<CWaterPatchMesh*>(
                Fable_Std_ConsVal_PreserveForwardedInput(
                    reinterpret_cast<void*>(&DAT_00000020),
                    reinterpret_cast<void*>(this),
                    Fable_UnresolvedForwarded_EBX_CAnimationPair()));

        CWaterPatchMesh* pWaterPatch = nullptr;
        if (pWaterStorage != nullptr)
        {
            pWaterPatch = reinterpret_cast<CWaterPatchMesh*>(
                CWaterPatchMesh::CWaterPatchMesh(
                    pWaterStorage,
                    self.Map,
                    reinterpret_cast<CDataInputStream*>(&foregroundStream),
                    reinterpret_cast<C2DCoordI*>(&waterCoords[0])));
        }

        self.WaterPatch = pWaterPatch;
        if (pWaterPatch == nullptr)
        {
            CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
            CTemporaryBuffer::~CTemporaryBuffer(&tempBuffer);
            CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
            Fable_Std_DestVal_PreserveForwardedInput(
                &pBankEntry,
                Fable_UnresolvedForwarded_EDX_OptimisedPrimitiveCountedPointer());
            return static_cast<LS_LOADING_STATUS>(3);
        }
    }

    if (self.LoadingPatch.Ref != nullptr)
    {
        self.LoadingPatch.Ref->RefCount = self.LoadingPatch.Ref->RefCount + -1;
        if (self.LoadingPatch.Ref->RefCount == 0)
        {
            self.LoadingPatch.Ref->Release(self.LoadingPatch.Ref);
            operator_delete(self.LoadingPatch.Ref);
        }
    }

    self.LoadingPatch.Ref = nullptr;
    self.LoadingPatch.Ptr = nullptr;
    self.Flags = static_cast<std::uint8_t>(self.Flags | 0x02);

    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&foregroundStream));
    CTemporaryBuffer::~CTemporaryBuffer(&tempBuffer);
    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(&compressedStream));
    ReleaseCountedRef(pBankEntryRefCount);
    return static_cast<LS_LOADING_STATUS>(1);
}