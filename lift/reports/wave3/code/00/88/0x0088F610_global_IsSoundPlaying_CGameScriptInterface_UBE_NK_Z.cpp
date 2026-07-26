#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    bool __thiscall IsSoundPlaying(unsigned long soundId) const;
};

namespace
{
struct CGameScriptInterface_SoundProvider
{
    void** vtable; // 0x00
};

static_assert(offsetof(CGameScriptInterface_SoundProvider, vtable) == 0x00);

extern CGameScriptInterface_SoundProvider* DAT_013B8394;

using Fn_IsSoundPlaying = bool (__thiscall*)(CGameScriptInterface_SoundProvider* self, unsigned long soundId);
}

bool __thiscall CGameScriptInterface::IsSoundPlaying(unsigned long soundId) const
{
    auto* const soundProvider = DAT_013B8394;
    const auto isSoundPlaying =
        reinterpret_cast<Fn_IsSoundPlaying>(soundProvider->vtable[0x3C / sizeof(void*)]);

    return isSoundPlaying(soundProvider, soundId);
}