#include "fable_ui_observer_lifetime.h"
FableUiManagerObserverView* __fastcall FableUiConstructObservable(FableUiManagerObserverView* observable, void*)
{
    observable->Vtable=&FableUiObservableVtable;
    FableUiConstructObserverList(&observable->Observers,0,0);
    observable->ExclusiveObserver=0;
    FableUiConstructObserverList(&observable->ConcurrentExclusiveObservers,0,0);
    return observable;
}
