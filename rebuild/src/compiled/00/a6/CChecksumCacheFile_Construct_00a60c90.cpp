#include "fable_ui_bank_file.h"
FableUiChecksumCacheView* __fastcall FableUiConstructChecksumCache(FableUiChecksumCacheView* cache,void*,unsigned size)
{
    new (&cache->Filename) CWideString;
    FableUiConstructBankTree(&cache->Entries,52);
    cache->NeedsSaving=false;
    // The retail expression clears bit 3 only, not all low alignment bits.
    cache->BufferSize=(size+8)&~8u;
    return cache;
}
