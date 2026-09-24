#include "fable_ui_observer.h"
void __fastcall FableUiSetExclusiveObserver(FableUiManagerObserverView* observable, void*, FableUiObserverInterfaceView* observer)
{
    observable->ExclusiveObserver=observer;
}
