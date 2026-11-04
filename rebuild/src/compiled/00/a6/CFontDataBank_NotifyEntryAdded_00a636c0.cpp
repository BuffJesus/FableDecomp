// lib_font_data_bank.cpp, retail 0x00A636C0 (Ego_r
// ?NotifyEntryAdded@CFontDataBank@@MAEXKKABV?$CArray@D@@@Z): grow the font
// table to cover the index, give type-0 entries a font entry, then let the
// base bank record the entry.
#include <new>
#include "fable_font_data_banks.h"
#include "engine/CFontDataBankEntry.h"
#include "fable_stlport_vector_impl.h"

void CFontDataBank::NotifyEntryAdded(unsigned long type, unsigned long index, const CArray<char>& data)
{
    while (index >= Fonts.size())
        Fonts.push_back(0);
    CFontDataBankEntry* entry = 0;
    if (type == 0)
        entry = new CFontDataBankEntry(type, index, this);
    Fonts[index] = entry;
    CBankFile::NotifyEntryAdded(type, index, data);
}
