#include "fable_ui_observer.h"

void __fastcall FableUiDie(FableUiComponentDrawView* component, void*)
{
    component->Vtable->RequestState(component, 0, 2);
    FableUiRemoveObserverRecursive(component, 0);
}
