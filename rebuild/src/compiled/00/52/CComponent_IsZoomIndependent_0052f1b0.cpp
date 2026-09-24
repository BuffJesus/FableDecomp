#include "fable_ui_state_progress.h"

bool __fastcall FableUiBaseZoomIndependent(FableUiComponentDrawView* component, void*)
{
    return component->Vtable->IsIndependent(component, 0);
}
