#include <cstddef>
#include <cstdint>

class CDataOutputStream {
public:
    static void WriteCBYTE(CDataOutputStream* stream, char value);
    static void WriteEBOOL(CDataOutputStream* stream, bool value);
};

struct MemoryAllocatorAreaNodeOverlay {
    std::byte pad_00[0x08];
    MemoryAllocatorAreaNodeOverlay* next;
    std::byte pad_0C[0x04];
    std::uint32_t areaByte;
    bool isAllocated;
};

static_assert(offsetof(MemoryAllocatorAreaNodeOverlay, next) == 0x08);
static_assert(offsetof(MemoryAllocatorAreaNodeOverlay, areaByte) == 0x10);
static_assert(offsetof(MemoryAllocatorAreaNodeOverlay, isAllocated) == 0x14);

struct SaveMemoryAllocatorAreasStateOverlay {
    MemoryAllocatorAreaNodeOverlay* sentinel;
    std::uint32_t areaCount;
};

static_assert(offsetof(SaveMemoryAllocatorAreasStateOverlay, sentinel) == 0x00);
static_assert(offsetof(SaveMemoryAllocatorAreasStateOverlay, areaCount) == 0x04);

// Observed call shape is a raw push/call using the current node as the single argument.
extern MemoryAllocatorAreaNodeOverlay* GetNoAllocatedAreas(MemoryAllocatorAreaNodeOverlay* node);

void __fastcall Save_WriteMemoryAllocatorAreas(
    CDataOutputStream* stream,
    SaveMemoryAllocatorAreasStateOverlay* state)
{
    CDataOutputStream::WriteCBYTE(stream, static_cast<char>(state->areaCount));

    MemoryAllocatorAreaNodeOverlay* area = state->sentinel->next;
    if (area != state->sentinel) {
        do {
            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(area->areaByte));
            CDataOutputStream::WriteEBOOL(stream, area->isAllocated);
            area = GetNoAllocatedAreas(area);
        } while (area != state->sentinel);
    }
}