#include "fable_ui_state_progress.h"

bool __fastcall FableUiBasePositionIndependent(FableUiComponentDrawView* component, void*)
{
    return component->Vtable->IsIndependent(component, 0);
}
