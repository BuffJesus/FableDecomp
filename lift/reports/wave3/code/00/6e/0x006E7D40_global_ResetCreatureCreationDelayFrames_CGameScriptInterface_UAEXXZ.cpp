#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    virtual void ResetCreatureCreationDelayFrames();
};

struct CGameScriptInterface_Overlay_0x4C
{
    std::byte pad_00[0x4C];
    std::int32_t field_4C;
};

static_assert(offsetof(CGameScriptInterface_Overlay_0x4C, field_4C) == 0x4C);

void CGameScriptInterface::ResetCreatureCreationDelayFrames()
{
    auto* const pOverlay =
        reinterpret_cast<CGameScriptInterface_Overlay_0x4C*>(this);

    pOverlay->field_4C = 0;
}