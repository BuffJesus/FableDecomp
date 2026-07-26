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
static_assert(offsetof(CGameEventOverlay, HasHighBit) == 0x27);

struct CGameEventPackageSetOverlay {
    std::uint32_t NoPackages;
    CGameEventPackageOverlay Packages[50];
};
static_assert(sizeof(CGameEventPackageSetOverlay) == 80404);
static_assert(offsetof(CGameEventPackageSetOverlay, NoPackages) == 0x0);
static_assert(offsetof(CGameEventPackageSetOverlay, Packages) == 0x4);

long CGameEventPackageSet::CompressIntoBuffer(unsigned char* buffer) const {
    auto const* set = reinterpret_cast<CGameEventPackageSetOverlay const*>(this);

    unsigned char* out = buffer;
    *out = static_cast<unsigned char>(set->NoPackages);
    ++out;

    std::uint32_t packageIndex = 0;
    if (set->NoPackages != 0) {
        auto const* package = &set->Packages[0];
        do {
            *out = static_cast<unsigned char>(package->EventCount);

            *reinterpret_cast<std::uint32_t*>(out + 1) = package->Unknown0x00;
            out += 5;

            std::uint32_t eventIndex = 0;
            if (package->EventCount != 0) {
                auto const* event =
                    reinterpret_cast<CGameEventOverlay const*>(reinterpret_cast<std::byte const*>(package) + 0x8);

                do {
                    std::uint16_t eventId = event->EventId;
                    if (event->HasHighBit != 0) {
                        eventId = static_cast<std::uint16_t>(eventId | 0x8000);
                    }

                    *reinterpret_cast<std::uint16_t*>(out) = eventId;
                    out[2] = event->Type;

                    const std::uint8_t payloadSize = event->PayloadSize;
                    out[3] = payloadSize;

                    auto const* src = &event->Payload[0];
                    auto* dst = out + 4;

                    for (std::uint32_t wordCount = static_cast<std::uint32_t>(payloadSize >> 2);
                         wordCount != 0;
                         --wordCount) {
                        *reinterpret_cast<std::uint32_t*>(dst) =
                            *reinterpret_cast<std::uint32_t const*>(src);
                        src += 4;
                        dst += 4;
                    }

                    for (std::uint32_t byteCount = static_cast<std::uint32_t>(payloadSize & 3);
                         byteCount != 0;
                         --byteCount) {
                        *dst = *src;
                        ++src;
                        ++dst;
                    }

                    out += 4 + payloadSize;
                    ++eventIndex;
                    event = reinterpret_cast<CGameEventOverlay const*>(
                        reinterpret_cast<std::byte const*>(event) + 0x28);
                } while (eventIndex < package->EventCount);
            }

            ++packageIndex;
            package = reinterpret_cast<CGameEventPackageOverlay const*>(
                reinterpret_cast<std::byte const*>(package) + 0x648);
        } while (packageIndex < set->NoPackages);
    }

    return static_cast<long>(out - buffer);
}