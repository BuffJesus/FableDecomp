#include "fable_ui_observer_lifetime.h"
FableUiObserverListNode** __fastcall FableUiConstructObserverList(FableUiObserverListNode** list, void*, const void*)
{
    *list=0;
    FableUiObserverListNode* head=static_cast<FableUiObserverListNode*>(FableUiAllocateObserverNode(12));
    head->Next=head; head->Previous=head;
    *list=head;
    return list;
}
