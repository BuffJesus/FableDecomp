#include "fable_ui_observer_lifetime.h"
void __fastcall FableUiDestroyObserverList(FableUiObserverListNode** list, void*)
{
    FableUiClearObserverList(list,0);
    if(*list) FableUiFreeObserverNode(*list);
}
