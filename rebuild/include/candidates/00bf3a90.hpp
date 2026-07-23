#pragma once

// Strict declaration envelope for the auto-RE snapshot at 0x00BF3A90.
// The byte layout is taken from the recovered CEngineLandscapePatch layout.
class CMemoryDataInputStream
{
};

class CDataInputStream
{
public:
    static void ReadWithSrcChunkOverflow(CDataInputStream* stream, uchar* destination, int byteCount);
};

class CEngineLandscapePatch
{
private:
    std::byte m_Pad00[0x3C];

public:
    std::uint8_t m_Flags;
    std::byte m_Pad3D[0x0B];

    void LoadHeader(CMemoryDataInputStream& stream);
};

static_assert(offsetof(CEngineLandscapePatch, m_Flags) == 0x3C);
static_assert(sizeof(CEngineLandscapePatch) == 0x48);
