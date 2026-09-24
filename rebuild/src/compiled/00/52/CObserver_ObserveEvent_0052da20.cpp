#include "fable_ui_observer_events.h"
void __fastcall FableUiObserveEvent(FableUiObserverEventsView* observer,void*,int event)
{
    FableUiEventTreeNode* previous;
    FableUiFindObservedEvent(&observer->EventsToObserve,0,&previous,&event);
    FableUiEventTreeNode* head=observer->EventsToObserve.Head;
    FableUiEventInsertResult inserted;
    FableUiInsertObservedEvent(&observer->EventsToObserve,0,&inserted,&event);
    if(event==25 && previous==head)
    {
        FableUiObserverEventVtable* table=static_cast<FableUiObserverEventVtable*>(observer->Vtable);
        table->ProcessEvent(reinterpret_cast<FableUiObserverInterfaceView*>(observer),0,25);
    }
}
