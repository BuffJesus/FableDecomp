#include <cstddef>
#include <cstdint>

namespace
{
    struct AudioCheckBufferRegionActive_GridSourceOverlay
    {
        std::byte pad_00[0x08];
        std::int32_t cellSpan;          // +0x08
        std::byte pad_0C[0x94];
        std::uint32_t gridAddress;      // +0xA0
        std::byte pad_A4[0x08];
        std::int32_t activeCell;        // +0xAC
    };
    static_assert(offsetof(AudioCheckBufferRegionActive_GridSourceOverlay, cellSpan) == 0x08);
    static_assert(offsetof(AudioCheckBufferRegionActive_GridSourceOverlay, gridAddress) == 0xA0);
    static_assert(offsetof(AudioCheckBufferRegionActive_GridSourceOverlay, activeCell) == 0xAC);

    struct AudioCheckBufferRegionActive_SourceOverlay
    {
        std::byte pad_00[0x1C];
        std::uint32_t valuesAddress;    // +0x1C
    };
    static_assert(offsetof(AudioCheckBufferRegionActive_SourceOverlay, valuesAddress) == 0x1C);

    struct AudioCheckBufferRegionActive_ParamOverlay
    {
        std::byte pad_00[0x04];
        std::uint32_t sourceAddress;    // +0x04
        std::byte pad_08[0x1C];
        std::int32_t index24;           // +0x24
        std::int32_t index28;           // +0x28
        std::int32_t index2C;           // +0x2C
        std::int32_t center30;          // +0x30
        std::byte pad_34[0x34];
        std::uint32_t gridSourcePtrAddress; // +0x68
    };
    static_assert(offsetof(AudioCheckBufferRegionActive_ParamOverlay, sourceAddress) == 0x04);
    static_assert(offsetof(AudioCheckBufferRegionActive_ParamOverlay, index24) == 0x24);
    static_assert(offsetof(AudioCheckBufferRegionActive_ParamOverlay, index28) == 0x28);
    static_assert(offsetof(AudioCheckBufferRegionActive_ParamOverlay, index2C) == 0x2C);
    static_assert(offsetof(AudioCheckBufferRegionActive_ParamOverlay, center30) == 0x30);
    static_assert(offsetof(AudioCheckBufferRegionActive_ParamOverlay, gridSourcePtrAddress) == 0x68);

    [[nodiscard]] static std::int32_t QuarterRoundTowardZero(const std::int32_t value)
    {
        return (value + ((value >> 31) & 3)) >> 2;
    }
}

std::int32_t __fastcall Audio_CheckBufferRegionActive(void* const self)
{
    auto* const a = static_cast<AudioCheckBufferRegionActive_ParamOverlay*>(self);

    auto* const source =
        reinterpret_cast<AudioCheckBufferRegionActive_SourceOverlay*>(
            static_cast<std::uintptr_t>(a->sourceAddress));
    auto* const values =
        reinterpret_cast<std::int32_t*>(
            static_cast<std::uintptr_t>(source->valuesAddress));

    auto* const gridSourcePtr =
        reinterpret_cast<const std::uint32_t*>(
            static_cast<std::uintptr_t>(a->gridSourcePtrAddress));
    auto* const gridSource =
        reinterpret_cast<AudioCheckBufferRegionActive_GridSourceOverlay*>(
            static_cast<std::uintptr_t>(*gridSourcePtr));

    const std::int32_t iVar2 = QuarterRoundTowardZero(values[a->index28]);

    std::int32_t iVar3;
    std::int32_t iVar5;
    if (a->index28 == 0)
    {
        iVar3 = QuarterRoundTowardZero(values[0]);
        iVar5 = iVar3;
    }
    else
    {
        iVar3 = QuarterRoundTowardZero(values[a->index2C]);
        iVar5 = QuarterRoundTowardZero(values[a->index24]);
    }

    iVar5 = (a->center30 - iVar2) - iVar5;
    iVar3 = a->center30 + iVar2 + iVar3;

    if ((iVar5 <= gridSource->activeCell) && (gridSource->activeCell < iVar3))
    {
        return 1;
    }

    iVar3 = iVar3 / gridSource->cellSpan;
    iVar5 = iVar5 / gridSource->cellSpan;

    if (iVar5 < iVar3)
    {
        auto* gridEntry =
            reinterpret_cast<std::int32_t*>(
                static_cast<std::uintptr_t>(gridSource->gridAddress)) + iVar5;
        do
        {
            if (*gridEntry != 0)
            {
                return 1;
            }

            ++iVar5;
            ++gridEntry;
        } while (iVar5 < iVar3);
    }

    return 0;
}