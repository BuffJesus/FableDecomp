// Type-only provider for compositions where the EntryExists/CountEntries unit
// already emits GetEntryDataSize. The existing two-accessor unit remains intact.
#include "fable_game_text_bank.h"

unsigned long CBankFile::GetEntryDataType(unsigned long index) const
{
    if (PackedDataType.Size) return PackedDataType[index - 1];
    return RuntimeData[index].DataType;
}
