#include "fable_ui_observer.h"

void __fastcall FableUiRemoveObserverRecursive(FableUiComponentDrawView* component, void*)
{
    FableUiObserverInterfaceView* observer = component ? &component->Observer : 0;
    FableUiManagerObserverView* manager = static_cast<FableUiManagerObserverView*>(FableUiGetManager());
    manager->Vtable->RemoveObserver(manager, 0, observer);
    component->Vtable->ObserverRemoved(component, 0);
    for (unsigned i = 0; i < component->Children.Size(); ++i)
        FableUiRemoveObserverRecursive(component->Children.Begin[i].Data, 0);
}
