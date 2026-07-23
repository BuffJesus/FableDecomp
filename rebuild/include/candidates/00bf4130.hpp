#pragma once

extern std::uint8_t* DAT_01436e98;
extern std::uint8_t* DAT_01436ea8;

class CEngineLandscapeRenderer
{
public:
    static void AddFrameForegroundTexturesUsed(
        std::uint8_t* renderer,
        std::uint16_t* textureIndices,
        std::uint32_t textureCount,
        float distanceSquared);
};

class CEngineLandscapePatch
{
public:
    std::byte m_Pad00[0x0C];
    std::uint16_t* m_pForegroundTextureIndices;
    std::uint16_t m_nForegroundTextureCount;
    std::byte m_Pad12[0x2A];
    std::uint8_t m_Flags;
    std::byte m_Pad3D[0x0B];

    bool LoadTextures(float distanceSquared);
};

static_assert(offsetof(CEngineLandscapePatch, m_pForegroundTextureIndices) == 0x0C);
static_assert(offsetof(CEngineLandscapePatch, m_nForegroundTextureCount) == 0x10);
static_assert(offsetof(CEngineLandscapePatch, m_Flags) == 0x3C);
static_assert(sizeof(CEngineLandscapePatch) == 0x48);
