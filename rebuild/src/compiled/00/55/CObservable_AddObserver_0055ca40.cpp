#include "fable_ui_observer.h"
void __fastcall FableUiAddObserver(FableUiManagerObserverView* observable, void*, FableUiObserverInterfaceView* observer)
{
    FableUiAppendUniqueObserver(observable->Observers, observer);
}
