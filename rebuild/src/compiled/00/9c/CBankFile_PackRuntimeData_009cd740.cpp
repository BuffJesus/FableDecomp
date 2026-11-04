// Retail009CD740: pack entries1..n-1 of the runtime table into four packed arrays and
// keep whichever representation is smaller (the vector reserve counts one extra slot).
#include "fable_game_text_bank.h"
void __fastcall ClearPackedBankArray(CPackedUIntArray*);
void CBankFile::PackRuntimeData()
{
    unsigned long count = RuntimeData.size();
    if (count > 1) {
        CRuntimeData* data = &RuntimeData[1]; --count;
        PackedDataOffset.Initialise(&data->DataOffset, 32, sizeof(CRuntimeData), count);
        PackedDataSize.Initialise(&data->DataSize, 32, sizeof(CRuntimeData), count);
        PackedDataType.Initialise(&data->DataType, 8, sizeof(CRuntimeData), count);
        PackedValid.Initialise(&data->Valid, 8, sizeof(CRuntimeData), count);
        long unpacked = (RuntimeData.capacity() + 1) * sizeof(CRuntimeData);
        long packed = PackedDataOffset.Sizeof() + PackedDataSize.Sizeof() + PackedDataType.Sizeof() + PackedValid.Sizeof();
        if (packed < unpacked) RuntimeData.DeallocateMemory();
        else {
            ClearPackedBankArray(&PackedDataOffset); ClearPackedBankArray(&PackedDataSize);
            ClearPackedBankArray(&PackedDataType); ClearPackedBankArray(&PackedValid);
        }
    }
}
