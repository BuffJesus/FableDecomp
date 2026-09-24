#include "fable_ui_observer.h"
void __fastcall FableUiClearExclusiveObserver(FableUiManagerObserverView* observable, void*, FableUiObserverInterfaceView*)
{
    // Retail clears unconditionally; the supplied observer is not compared.
    observable->ExclusiveObserver=0;
}
