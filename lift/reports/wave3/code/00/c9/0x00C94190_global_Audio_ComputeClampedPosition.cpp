#include <cstddef>
#include <cstdint>

namespace
{
    struct AudioComputeClampedPosition_SourceOverlay
    {
        std::byte pad_0000[0x08];
        std::int32_t sampleRate;
        std::byte pad_000C[0x10];
        std::int32_t tableBase;
    };
    static_assert(offsetof(AudioComputeClampedPosition_SourceOverlay, sampleRate) == 0x08);
    static_assert(offsetof(AudioComputeClampedPosition_SourceOverlay, tableBase) == 0x1C);

    struct AudioComputeClampedPosition_ThisOverlay
    {
        std::byte pad_0000[0x04];
        AudioComputeClampedPosition_SourceOverlay* source;
        std::byte pad_0008[0x20];
        std::int32_t tableIndex;
    };
    static_assert(offsetof(AudioComputeClampedPosition_ThisOverlay, source) == 0x04);
    static_assert(offsetof(AudioComputeClampedPosition_ThisOverlay, tableIndex) == 0x28);

    constexpr std::ptrdiff_t kTableScaleOffset = 0x0B78;

    extern const float _DAT_012bf314;
}

long double __thiscall Audio_ComputeClampedPosition(void* self, float position)
{
    const auto* const audio = static_cast<const AudioComputeClampedPosition_ThisOverlay*>(self);
    const auto* const source = audio->source;
    const auto tableBase = static_cast<std::uintptr_t>(source->tableBase);

    const std::int32_t halfValue =
        *reinterpret_cast<const std::int32_t*>(tableBase + static_cast<std::uintptr_t>(audio->tableIndex) * 4) / 2;

    long double result =
        (static_cast<long double>(halfValue) / static_cast<long double>(source->sampleRate)) *
            static_cast<long double>(*reinterpret_cast<const float*>(tableBase + kTableScaleOffset)) +
        static_cast<long double>(position);

    if (result < static_cast<long double>(_DAT_012bf314)) {
        result = static_cast<long double>(_DAT_012bf314);
    }

    return result;
}