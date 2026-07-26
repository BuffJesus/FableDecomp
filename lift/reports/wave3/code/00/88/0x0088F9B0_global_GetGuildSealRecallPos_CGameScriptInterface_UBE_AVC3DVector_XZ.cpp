#include <cstddef>
#include <cstdint>

class C3DVector
{
public:
    float x;
    float y;
    float z;
};

static_assert(sizeof(C3DVector) == 0x0C);

class CGameScriptInterface
{
public:
    C3DVector GetGuildSealRecallPos() const;
};

struct CGameScriptInterface_Overlay
{
    std::byte pad_0x00[0x04];
    void* field_0x04;
};

static_assert(offsetof(CGameScriptInterface_Overlay, field_0x04) == 0x04);

struct GuildSealRecallPosSource_Overlay
{
    std::byte pad_0x00[0x154];
    C3DVector field_0x154;
};

static_assert(offsetof(GuildSealRecallPosSource_Overlay, field_0x154) == 0x154);

C3DVector CGameScriptInterface::GetGuildSealRecallPos() const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    const auto* const source = reinterpret_cast<const GuildSealRecallPosSource_Overlay*>(self->field_0x04);
    return source->field_0x154;
}