#include "fable_ui_observer_lifetime.h"
void __fastcall FableUiInsertObserverRange(FableUiObserverListNode**, void*, FableUiObserverListNode* before,
    FableUiObserverListNode* first, FableUiObserverListNode* end, const void*)
{
    while(first!=end)
    {
        FableUiObserverListNode* node=static_cast<FableUiObserverListNode*>(FableUiAllocateObserverNode(12));
        node->Observer=first->Observer;
        node->Next=before; node->Previous=before->Previous;
        before->Previous->Next=node; before->Previous=node;
        first=first->Next;
    }
}
