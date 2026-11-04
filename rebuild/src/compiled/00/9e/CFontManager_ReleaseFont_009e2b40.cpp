#include "fable_system_manager.h"
#include "fable_font_data_banks.h"
#include "fable_symbol_map.h"

void CFontManager::ReleaseFont(const CCharString& name)
{
    CFontDataBankEntry* entry;
    if (PSymbols->SymbolExists(name))
    {
        unsigned long index = PSymbols->GetSymbolSLONG(name);
        entry = PFontDataBank->GetFontInfo(index);
    }
    else if (PStreamingSymbols->SymbolExists(name))
    {
        unsigned long index = PStreamingSymbols->GetSymbolSLONG(name);
        entry = reinterpret_cast<CFontDataBankEntry*>(
            PStreamingFontDataBank->GetFontInfo(index));
    }
    else
        return;
    if (entry->CanUnload())
        entry->Unload();
}