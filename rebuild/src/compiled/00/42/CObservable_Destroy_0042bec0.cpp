#include "fable_ui_observer_lifetime.h"
void __fastcall FableUiDestroyObservable(FableUiManagerObserverView* observable, void*)
{
    FableUiDestroyObserverList(&observable->ConcurrentExclusiveObservers,0);
    FableUiDestroyObserverList(&observable->Observers,0);
}
