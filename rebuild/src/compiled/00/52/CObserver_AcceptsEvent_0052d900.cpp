#include "fable_ui_observer_events.h"
bool __fastcall FableUiAcceptsEvent(FableUiObserverEventsView* observer,void*,int event)
{
    if(observer->PreventObservation) return false;
    FableUiEventTreeNode* result;
    FableUiFindEventInRange(&result,&event,observer->EventsToObserve.Head->Left,observer->EventsToObserve.Head,0);
    return result!=observer->EventsToObserve.Head;
}
