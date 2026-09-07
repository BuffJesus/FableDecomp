#include <cstddef>
#include <cstdint>

struct AllocationAreaNodeOverlay {
    std::byte pad00[0x08];
    std::uint32_t next;    // 0x08
    std::byte pad0C[0x04];
    std::uint32_t value10; // 0x10
    std::uint32_t value14; // 0x14
};

static_assert(offsetof(AllocationAreaNodeOverlay, next) == 0x08);
static_assert(offsetof(AllocationAreaNodeOverlay, value10) == 0x10);
static_assert(offsetof(AllocationAreaNodeOverlay, value14) == 0x14);

extern "C" std::uint32_t __cdecl sub_00429300(std::uint32_t current);

void __fastcall SerializeAllocationAreas(CDataOutputStream* stream, unsigned long* allocationChain)
{
    stream->WriteCBYTE(static_cast<char>(static_cast<std::uint8_t>(allocationChain[1])));

    const auto sentinel = static_cast<std::uint32_t>(allocationChain[0]);
    auto current =
        reinterpret_cast<const AllocationAreaNodeOverlay*>(static_cast<std::uintptr_t>(sentinel))->next;

    if (current != sentinel) {
        do {
            const auto* const node =
                reinterpret_cast<const AllocationAreaNodeOverlay*>(static_cast<std::uintptr_t>(current));

            stream->WriteCBYTE(static_cast<char>(static_cast<std::uint8_t>(node->value10)));
            stream->WriteCBYTE(static_cast<char>(static_cast<std::uint8_t>(node->value14)));
            current = sub_00429300(current);
        } while (current != sentinel);
    }
}