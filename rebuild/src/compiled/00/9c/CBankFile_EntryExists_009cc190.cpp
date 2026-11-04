#define FABLE_BANK_FILE_ENTRY_DATA_SIZE_INLINE
#include "fable_font_data_banks.h"
bool CBankFile::EntryExists(unsigned long index) const
{
    if (!index || index >= Size) return false;
    if (PackedValid.Size) return PackedValid[index-1] != 0;
    return RuntimeData[index].Valid;
}

// Retail 009CC3B0, same TU: EntryExists is auto-inlined into the loop.
unsigned long CBankFile::CountEntries() const
{
    unsigned long count = 0;
    for (unsigned long index = 0; index < Size; ++index)
        if (EntryExists(index)) ++count;
    return count;
}

// Retail 009CC450, same TU: EntryExists and GetEntryDataSize (inline switch)
// are both expanded into the loop.
unsigned long CBankFile::GetTotalDataSize() const
{
    unsigned long total = 0;
    for (unsigned long index = 0; index < Size; ++index)
        if (EntryExists(index)) total += GetEntryDataSize(index);
    return total;
}
