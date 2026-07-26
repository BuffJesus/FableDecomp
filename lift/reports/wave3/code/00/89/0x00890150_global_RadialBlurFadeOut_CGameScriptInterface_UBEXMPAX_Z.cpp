#include <cstddef>
#include <cstdint>

struct CGameScriptInterface_Field04
{
    std::byte Pad0[0x18];
    void* Field18;
};

static_assert(offsetof(CGameScriptInterface_Field04, Field18) == 0x18);

struct CGameScriptInterface_Field04_Field18
{
    std::byte Pad0[0x1964];
    void* Field1964;
};

static_assert(offsetof(CGameScriptInterface_Field04_Field18, Field1964) == 0x1964);

class CGameScriptInterface
{
public:
    std::byte Pad0[0x04];
    CGameScriptInterface_Field04* Field04;

    void __thiscall RadialBlurFadeOut(float fadeOutTime, void* pArg);
};

static_assert(offsetof(CGameScriptInterface, Field04) == 0x04);

using TRadialBlurFadeOutImpl = void(__thiscall*)(void*, float, void*);

void CGameScriptInterface::RadialBlurFadeOut(float fadeOutTime, void* pArg)
{
    auto* const field18 =
        reinterpret_cast<CGameScriptInterface_Field04_Field18*>(this->Field04->Field18);

    reinterpret_cast<TRadialBlurFadeOutImpl>(0x006B82E0)(
        field18->Field1964,
        fadeOutTime,
        pArg);
}