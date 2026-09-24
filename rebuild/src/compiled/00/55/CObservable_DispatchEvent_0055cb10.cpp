#include "fable_ui_observer_lifetime.h"

void __fastcall FableUiDispatchEvent(FableUiManagerObserverView* observable, void*, unsigned event)
{
    if(observable->ExclusiveObserver)
    {
        FableUiObserverInterfaceView* observer=observable->ExclusiveObserver;
        if(static_cast<FableUiObserverEventVtable*>(observer->Vtable)->AcceptsEvent(observer,0,event))
        {
            // The query can replace the exclusive observer.
            observer=observable->ExclusiveObserver;
            static_cast<FableUiObserverEventVtable*>(observer->Vtable)->ProcessEvent(observer,0,event);
        }
        return;
    }
    FableUiObserverListNode** source=
        observable->ConcurrentExclusiveObservers->Next!=observable->ConcurrentExclusiveObservers ?
        &observable->ConcurrentExclusiveObservers : &observable->Observers;
    FableUiObserverListNode* snapshot;
    FableUiCopyObserverList(&snapshot,0,source);
    for(FableUiObserverListNode* entry=snapshot->Next; entry!=snapshot; entry=entry->Next)
    {
        FableUiObserverInterfaceView* observer=entry->Observer;
        if(static_cast<FableUiObserverEventVtable*>(observer->Vtable)->AcceptsEvent(observer,0,event))
        {
            observer=entry->Observer;
            static_cast<FableUiObserverEventVtable*>(observer->Vtable)->ProcessEvent(observer,0,event);
        }
    }
    FableUiDestroyObserverList(&snapshot,0);
}
