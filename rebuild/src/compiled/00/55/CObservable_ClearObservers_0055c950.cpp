#include "fable_ui_observer.h"

static void ClearList(FableUiObserverListNode*& head)
{
    FableUiObserverListNode* n=head->Next;
    while(n!=head)
    {
        FableUiObserverListNode* removed=n;
        n=n->Next;
        FableUiFreeObserverNode(removed);
    }
    head->Next=head;
    head->Previous=head;
}
void __fastcall FableUiClearObservers(FableUiManagerObserverView* observable, void*)
{
    // Unlike individual removal, bulk clearing does not notify observers.
    ClearList(observable->Observers);
    observable->ExclusiveObserver=0;
    ClearList(observable->ConcurrentExclusiveObservers);
}
