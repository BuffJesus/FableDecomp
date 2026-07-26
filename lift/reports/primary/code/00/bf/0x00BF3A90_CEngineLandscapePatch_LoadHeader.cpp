void CEngineLandscapePatch::LoadHeader(CMemoryDataInputStream& stream)
{
    struct CDataInputStreamLayout
    {
        std::byte m_Pad00[0x04];
        unsigned int m_uBytesRead;   // 0x04
        std::byte m_Pad08[0x04];
        unsigned int* m_pCursor32;   // 0x0C
        std::byte m_Pad10[0x04];
        int m_nBytesRemaining;       // 0x14
    };

    struct CEngineLandscapePatchHeaderWords
    {
        std::uint32_t m_uForegroundChunkOffset; // 0x18
        std::uint32_t m_uForegroundChunkSize;   // 0x1C
        std::uint32_t m_BoundsMinX;             // 0x20
        std::uint32_t m_BoundsMinY;             // 0x24
        std::uint32_t m_BoundsMinZ;             // 0x28
        std::uint32_t m_BoundsMaxX;             // 0x2C
        std::uint32_t m_BoundsMaxY;             // 0x30
        std::uint32_t m_BoundsMaxZ;             // 0x34
    };

    union HeaderBuffer
    {
        std::uint32_t m_Words[9];
        std::uint8_t m_Bytes[0x24];
    };

    static_assert(offsetof(CDataInputStreamLayout, m_uBytesRead) == 0x04);
    static_assert(offsetof(CDataInputStreamLayout, m_pCursor32) == 0x0C);
    static_assert(offsetof(CDataInputStreamLayout, m_nBytesRemaining) == 0x14);
    static_assert(sizeof(HeaderBuffer) == 0x24);

    auto& input = reinterpret_cast<CDataInputStreamLayout&>(stream);
    auto& headerFields =
        *reinterpret_cast<CEngineLandscapePatchHeaderWords*>(reinterpret_cast<std::byte*>(this) + 0x18);

    HeaderBuffer header;
    header.m_Words[0] = 0;
    header.m_Words[1] = 0;
    header.m_Bytes[0x20] = 0;

    std::uint8_t foregroundEnabled = 0;
    const int bytesRead = static_cast<int>(input.m_uBytesRead);

    if (bytesRead + 0x24U < 0x80000000U)
    {
        if (input.m_nBytesRemaining < 0x24)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(&stream),
                reinterpret_cast<uchar*>(header.m_Bytes),
                0x24);
            foregroundEnabled = header.m_Bytes[0x20];
        }
        else
        {
            unsigned int* pSrc = input.m_pCursor32;
            unsigned int* pDst = header.m_Words;

            for (int count = 9; count != 0; --count)
            {
                *pDst = *pSrc;
                ++pSrc;
                ++pDst;
            }

            input.m_pCursor32 = pSrc;
            input.m_nBytesRemaining = input.m_nBytesRemaining + -0x24;
            input.m_uBytesRead = bytesRead + 0x24U;
            foregroundEnabled = header.m_Bytes[0x20];
        }
    }

    headerFields.m_uForegroundChunkOffset = header.m_Words[0];
    headerFields.m_uForegroundChunkSize = header.m_Words[1];
    headerFields.m_BoundsMinX = header.m_Words[2];
    headerFields.m_BoundsMinY = header.m_Words[3];
    headerFields.m_BoundsMinZ = header.m_Words[4];
    headerFields.m_BoundsMaxX = header.m_Words[5];
    headerFields.m_BoundsMaxY = header.m_Words[6];
    headerFields.m_BoundsMaxZ = header.m_Words[7];
    this->m_Flags = static_cast<std::uint8_t>(
        this->m_Flags ^ (((foregroundEnabled << 4) ^ this->m_Flags) & 0x10));
}