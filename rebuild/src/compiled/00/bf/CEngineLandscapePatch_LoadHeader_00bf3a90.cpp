#include "candidates/landscape_patch_compiled.h"

void CEngineLandscapePatch::LoadHeader(CMemoryDataInputStream& stream)
{
    struct CDataInputStreamLayout
    {
        fable_u8 m_Pad00[0x04];
        fable_u32 m_uBytesRead;
        fable_u8 m_Pad08[0x04];
        fable_u32* m_pCursor32;
        fable_u8 m_Pad10[0x04];
        fable_i32 m_nBytesRemaining;
    };

    struct CEngineLandscapePatchHeaderWords
    {
        fable_u32 m_uForegroundChunkOffset;
        fable_u32 m_uForegroundChunkSize;
        fable_u32 m_BoundsMinX;
        fable_u32 m_BoundsMinY;
        fable_u32 m_BoundsMinZ;
        fable_u32 m_BoundsMaxX;
        fable_u32 m_BoundsMaxY;
        fable_u32 m_BoundsMaxZ;
    };

    union HeaderBuffer
    {
        fable_u32 m_Words[9];
        fable_u8 m_Bytes[0x24];
    };

    FABLE_STATIC_ASSERT(offsetof(CDataInputStreamLayout, m_uBytesRead) == 0x04);
    FABLE_STATIC_ASSERT(offsetof(CDataInputStreamLayout, m_pCursor32) == 0x0C);
    FABLE_STATIC_ASSERT(offsetof(CDataInputStreamLayout, m_nBytesRemaining) == 0x14);
    FABLE_STATIC_ASSERT(sizeof(HeaderBuffer) == 0x24);

    CDataInputStreamLayout& input = reinterpret_cast<CDataInputStreamLayout&>(stream);
    CEngineLandscapePatchHeaderWords& headerFields =
        *reinterpret_cast<CEngineLandscapePatchHeaderWords*>(reinterpret_cast<fable_u8*>(this) + 0x18);

    HeaderBuffer header;
    header.m_Words[0] = 0;
    header.m_Words[1] = 0;
    header.m_Bytes[0x20] = 0;

    fable_u8 foregroundEnabled = 0;
    const int bytesRead = static_cast<int>(input.m_uBytesRead);

    if (static_cast<fable_u32>(bytesRead) + 0x24U < 0x80000000U)
    {
        if (input.m_nBytesRemaining < 0x24)
        {
            CDataInputStream::ReadWithSrcChunkOverflow(
                reinterpret_cast<CDataInputStream*>(&stream),
                header.m_Bytes,
                0x24);
            foregroundEnabled = header.m_Bytes[0x20];
        }
        else
        {
            fable_u32* pSrc = input.m_pCursor32;
            fable_u32* pDst = header.m_Words;

            for (int count = 9; count != 0; --count)
            {
                *pDst = *pSrc;
                ++pSrc;
                ++pDst;
            }

            input.m_pCursor32 = pSrc;
            input.m_nBytesRemaining -= 0x24;
            input.m_uBytesRead = static_cast<fable_u32>(bytesRead) + 0x24U;
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
    m_Flags = static_cast<fable_u8>(m_Flags ^ (((foregroundEnabled << 4) ^ m_Flags) & 0x10));
}
