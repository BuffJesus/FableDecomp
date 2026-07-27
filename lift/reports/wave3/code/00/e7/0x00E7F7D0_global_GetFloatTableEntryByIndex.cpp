#include <cstddef>

struct GetFloatTableEntryByIndex_ObjectOverlay {
    std::byte pad_00[100];
    int tableIndex;
};
static_assert(offsetof(GetFloatTableEntryByIndex_ObjectOverlay, tableIndex) == 100);

struct GetFloatTableEntryByIndex_GlobalOverlay {
    std::byte pad_0000[0x5D0];
    float* floatTable;
};
static_assert(offsetof(GetFloatTableEntryByIndex_GlobalOverlay, floatTable) == 0x5D0);

extern GetFloatTableEntryByIndex_GlobalOverlay DAT_0143e90c;

float __fastcall GetFloatTableEntryByIndex(void* thisPtr)
{
    auto* const self = static_cast<GetFloatTableEntryByIndex_ObjectOverlay*>(thisPtr);

    int index = self->tableIndex + 1;
    if (self->tableIndex == -1) {
        index = 1;
    }

    return DAT_0143e90c.floatTable[index];
}