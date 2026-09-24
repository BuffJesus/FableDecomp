#include "fable_ui_observer_events.h"
FableUiObserverEventsView* __fastcall FableUiConstructObserver(FableUiObserverEventsView* observer,void*)
{
    observer->Vtable=&FableUiObserverVtable;
    observer->EventsToObserve.Head=0;
    observer->EventsToObserve.Head=static_cast<FableUiEventTreeNode*>(FableUiAllocateEventNode(20));
    observer->EventsToObserve.Count=0;
    FableUiEventTreeNode* head=observer->EventsToObserve.Head;
    head->Colour=0; head->Parent=0; head->Left=head; head->Right=head;
    observer->PreventObservation=0;
    return observer;
}
