// lib_streaming_font_data_bank.cpp, retail 0x00A5F430 (Ego_r
// ?NotifyEntryAdded@CStreamingFontDataBank@@MAEXKKABV?$CArray@D@@@Z): grow the
// font table to cover the index, give type 0/1/2 entries a streaming font
// entry, then let the base bank record the entry.
#include <new>
#include "fable_font_data_banks.h"
#include "engine/CStreamingFontDataBankEntry.h"
#include "fable_stlport_vector_impl.h"

void CStreamingFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
{
    while (index >= Fonts.size())
        Fonts.push_back(0);
    CStreamingFontDataBankEntry* entry = 0;
    if (type == 0 || type == 1 || type == 2)
        entry = new CStreamingFontDataBankEntry(type, index, this);
    Fonts[index] = entry;
    CBankFile::NotifyEntryAdded(type, index, data);
}
