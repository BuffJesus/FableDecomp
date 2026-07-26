bool CLandscapeBackgroundPatch::LoadCompressed(
    CMemoryDataInputStream& compressedStream,
    float param_2,
    float param_3)
{
    struct CDataInputStreamLayout
    {
        std::byte pad00[0x04];
        std::uint32_t m_uBytesRead;        // 0x04
        std::byte pad08[0x04];
        std::uint32_t* m_pSrcCursor32;     // 0x0C
        std::byte pad10[0x04];
        int m_nSrcBytesRemaining;          // 0x14
        int m_nSourceChunkBaseAddress;     // 0x18
    };

    static_assert(offsetof(CDataInputStreamLayout, m_uBytesRead) == 0x04);
    static_assert(offsetof(CDataInputStreamLayout, m_pSrcCursor32) == 0x0C);
    static_assert(offsetof(CDataInputStreamLayout, m_nSrcBytesRemaining) == 0x14);
    static_assert(offsetof(CDataInputStreamLayout, m_nSourceChunkBaseAddress) == 0x18);

    CMemoryDataInputStream* this_00 = &compressedStream;
    CLandscapeBackgroundPatch* local_38 = this;
    int local_40;
    CMemoryDataInputStream* local_3c;
    CMemoryDataInputStream* param_1 = this_00;
    CMemoryDataInputStream* pCVar5;
    void* pvVar1;
    int iVar3;
    int iVar4;
    int iVar6;
    bool bVar2;
    auto& stream = *reinterpret_cast<CDataInputStreamLayout*>(this_00);

    alignas(CTemporaryBuffer) std::byte local_34[sizeof(CTemporaryBuffer)];
    auto* const local_34_obj = reinterpret_cast<CTemporaryBuffer*>(local_34);

    void* local_30;

    alignas(CMemoryDataInputStream) std::byte aCStack_1c_storage[sizeof(CMemoryDataInputStream)];
    auto* const aCStack_1c = reinterpret_cast<CMemoryDataInputStream*>(aCStack_1c_storage);

    if (stream.m_uBytesRead + 4U < 0x80000000U)
    {
        if (stream.m_nSrcBytesRemaining < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(this_00),
                reinterpret_cast<uchar*>(&param_1),
                4);
        }
        else
        {
            pCVar5 = reinterpret_cast<CMemoryDataInputStream*>(*stream.m_pSrcCursor32);
            stream.m_pSrcCursor32 = stream.m_pSrcCursor32 + 1;
            stream.m_nSrcBytesRemaining = stream.m_nSrcBytesRemaining + -4;
            stream.m_uBytesRead = stream.m_uBytesRead + 4U;
            param_1 = pCVar5;
        }
    }

    iVar4 = local_40;
    if (stream.m_uBytesRead + 4U < 0x80000000U)
    {
        if (stream.m_nSrcBytesRemaining < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(this_00),
                reinterpret_cast<uchar*>(&local_40),
                4);
            iVar4 = local_40;
        }
        else
        {
            iVar4 = *reinterpret_cast<int*>(stream.m_pSrcCursor32);
            stream.m_pSrcCursor32 = stream.m_pSrcCursor32 + 1;
            stream.m_nSrcBytesRemaining = stream.m_nSrcBytesRemaining + -4;
            stream.m_uBytesRead = stream.m_uBytesRead + 4U;
        }
    }

    pCVar5 = param_1 + 3;
    CTemporaryBuffer::CTemporaryBuffer(
        local_34_obj,
        static_cast<ulong>(reinterpret_cast<std::uint32_t>(pCVar5)),
        *reinterpret_cast<CHandleBasedMemoryPool**>(*(int*)(DAT_01436ea8 + 0x6B0) + 8),
        reinterpret_cast<CScratchBuffer*>(&DAT_013cb498));

    local_30 = *reinterpret_cast<void**>(reinterpret_cast<std::byte*>(local_34_obj) + 4);
    pvVar1 = local_30;
    local_3c = pCVar5;

    if (0 < iVar4)
    {
        iVar3 = (*reinterpret_cast<int(__thiscall***)(CMemoryDataInputStream*)>(this_00))[2](this_00);
        if (static_cast<std::uint32_t>(iVar3 + iVar4) < 0x80000000U)
        {
            iVar6 = stream.m_nSourceChunkBaseAddress + iVar3;
            CDataInputStream::SetPositionAssumeWithinSourceChunk(
                reinterpret_cast<CDataInputStream*>(this_00),
                iVar3 + iVar4);
            goto LAB_00be8a30;
        }
    }

    iVar6 = 0;

LAB_00be8a30:
    LZSS_Decompress(iVar6, iVar4, pvVar1, &local_3c, 0);
    CMemoryDataInputStream::CMemoryDataInputStream(
        aCStack_1c,
        local_30,
        static_cast<ulong>(reinterpret_cast<std::uint32_t>(param_1)));
    bVar2 = local_38->Load(
        reinterpret_cast<CDataInputStream&>(*aCStack_1c),
        param_2,
        param_3);
    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(aCStack_1c));
    CTemporaryBuffer::~CTemporaryBuffer(local_34_obj);
    return bVar2;
}