#include "fable_game_text_bank.h"
unsigned long CBankFile::GetEntryDataType(unsigned long index) const
{
    if (PackedDataType.Size) return PackedDataType[index - 1];
    return RuntimeData[index].DataType;
}
unsigned long CBankFile::GetEntryDataSize(unsigned long index) const
{
    if (PackedDataSize.Size) return PackedDataSize[index - 1];
    return RuntimeData[index].DataSize;
}
