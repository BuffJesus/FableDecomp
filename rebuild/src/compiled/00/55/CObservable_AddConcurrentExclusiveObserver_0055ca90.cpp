#include "fable_ui_observer.h"
void __fastcall FableUiAddConcurrentExclusiveObserver(FableUiManagerObserverView* observable, void*, FableUiObserverInterfaceView* observer)
{
    FableUiAppendUniqueObserver(observable->ConcurrentExclusiveObservers, observer);
}
