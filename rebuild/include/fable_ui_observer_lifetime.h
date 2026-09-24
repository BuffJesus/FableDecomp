#pragma once
#include "fable_ui_observer.h"

// Retail CObservable vtable 01230044, supplied by engine integration.
extern FableUiManagerObserverVtable FableUiObservableVtable;
FableUiObserverListNode** __fastcall FableUiConstructObserverList(FableUiObserverListNode**, void*, const void*);
void __fastcall FableUiClearObserverList(FableUiObserverListNode**, void*);
void __fastcall FableUiDestroyObserverList(FableUiObserverListNode**, void*);
FableUiManagerObserverView* __fastcall FableUiConstructObservable(FableUiManagerObserverView*, void*);
void __fastcall FableUiDestroyObservable(FableUiManagerObserverView*, void*);
FableUiObserverInterfaceView* __fastcall FableUiGetExclusiveObserver(FableUiManagerObserverView*, void*);
FableUiObserverListNode** __fastcall FableUiCopyObserverList(FableUiObserverListNode**, void*, FableUiObserverListNode* const*);
void __fastcall FableUiInsertObserverRange(FableUiObserverListNode**, void*, FableUiObserverListNode*, FableUiObserverListNode*, FableUiObserverListNode*, const void*);
void __fastcall FableUiDispatchEvent(FableUiManagerObserverView*, void*, unsigned);

struct FableUiObserverEventVtable
{
    void* Unrecovered00;
    void (__fastcall *ProcessEvent)(FableUiObserverInterfaceView*, void*, unsigned);
    bool (__fastcall *AcceptsEvent)(FableUiObserverInterfaceView*, void*, unsigned);
};
FABLE_STATIC_ASSERT(offsetof(FableUiObserverEventVtable, ProcessEvent)==4);
FABLE_STATIC_ASSERT(offsetof(FableUiObserverEventVtable, AcceptsEvent)==8);
