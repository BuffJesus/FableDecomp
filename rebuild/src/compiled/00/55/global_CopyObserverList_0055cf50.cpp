#include "fable_ui_observer_lifetime.h"
FableUiObserverListNode** __fastcall FableUiCopyObserverList(FableUiObserverListNode** destination, void*, FableUiObserverListNode* const* source)
{
    FableUiConstructObserverList(destination,0,0);
    FableUiInsertObserverRange(destination,0,(*destination)->Next,(*source)->Next,*source,0);
    return destination;
}
