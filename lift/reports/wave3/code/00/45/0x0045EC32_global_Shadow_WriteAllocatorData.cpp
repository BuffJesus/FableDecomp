#include <cstddef>
#include <cstdint>

struct CDataOutputStream;

struct CMemoryAllocatorVariableSize {
    std::byte pad_00[0x08];
    CMemoryAllocatorVariableSize* field_08;
    std::byte pad_0C[0x04];
    std::uint32_t field_10;
    bool field_14;
};

static_assert(offsetof(CMemoryAllocatorVariableSize, field_08) == 0x08);
static_assert(offsetof(CMemoryAllocatorVariableSize, field_10) == 0x10);
static_assert(offsetof(CMemoryAllocatorVariableSize, field_14) == 0x14);

struct ShadowAllocatorDataOverlay {
    CMemoryAllocatorVariableSize* field_00;
    std::uint32_t field_04;
};

static_assert(offsetof(ShadowAllocatorDataOverlay, field_00) == 0x00);
static_assert(offsetof(ShadowAllocatorDataOverlay, field_04) == 0x04);

// Raw call shape observed in assembly: argument is pushed, not passed as `this`.
extern CMemoryAllocatorVariableSize* __cdecl GetNoAllocatedAreas(CMemoryAllocatorVariableSize* allocator);

void __fastcall Shadow_WriteAllocatorData(
    CDataOutputStream* stream,
    ShadowAllocatorDataOverlay* allocatorData)
{
    CDataOutputStream::WriteCBYTE(stream, static_cast<char>(allocatorData->field_04));

    CMemoryAllocatorVariableSize* current = allocatorData->field_00->field_08;
    if (current != allocatorData->field_00) {
        do {
            CDataOutputStream::WriteCBYTE(stream, static_cast<char>(current->field_10));
            CDataOutputStream::WriteEBOOL(stream, current->field_14);
            current = GetNoAllocatedAreas(current);
        } while (current != allocatorData->field_00);
    }
}