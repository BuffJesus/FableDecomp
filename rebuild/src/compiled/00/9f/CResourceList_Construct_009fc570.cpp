#include "fable_ui_texture_manager.h"
#include <string.h>
CResourceList* __fastcall FableUiConstructResourceList(CResourceList* list,void*)
{
    list->__vftable=FableUiResourceListVtable;
    CResource* head=reinterpret_cast<CResource*>(list->Head);
    head->__vftable=FableUiResourceVtable;
    unsigned one=1; memcpy(head->_pad_0x04,&one,4);
    head->ResourceList=0; head->PrevResource=head; head->NextResource=head;
    head->ResourceSize=0; head->LastUsedFrame=0;
    list->ResourceCount=0; list->AllocatedMemory=0; list->MaximumMemory=0x7FFFFFFF;
    list->CurrentFrame=0; list->DebugStatsFrame=0; list->UnloadDelay=1; list->UnloadedThisFrame=0;
    return list;
}
