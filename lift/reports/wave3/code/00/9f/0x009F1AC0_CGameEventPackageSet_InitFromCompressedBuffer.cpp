#include <cstddef>
#include <cstdint>

struct CGameEventOverlay {
    std::uint16_t EventId;
    std::byte Pad0x02[0x2];
    std::uint8_t Type;
    std::uint8_t Payload[0x20];
    std::uint8_t PayloadSize;
    std::uint8_t Unknown0x26;
    std::uint8_t HasHighBit;
};
static_assert(sizeof(CGameEventOverlay) == 0x28);
static_assert(offsetof(CGameEventOverlay, EventId) == 0x0);
static_assert(offsetof(CGameEventOverlay, Type) == 0x4);
static_assert(offsetof(CGameEventOverlay, Payload) == 0x5);
static_assert(offsetof(CGameEventOverlay, PayloadSize) == 0x25);
static_assert(offsetof(CGameEventOverlay, Unknown0x26) == 0x26);
static_assert(offsetof(CGameEventOverlay, HasHighBit) == 0x27);

struct CGameEventPackageOverlay {
    std::uint32_t Unknown0x00;
    std::uint32_t EventCount;
    std::byte RawEvents[0x640];
};
static_assert(sizeof(CGameEventPackageOverlay) == 0x648);
static_assert(offsetof(CGameEventPackageOverlay, Unknown0x00) == 0x0);
static_assert(offsetof(CGameEventPackageOverlay, EventCount) == 0x4);
static_assert(offsetof(CGameEventPackageOverlay, RawEvents) == 0x8);

struct CGameEventPackageSetOverlay {
    std::uint32_t NoPackages;
    CGameEventPackageOverlay Packages[50];
};
static_assert(sizeof(CGameEventPackageSetOverlay) == 80404);
static_assert(offsetof(CGameEventPackageSetOverlay, NoPackages) == 0x0);
static_assert(offsetof(CGameEventPackageSetOverlay, Packages) == 0x4);

void __thiscall CGameEventPackageSet::InitFromCompressedBuffer(unsigned char const* buffer)
{
    CGameEventOverlay eventScratch;
    CGameEventPackageOverlay packageScratch;

    eventScratch.Type = 0xFF;
    eventScratch.Unknown0x26 = 0;

    auto* eventSlotUnknown0x26 = reinterpret_cast<std::uint8_t*>(packageScratch.RawEvents) + 0x26;
    for (int i = 0x28; i != 0; --i) {
        eventSlotUnknown0x26[-0x22] = 0xFF;
        *eventSlotUnknown0x26 = 0;
        eventSlotUnknown0x26 += 0x28;
    }

    auto* const set = reinterpret_cast<CGameEventPackageSetOverlay*>(this);
    set->NoPackages = 0;

    auto* pbVar8 = buffer + 1;
    for (std::uint32_t uVar4 = static_cast<std::uint32_t>(*buffer); uVar4 != 0; --uVar4) {
        const std::uint8_t bVar2 = *pbVar8;
        auto* const pbVar1 = pbVar8 + 1;
        pbVar8 = pbVar8 + 5;

        packageScratch.EventCount = 0;
        packageScratch.Unknown0x00 = *reinterpret_cast<std::uint32_t const*>(pbVar1);

        for (std::uint32_t uVar5 = static_cast<std::uint32_t>(bVar2); uVar5 != 0; --uVar5) {
            const long lVar6 =
                reinterpret_cast<CGameEvent*>(&eventScratch)->InitFromCompressedBuffer(pbVar8);
            pbVar8 = pbVar8 + lVar6;
            reinterpret_cast<CGameEventPackage*>(&packageScratch)
                ->AddEvent(reinterpret_cast<CGameEvent*>(&eventScratch));
        }

        auto* puVar9 = reinterpret_cast<std::uint32_t*>(&packageScratch);
        auto* pCVar10 = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::byte*>(this) + set->NoPackages * 0x648 + 0x4);
        for (int iVar7 = 0x192; iVar7 != 0; --iVar7) {
            *pCVar10 = *puVar9;
            ++puVar9;
            ++pCVar10;
        }

        set->NoPackages = set->NoPackages + 1;
    }
}