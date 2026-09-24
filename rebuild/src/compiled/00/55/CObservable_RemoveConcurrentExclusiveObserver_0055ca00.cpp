#include "fable_ui_observer.h"
void __fastcall FableUiRemoveConcurrentExclusiveObserver(FableUiManagerObserverView* observable, void*, FableUiObserverInterfaceView* observer)
{
    FableUiEraseObserver(observable->ConcurrentExclusiveObservers, observer);
}
