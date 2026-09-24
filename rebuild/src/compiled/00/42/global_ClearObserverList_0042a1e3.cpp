#include "fable_ui_observer_lifetime.h"
void __fastcall FableUiClearObserverList(FableUiObserverListNode** list, void*)
{
    FableUiObserverListNode* entry=(*list)->Next;
    while(entry!=*list)
    {
        FableUiObserverListNode* removed=entry;
        entry=entry->Next;
        FableUiFreeObserverNode(removed);
    }
    (*list)->Next=*list;
    (*list)->Previous=*list;
}
