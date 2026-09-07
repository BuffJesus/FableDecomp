#include <cstddef>
#include <cstdint>

struct AllocationAreaNodeOverlay {
    std::byte _pad0[0x08];
    AllocationAreaNodeOverlay* next;
    std::byte _padC[0x04];
    std::uint32_t value10;
    std::uint32_t value14;
};

static_assert(offsetof(AllocationAreaNodeOverlay, next) == 0x08);
static_assert(offsetof(AllocationAreaNodeOverlay, value10) == 0x10);
static_assert(offsetof(AllocationAreaNodeOverlay, value14) == 0x14);

struct AllocationAreaListOverlay {
    AllocationAreaNodeOverlay* head;
    std::uint32_t headerByte;
};

static_assert(offsetof(AllocationAreaListOverlay, head) == 0x00);
static_assert(offsetof(AllocationAreaListOverlay, headerByte) == 0x04);

// Raw direct-call target observed at 0x0042910C via `push esi; call ...`.
// The binary does not support a stronger virtual/member-call assumption here.
extern AllocationAreaNodeOverlay* __cdecl GetNoAllocatedAreas(AllocationAreaNodeOverlay* current);

void __fastcall SerializeAllocationAreas(CDataOutputStream* stream, AllocationAreaListOverlay* areas)
{
    CDataOutputStream::WriteCBYTE(stream, static_cast<char>(areas->headerByte));

    AllocationAreaNodeOverlay* node = areas->head->next;
    if (node != areas->head) {
        do {
            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(node->value10));
            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(node->value14));
            node = GetNoAllocatedAreas(node);
        } while (node != areas->head);
    }
}