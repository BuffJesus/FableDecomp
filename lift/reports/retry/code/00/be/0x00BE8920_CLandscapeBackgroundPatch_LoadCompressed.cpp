bool CLandscapeBackgroundPatch::LoadCompressed(
    CMemoryDataInputStream& stream,
    float worldX,
    float worldY)
{
    struct MemoryDataInputStreamLayout
    {
        std::byte pad00[0x04];
        std::uint32_t readPos;    // 0x04
        std::byte pad08[0x04];
        unsigned char* cursor;    // 0x0C
        std::byte pad10[0x04];
        int srcChunkRemaining;    // 0x14
        int srcChunkBase;         // 0x18
    };

    static_assert(offsetof(MemoryDataInputStreamLayout, readPos) == 0x04);
    static_assert(offsetof(MemoryDataInputStreamLayout, cursor) == 0x0C);
    static_assert(offsetof(MemoryDataInputStreamLayout, srcChunkRemaining) == 0x14);
    static_assert(offsetof(MemoryDataInputStreamLayout, srcChunkBase) == 0x18);

    static_assert(offsetof(CLandscapeBackgroundPatch, Parent) == 0x04);
    static_assert(offsetof(CLandscapeBackgroundPatch, Texture) == 0x0B8);
    static_assert(offsetof(CLandscapeBackgroundPatch, IndexBuffer) == 0x0BC);
    static_assert(offsetof(CLandscapeBackgroundPatch, VertexBuffer) == 0x0C0);
    static_assert(offsetof(CLandscapeBackgroundPatch, WaterSubPatch) == 0x0F4);
    static_assert(offsetof(CLandscapeBackgroundPatch, MinZ) == 0x0F8);
    static_assert(offsetof(CLandscapeBackgroundPatch, MaxZ) == 0x0FC);
    static_assert(offsetof(CLandscapeBackgroundPatch, VertexCount) == 0x104);
    static_assert(offsetof(CLandscapeBackgroundPatch, PolyCountIncDegenerates) == 0x106);
    static_assert(offsetof(CLandscapeBackgroundPatch, TextureWidthNeeded) == 0x108);
    static_assert(offsetof(CLandscapeBackgroundPatch, TextureHeightNeeded) == 0x10A);
    static_assert(offsetof(CLandscapeBackgroundPatch, MapX) == 0x10C);
    static_assert(offsetof(CLandscapeBackgroundPatch, MapY) == 0x10E);
    static_assert(offsetof(CLandscapeBackgroundPatch, Width) == 0x110);
    static_assert(offsetof(CLandscapeBackgroundPatch, Height) == 0x112);
    static_assert(offsetof(CLandscapeBackgroundPatch, LockedGridSpacingX) == 0x114);
    static_assert(offsetof(CLandscapeBackgroundPatch, LockedGridSpacingY) == 0x116);
    static_assert(offsetof(CLandscapeBackgroundPatch, LODBand) == 0x118);
    static_assert(offsetof(CLandscapeBackgroundPatch, SharedIndexBuffer) == 0x119);
    static_assert(offsetof(CLandscapeBackgroundPatch, WaterOnlyPatch) == 0x11A);
    static_assert(offsetof(CLandscapeBackgroundPatch, IsDefaultSection) == 0x11B);

    CMemoryDataInputStream* this_00 = &stream;
    void* pvVar1;
    bool bVar2;
    int iVar3;
    int iVar4;
    CMemoryDataInputStream* pCVar5;
    int iVar6;
    int local_40;
    CMemoryDataInputStream* local_3c;
    CLandscapeBackgroundPatch* local_38;
    CTemporaryBuffer local_34[4];
    void* local_30;
    CMemoryDataInputStream aCStack_1c[28];

    auto& streamLayout = reinterpret_cast<MemoryDataInputStreamLayout&>(stream);

    local_38 = this;
    pCVar5 = this_00;
    if (streamLayout.readPos + 4U < 0x80000000U)
    {
        if (streamLayout.srcChunkRemaining < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(this_00),
                reinterpret_cast<uchar*>(&pCVar5),
                4);
        }
        else
        {
            pCVar5 = reinterpret_cast<CMemoryDataInputStream*>(*reinterpret_cast<std::uint32_t*>(streamLayout.cursor));
            streamLayout.cursor += 4;
            streamLayout.srcChunkRemaining = streamLayout.srcChunkRemaining + -4;
            streamLayout.readPos = streamLayout.readPos + 4U;
        }
    }

    iVar4 = local_40;
    if (streamLayout.readPos + 4U < 0x80000000U)
    {
        if (streamLayout.srcChunkRemaining < 4)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(this_00),
                reinterpret_cast<uchar*>(&local_40),
                4);
            iVar4 = local_40;
        }
        else
        {
            iVar4 = *reinterpret_cast<int*>(streamLayout.cursor);
            streamLayout.cursor += 4;
            streamLayout.srcChunkRemaining = streamLayout.srcChunkRemaining + -4;
            streamLayout.readPos = streamLayout.readPos + 4U;
        }
    }

    CMemoryDataInputStream* const decompressedLength = pCVar5 + 3;
    CTemporaryBuffer::CTemporaryBuffer(
        local_34,
        static_cast<ulong>(decompressedLength),
        *reinterpret_cast<CHandleBasedMemoryPool**>(*(int*)(DAT_01436ea8 + 0x6B0) + 8),
        reinterpret_cast<CScratchBuffer*>(&DAT_013cb498));

    pvVar1 = local_30;
    local_3c = decompressedLength;

    if (0 < iVar4)
    {
        iVar3 = (*reinterpret_cast<int(__thiscall**)(CMemoryDataInputStream*)>(*reinterpret_cast<std::uintptr_t*>(this_00) + 8))(this_00);
        if (static_cast<std::uint32_t>(iVar3 + iVar4) < 0x80000000U)
        {
            iVar6 = streamLayout.srcChunkBase + iVar3;
            CDataInputStream::SetPositionAssumeWithinSourceChunk(
                reinterpret_cast<CDataInputStream*>(this_00),
                iVar3 + iVar4);
            goto LAB_00be8a30;
        }
    }
    iVar6 = 0;

LAB_00be8a30:
    LZSS_Decompress(iVar6, iVar4, pvVar1, &local_3c, 0);
    CMemoryDataInputStream::CMemoryDataInputStream(aCStack_1c, local_30, static_cast<ulong>(pCVar5));
    bVar2 = Load(local_38, reinterpret_cast<CDataInputStream*>(aCStack_1c), worldX, worldY);
    CChunkedFileChunk::~CChunkedFileChunk(reinterpret_cast<CChunkedFileChunk*>(aCStack_1c));
    CTemporaryBuffer::~CTemporaryBuffer(local_34);
    return bVar2;
}