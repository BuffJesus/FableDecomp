#include "fable_ui_observer.h"

void __fastcall FableUiRemoveObserver(FableUiManagerObserverView* observable, void*, FableUiObserverInterfaceView* observer)
{
    FableUiEraseObserver(observable->Observers, observer);
}
