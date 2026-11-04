#include "fable_font_data_banks.h"
#include "fable_bank_entry_update.h"
bool CBankFile::EntrySetUpdate(unsigned long index, bool update)
{
    if(index < RuntimeData.size() && RuntimeData[index].Valid) {
        bool previous=UpdateData[index]->RequiresUpdate;
        UpdateData[index]->RequiresUpdate=update;
        return previous;
    }
    return false;
}
