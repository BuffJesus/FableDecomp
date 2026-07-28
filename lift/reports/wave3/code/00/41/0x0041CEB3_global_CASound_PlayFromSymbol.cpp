#include <cstddef>
#include <cstdint>

struct CCharString;
struct CASoundBank;

struct CASound_PlayFromSymbol_This {
    std::byte pad_00[0x64];
    std::int32_t field_64;
};
static_assert(offsetof(CASound_PlayFromSymbol_This, field_64) == 0x64);

struct CSoundInit {
    std::byte pad_00[0x14];
    std::uint32_t field_14;
    std::byte pad_18[0x08];

    CSoundInit();
};
static_assert(offsetof(CSoundInit, field_14) == 0x14);
static_assert(sizeof(CSoundInit) == 0x20);

struct CASoundBank {
    long __thiscall GetSoundIndexFromSymbol(CCharString* symbol);
};

extern void* DAT_013b8394;
extern CASoundBank* __cdecl MessageMap__FindMessageByID(std::uint32_t messageId);

std::uint32_t __thiscall CASound_PlayFromSymbol(
    CASound_PlayFromSymbol_This* self,
    CCharString* symbol,
    std::uint32_t messageId)
{
    if (DAT_013b8394 == nullptr) {
        return 0;
    }

    if (self->field_64 != 0) {
        return 0;
    }

    CASoundBank* const soundBank = MessageMap__FindMessageByID(messageId);
    if (soundBank == nullptr) {
        return 0;
    }

    const long soundIndex = soundBank->GetSoundIndexFromSymbol(symbol);
    if (soundIndex == 0) {
        return 0;
    }

    CSoundInit soundInit;
    soundInit.field_14 = 0x40;

    using AcquirePlaybackIfaceFn = void* (__thiscall*)(void* self_);
    auto* const acquirePlaybackIface =
        reinterpret_cast<AcquirePlaybackIfaceFn>((*reinterpret_cast<void***>(DAT_013b8394))[0x24 / sizeof(void*)]);
    void* const playbackIface = acquirePlaybackIface(DAT_013b8394);

    using PlayFromBankFn =
        std::uint32_t (__thiscall*)(void* self_, CASoundBank* bank, long index, std::uint32_t one, CSoundInit* init, std::uint32_t zero);
    auto* const playFromBank =
        reinterpret_cast<PlayFromBankFn>((*reinterpret_cast<void***>(playbackIface))[0x0C / sizeof(void*)]);

    return playFromBank(playbackIface, soundBank, soundIndex, 1, &soundInit, 0);
}