#include <cstddef>
#include <cstdint>

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
static_assert(sizeof(CGameEventPackageSetOverlay) == 0x13a14);
static_assert(offsetof(CGameEventPackageSetOverlay, NoPackages) == 0x0);
static_assert(offsetof(CGameEventPackageSetOverlay, Packages) == 0x4);

void CGameEventPackageSet::AddPackage(CGameEventPackage const* package)
{
    auto* const set = reinterpret_cast<CGameEventPackageSetOverlay*>(this);
    auto* destination = reinterpret_cast<std::uint32_t*>(
        reinterpret_cast<std::byte*>(this) + set->NoPackages * 0x648 + 0x4);
    auto const* source = reinterpret_cast<std::uint32_t const*>(package);

    for (int i = 0x192; i != 0; --i) {
        *destination = *source;
        ++source;
        ++destination;
    }

    set->NoPackages = set->NoPackages + 1;
}