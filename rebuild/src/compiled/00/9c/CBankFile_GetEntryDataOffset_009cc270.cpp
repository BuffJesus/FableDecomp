#include "fable_game_text_bank.h"

// Retail 009CC270: packed offset table when present, else runtime entry record.
unsigned long CBankFile::GetEntryDataOffset(unsigned long index) const
{
    if (PackedDataOffset.Size) return PackedDataOffset[index - 1];
    return RuntimeData[index].DataOffset;
}
