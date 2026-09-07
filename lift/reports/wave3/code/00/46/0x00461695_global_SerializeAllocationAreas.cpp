#include <cstddef>
#include <cstdint>

class CDataOutputStream {
public:
    void WriteCBYTE(char value);
};

struct AllocationAreaNodeOverlay {
    std::byte pad00[0x08];
    AllocationAreaNodeOverlay* next;
    std::byte pad0C[0x04];
    std::uint32_t value10;
    float value14;
};

static_assert(offsetof(AllocationAreaNodeOverlay, next) == 0x08);
static_assert(offsetof(AllocationAreaNodeOverlay, value10) == 0x10);
static_assert(offsetof(AllocationAreaNodeOverlay, value14) == 0x14);

struct AllocationAreaChainOverlay {
    AllocationAreaNodeOverlay* head;
    std::uint32_t value04;
};

static_assert(offsetof(AllocationAreaChainOverlay, head) == 0x00);
static_assert(offsetof(AllocationAreaChainOverlay, value04) == 0x04);

void __fastcall SerializeAllocationAreas(
    CDataOutputStream* stream,
    AllocationAreaChainOverlay* chain
) {
    using WriteFloatFn = void(__thiscall*)(CDataOutputStream*, float);
    using GetNextAreaFn = AllocationAreaNodeOverlay*(__cdecl*)(AllocationAreaNodeOverlay*);

    constexpr auto kWriteFloat = reinterpret_cast<WriteFloatFn>(0x00994EDF);
    constexpr auto kGetNextArea = reinterpret_cast<GetNextAreaFn>(0x004292BF);

    stream->WriteCBYTE(static_cast<char>(chain->value04));

    AllocationAreaNodeOverlay* area = chain->head->next;
    if (area != chain->head) {
        do {
            stream->WriteCBYTE(static_cast<char>(area->value10));
            kWriteFloat(stream, area->value14);
            area = kGetNextArea(area);
        } while (area != chain->head);
    }
}