#include <cstddef>
#include <cstdint>

class CTCDParticleEmitter
{
public:
    static void __thiscall EditFrameUpdate(CTCDParticleEmitter* emitter, bool enabled);
};

class CGameScriptInterface
{
public:
    void __thiscall EnableSounds(bool enabled) const;
};

extern CTCDParticleEmitter* DAT_013b8394;
extern std::uint8_t DAT_0137e974;

namespace
{
using CTCDParticleEmitterVFunc40 = void(__thiscall*)(CTCDParticleEmitter* emitter);

struct CTCDParticleEmitterVftableOverlay
{
    std::byte pad00[0x40];
    CTCDParticleEmitterVFunc40 slot40; // 0x40
};

static_assert(offsetof(CTCDParticleEmitterVftableOverlay, slot40) == 0x40);

struct CTCDParticleEmitterOverlay
{
    CTCDParticleEmitterVftableOverlay* vftable; // 0x00
};

static_assert(offsetof(CTCDParticleEmitterOverlay, vftable) == 0x00);
}

void __thiscall CGameScriptInterface::EnableSounds(bool enabled) const
{
    (void)this;

    CTCDParticleEmitter* const emitter = DAT_013b8394;
    if (emitter != nullptr)
    {
        if (enabled)
        {
            CTCDParticleEmitter::EditFrameUpdate(emitter, true);
            DAT_0137e974 = 1;
            return;
        }

        const auto* const emitterOverlay =
            reinterpret_cast<CTCDParticleEmitterOverlay*>(emitter);
        emitterOverlay->vftable->slot40(emitter);

        CTCDParticleEmitter::EditFrameUpdate(DAT_013b8394, false);
        DAT_0137e974 = 0;
    }
}