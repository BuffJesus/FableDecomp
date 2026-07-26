#include <cstddef>
#include <cstdint>

namespace
{
struct CCreatureAction_FireMissileWeapon_Overlay
{
    std::uint8_t bytes[0x01];
};

struct Weapon_ProcessAnimFrameIfReady_Overlay
{
    std::uint8_t pad00[0x54];
    CCreatureAction_FireMissileWeapon_Overlay fireMissileWeapon54; // +0x54
};

namespace NPlayerGui
{
struct CDrawAutoPickups
{
    std::uint8_t bytes[0x01];

    void __thiscall ClearPrimitivesIfNecessary(std::int32_t createAudioBufferResult);
};
}

extern std::uint32_t __stdcall CShaderConstantRange_ShouldClearPrimitives(void* value);
extern int __thiscall CModeBlock_CreateAudioBuffer(void* self);
extern std::uint32_t __thiscall CCreatureAction_FireMissileWeapon__ProcessAnimFrame(
    CCreatureAction_FireMissileWeapon_Overlay* self,
    void* param_1,
    std::uint32_t param_2,
    std::uint32_t param_3);

extern NPlayerGui::CDrawAutoPickups DAT_0143bb58;

static_assert(offsetof(Weapon_ProcessAnimFrameIfReady_Overlay, fireMissileWeapon54) == 0x54);
}

std::uint32_t __thiscall Weapon_ProcessAnimFrameIfReady(
    Weapon_ProcessAnimFrameIfReady_Overlay* self,
    void* param_1,
    std::uint32_t param_2,
    std::uint32_t param_3)
{
    std::uint32_t result = CShaderConstantRange_ShouldClearPrimitives(param_1);
    if (static_cast<char>(result) != 0) {
        const int createAudioBufferResult = CModeBlock_CreateAudioBuffer(param_1);
        if (-1 < createAudioBufferResult) {
            return CCreatureAction_FireMissileWeapon__ProcessAnimFrame(
                &self->fireMissileWeapon54,
                param_1,
                param_2,
                param_3);
        }

        DAT_0143bb58.ClearPrimitivesIfNecessary(createAudioBufferResult);
        __asm {
            mov result, eax
        }
    }

    return result & 0xFFFFFF00u;
}