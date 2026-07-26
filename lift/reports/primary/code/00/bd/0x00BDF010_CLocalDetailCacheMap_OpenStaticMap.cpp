void CLocalDetailCacheMap::OpenStaticMap(
    CEngineStaticMapFileBlockManager* pStaticMapFileBlockManager,
    CDataInputStream& in)
{
    struct DataInputStreamVTable
    {
        void* m_slot00;
        void(__thiscall* m_setSrcChunk)(CDataInputStream*, std::uint32_t);
    };

    struct DataInputStreamRuntime
    {
        DataInputStreamVTable* m_vtable;   // 0x00
        std::int32_t m_readPosition;       // 0x04
        std::byte m_pad08[0x04];           // 0x08
        std::uint32_t* m_srcCursor32;      // 0x0C
        std::byte m_pad10[0x04];           // 0x10
        std::int32_t m_srcBytesRemaining;  // 0x14
    };

    struct StaticMapChunkHeader
    {
        std::uint32_t m_chunkType;   // 0x00
        std::uint32_t m_chunkSize;   // 0x04
        std::uint32_t m_chunkFlags;  // 0x08
    };

    struct LocalDetailCacheMapRuntime
    {
        std::byte m_pad00[0x04];
        CEngineStaticMapFileBlockManager* m_pStaticMapFileBlockManager; // 0x04
        std::byte m_pad08[0x04];
        CQuadTreeElement m_quadTreeElement;                             // 0x0C
        CObjectTypeCollectionPalette m_objectTypeCollectionPalette;     // 0x64
    };

    static_assert(offsetof(DataInputStreamRuntime, m_readPosition) == 0x04);
    static_assert(offsetof(DataInputStreamRuntime, m_srcCursor32) == 0x0C);
    static_assert(offsetof(DataInputStreamRuntime, m_srcBytesRemaining) == 0x14);
    static_assert(sizeof(StaticMapChunkHeader) == 0x0C);
    static_assert(offsetof(LocalDetailCacheMapRuntime, m_pStaticMapFileBlockManager) == 0x04);
    static_assert(offsetof(LocalDetailCacheMapRuntime, m_quadTreeElement) == 0x0C);
    static_assert(offsetof(LocalDetailCacheMapRuntime, m_objectTypeCollectionPalette) == 0x64);

    auto& self = *reinterpret_cast<LocalDetailCacheMapRuntime*>(this);
    auto& stream = *reinterpret_cast<DataInputStreamRuntime*>(&in);

    self.m_pStaticMapFileBlockManager = pStaticMapFileBlockManager;

    alignas(4) std::array<std::byte, sizeof(StaticMapChunkHeader)> headerStorage;
    auto& header = *reinterpret_cast<StaticMapChunkHeader*>(headerStorage.data());

    header.m_chunkSize = 0;
    header.m_chunkType = 0;
    headerStorage[8] = std::byte{0};

    if (stream.m_readPosition + 0x0CU < 0x80000000U)
    {
        if (stream.m_srcBytesRemaining < 0x0C)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                &in,
                reinterpret_cast<uchar*>(headerStorage.data()),
                0x0C);
        }
        else
        {
            std::uint32_t* const pHeader = stream.m_srcCursor32;
            header.m_chunkType = pHeader[0];
            header.m_chunkSize = pHeader[1];
            header.m_chunkFlags = pHeader[2];
            stream.m_srcCursor32 = pHeader + 3;
            stream.m_srcBytesRemaining = stream.m_srcBytesRemaining - 0x0C;
            stream.m_readPosition = static_cast<std::int32_t>(stream.m_readPosition + 0x0CU);
        }
    }

    stream.m_vtable->m_setSrcChunk(&in, header.m_chunkType);
    self.m_quadTreeElement.LoadHeader(in);
    self.m_objectTypeCollectionPalette.Load(in);
}