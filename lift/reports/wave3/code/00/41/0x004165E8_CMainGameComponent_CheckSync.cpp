#include <cstddef>
#include <cstdint>

class CGameEventPackage;

namespace
{
struct CheckSyncEventTypeOverlay
{
    std::int32_t Type;
};

static_assert(offsetof(CheckSyncEventTypeOverlay, Type) == 0x00);

struct CheckSyncBuffer
{
    std::byte Storage[0x20];
};

static_assert(sizeof(CheckSyncBuffer) == 0x20);

using CheckSync_GetPackageLimitFn = std::uint32_t(__thiscall*)(CGameEventPackage*);
using CheckSync_GetPackageEntryFn = void*(__thiscall*)(CGameEventPackage*, std::uint32_t);
using CheckSync_CopyToBufferFn = void(__thiscall*)(void*, CheckSyncBuffer*);
using CheckSync_BufferReadFn = void(__thiscall*)(CheckSyncBuffer*, std::int32_t, void*);

constexpr auto CheckSync_GetPackageLimit =
    reinterpret_cast<CheckSync_GetPackageLimitFn>(0x009F16D0);

constexpr auto CheckSync_GetPackageEntry =
    reinterpret_cast<CheckSync_GetPackageEntryFn>(0x009F16E0);

constexpr auto CheckSync_CopyToBuffer =
    reinterpret_cast<CheckSync_CopyToBufferFn>(0x009F17C0);

constexpr auto CheckSync_BufferRead =
    reinterpret_cast<CheckSync_BufferReadFn>(0x009F1880);
}

void CMainGameComponent::CheckSync(CGameEventPackage* package)
{
    std::uint32_t event_index = 0;
    std::uint32_t checksum1 = 0;
    std::uint32_t checksum2 = 0;
    std::uint32_t frame = 0;

    PeekWorldChecksum();

    std::uint32_t event_limit = CheckSync_GetPackageLimit(package);
    if (event_limit != 0) {
        do {
            void* const event_entry = CheckSync_GetPackageEntry(package, event_index);
            if (reinterpret_cast<CheckSyncEventTypeOverlay*>(event_entry)->Type == 1) {
                CheckSyncBuffer buffer;

                CheckSync_CopyToBuffer(event_entry, &buffer);
                CheckSync_BufferRead(&buffer, 4, &checksum1);
                CheckSync_BufferRead(&buffer, 4, &checksum2);
                CheckSync_BufferRead(&buffer, 4, &frame);
            }

            ++event_index;
            event_limit = CheckSync_GetPackageLimit(package);
        } while (event_index < event_limit);
    }
}