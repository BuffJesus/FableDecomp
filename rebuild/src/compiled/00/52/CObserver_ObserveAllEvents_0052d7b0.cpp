#include "fable_ui_observer_events.h"
void __fastcall FableUiObserveAllEvents(FableUiObserverEventsView* observer,void*)
{
    for(int event=0;event<=33;++event)
        static_cast<FableUiObserveEventVtable*>(observer->Vtable)->ObserveEvent(observer,0,event);
    // Retail's ObserveAllEvents explicitly omits event IDs 34 and 35.
    static_cast<FableUiObserveEventVtable*>(observer->Vtable)->ObserveEvent(observer,0,36);
    static_cast<FableUiObserveEventVtable*>(observer->Vtable)->ObserveEvent(observer,0,37);
}
