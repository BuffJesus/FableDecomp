#include "fable_ui_bank_runtime.h"
#include <string.h>
static void ConstructList(CResourceList* list)
{
    list->__vftable=FableUiResourceListVtable;
    CResource* head=reinterpret_cast<CResource*>(list->Head);
    head->__vftable=FableUiResourceVtable;
    unsigned one=1; memcpy(head->_pad_0x04,&one,4); // Intrusive count, absent as a named PDB member.
    head->ResourceList=0; head->PrevResource=head; head->NextResource=head;
    head->ResourceSize=0; head->LastUsedFrame=0;
    list->ResourceCount=0; list->AllocatedMemory=0; list->MaximumMemory=0x7FFFFFFF;
    list->CurrentFrame=0; list->DebugStatsFrame=0; list->UnloadDelay=1; list->UnloadedThisFrame=0;
}
FableUiResourceBankView* __fastcall FableUiConstructResourceBank(FableUiResourceBankView* bank,void*)
{
    FableUiConstructBase(bank,0); bank->Vtable=FableUiResourceBankVtable;
    ConstructList(&bank->ResourceList); ConstructList(&bank->LoadingQueue); return bank;
}
