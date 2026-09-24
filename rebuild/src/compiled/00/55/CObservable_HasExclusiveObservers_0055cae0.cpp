#include "fable_ui_observer.h"
bool __fastcall FableUiHasExclusiveObservers(FableUiManagerObserverView* observable, void*)
{
    return observable->ExclusiveObserver ||
        observable->ConcurrentExclusiveObservers->Next!=observable->ConcurrentExclusiveObservers;
}
