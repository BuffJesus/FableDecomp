#include <cstddef>
#include <cstdint>

void CProcessedInput::AddGameEvent(CGameEvent const& event)
{
    static_assert(offsetof(CProcessedInput, Player) == 0x00);
    static_assert(offsetof(CProcessedInput, Type) == 0x04);
    static_assert(offsetof(CProcessedInput, GameEvents) == 0x08);
    static_assert(offsetof(CProcessedInput, NoGameEvents) == 0xA8);
    static_assert(offsetof(CProcessedInput, Priority) == 0xAC);
    static_assert(sizeof(CProcessedInput) == 0xB0);

    struct CGameEventOverlay
    {
        std::uint32_t dword[10];
    };
    static_assert(sizeof(CGameEventOverlay) == 0x28);

    Type = static_cast<EProcessedEventType>(1);

    const auto index = static_cast<std::uint8_t>(NoGameEvents);
    auto* dst = reinterpret_cast<CGameEventOverlay*>(
        reinterpret_cast<std::uint8_t*>(this) + 0x08 + static_cast<std::size_t>(index) * 0x28
    );
    const auto* src = reinterpret_cast<CGameEventOverlay const*>(&event);

    for (int i = 0; i != 10; ++i)
    {
        dst->dword[i] = src->dword[i];
    }

    *(reinterpret_cast<std::uint8_t*>(this) + 0x0C + static_cast<std::size_t>(index) * 0x28) =
        static_cast<std::uint8_t>(Player);

    NoGameEvents = static_cast<unsigned char>(static_cast<unsigned char>(NoGameEvents) + 1);
}