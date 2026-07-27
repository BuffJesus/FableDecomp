#include <cstddef>
#include <cstdint>
#include <windows.h>

struct GetBufferOffset_Locked_Overlay {
    std::byte pad_00[0x20];
    std::int32_t value_20;
    std::byte pad_24[0x08];
    std::int32_t value_2C;
    std::int32_t value_30;
};

static_assert(offsetof(GetBufferOffset_Locked_Overlay, value_20) == 0x20);
static_assert(offsetof(GetBufferOffset_Locked_Overlay, value_2C) == 0x2C);
static_assert(offsetof(GetBufferOffset_Locked_Overlay, value_30) == 0x30);

std::uint32_t __stdcall GetBufferOffset_Locked(void* self, std::int32_t* outOffset)
{
    const auto selfValue = reinterpret_cast<std::uintptr_t>(self);

    LPCRITICAL_SECTION const criticalSection =
        (selfValue == 0x0Cu)
            ? static_cast<LPCRITICAL_SECTION>(nullptr)
            : reinterpret_cast<LPCRITICAL_SECTION>(selfValue + 0x04u);

    EnterCriticalSection(criticalSection);

    const auto* const overlay =
        reinterpret_cast<const GetBufferOffset_Locked_Overlay*>(selfValue);

    *outOffset = (overlay->value_2C - overlay->value_30) + overlay->value_20;

    LeaveCriticalSection(criticalSection);
    return 0;
}