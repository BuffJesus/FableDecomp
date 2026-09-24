#include "fable_ui_observer_lifetime.h"
FableUiObserverInterfaceView* __fastcall FableUiGetExclusiveObserver(FableUiManagerObserverView* observable, void*)
{
    return observable->ExclusiveObserver;
}
