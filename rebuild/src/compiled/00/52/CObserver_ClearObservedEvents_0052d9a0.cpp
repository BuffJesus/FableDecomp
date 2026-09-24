#include "fable_ui_observer_events.h"
void __fastcall FableUiClearObservedEvents(FableUiObserverEventsView* observer,void*)
{
    FableUiEventSet& set=observer->EventsToObserve;
    if(!set.Count) return;
    FableUiFreeEventTree(&set,0,set.Head->Parent);
    set.Head->Left=set.Head; set.Head->Parent=0; set.Head->Right=set.Head;
    set.Count=0;
}
