#include <cstddef>
#include <cstdint>

extern "C" std::int32_t __cdecl __ftol2();

class CGameScriptInterface
{
public:
    void __thiscall CameraResetToViewBehindHero(float time);
};

namespace
{
struct CGameScriptInterface_CameraResetToViewBehindHero_Overlay
{
    std::byte m_Pad00[0x48];
    std::int32_t m_Field48;
};

static_assert(offsetof(CGameScriptInterface_CameraResetToViewBehindHero_Overlay, m_Field48) == 0x48);

inline std::int32_t* const g_CameraResetToViewBehindHeroScale =
    reinterpret_cast<std::int32_t*>(0x01375550);
}

void __thiscall CGameScriptInterface::CameraResetToViewBehindHero(float time)
{
    auto* const self = reinterpret_cast<CGameScriptInterface_CameraResetToViewBehindHero_Overlay*>(this);

    std::int32_t value;
    __asm
    {
        mov eax, g_CameraResetToViewBehindHeroScale
        fild dword ptr [eax]
        fmul dword ptr [time]
        call __ftol2
        mov value, eax
    }

    self->m_Field48 = value;
    if (value < 0)
    {
        self->m_Field48 = 0;
    }
}