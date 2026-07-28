#include <cstddef>
#include <cstdint>

struct CSoundInit {
    CSoundInit();
};

extern void* DAT_013b8394;
extern std::uintptr_t DAT_013b871c;

extern std::uintptr_t MessageMap__FindMessageByID(std::uint32_t messageId);

struct CASound_PlayMessage_MessageSource_050 {
    std::byte pad_00[0x50];
    std::uintptr_t field_50;
};
static_assert(offsetof(CASound_PlayMessage_MessageSource_050, field_50) == 0x50);

struct CASound_PlayMessage_CSoundInitRaw_014 {
    alignas(CSoundInit) std::byte sound_init_storage[0x14];
    std::uint32_t field_14;
};
static_assert(offsetof(CASound_PlayMessage_CSoundInitRaw_014, field_14) == 0x14);

void __stdcall CASound_PlayMessage(std::uint32_t param_1, std::uint32_t param_2)
{
    void* sound_root = DAT_013b8394;
    if (sound_root == nullptr) {
        return;
    }

    CASound_PlayMessage_CSoundInitRaw_014 local_24;
    auto* const sound_init = reinterpret_cast<CSoundInit*>(local_24.sound_init_storage);
    sound_init->CSoundInit::CSoundInit();
    local_24.field_14 = 0x20;

    std::uintptr_t message;
    void* player;

    if (DAT_013b871c == 0) {
        message = MessageMap__FindMessageByID(param_2);
        if (message == 0) {
            return;
        }

        using GetPlayerFn = void* (__thiscall*)(void*);
        player = reinterpret_cast<GetPlayerFn>(
            *reinterpret_cast<std::uintptr_t*>(
                *reinterpret_cast<std::uintptr_t*>(sound_root) + 0x24))(sound_root);
    } else {
        message =
            reinterpret_cast<CASound_PlayMessage_MessageSource_050*>(DAT_013b871c)->field_50;
        if (message == 0) {
            return;
        }

        using GetPlayerFn = void* (__thiscall*)(void*);
        player = reinterpret_cast<GetPlayerFn>(
            *reinterpret_cast<std::uintptr_t*>(
                *reinterpret_cast<std::uintptr_t*>(sound_root) + 0x24))(sound_root);
    }

    using PlayFn =
        void (__thiscall*)(void*, std::uintptr_t, std::uint32_t, int, CSoundInit*, int);
    reinterpret_cast<PlayFn>(
        *reinterpret_cast<std::uintptr_t*>(
            *reinterpret_cast<std::uintptr_t*>(player) + 0x10))(
                player, message, param_1, 1, sound_init, 0);
}