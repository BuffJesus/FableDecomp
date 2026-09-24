#pragma once
#include "fable_ui_transform.h"

struct FableUiManagerObserverView;
typedef void (__fastcall *FableUiRemoveObserverCall)(FableUiManagerObserverView*, void*, FableUiObserverInterfaceView*);
struct FableUiManagerObserverVtable
{
    void* Unrecovered00[5];
    FableUiRemoveObserverCall RemoveObserver;
};
struct FableUiObserverListNode
{
    FableUiObserverListNode* Next;
    FableUiObserverListNode* Previous;
    FableUiObserverInterfaceView* Observer;
};
struct FableUiManagerObserverView
{
    FableUiManagerObserverVtable* Vtable;
    FableUiObserverListNode* Observers;
    FableUiObserverInterfaceView* ExclusiveObserver;
    FableUiObserverListNode* ConcurrentExclusiveObservers;
};
struct FableUiObserverNotificationVtable
{
    void* Unrecovered00[5];
    void (__fastcall *Removed)(FableUiObserverInterfaceView*, void*);
};
FABLE_STATIC_ASSERT(offsetof(FableUiManagerObserverView, Observers) == 4);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerObserverView, ExclusiveObserver) == 8);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerObserverView, ConcurrentExclusiveObservers) == 12);
FABLE_STATIC_ASSERT(sizeof(FableUiObserverListNode) == 12);
FABLE_STATIC_ASSERT(offsetof(FableUiObserverNotificationVtable, Removed) == 0x14);
// Retail free 00BFEA14; only the removed list node is released here.
void FableUiFreeObserverNode(void*);
void* FableUiAllocateObserverNode(unsigned);
void __fastcall FableUiRemoveObserver(FableUiManagerObserverView*, void*, FableUiObserverInterfaceView*);
void __fastcall FableUiRemoveConcurrentExclusiveObserver(FableUiManagerObserverView*, void*, FableUiObserverInterfaceView*);
void __fastcall FableUiAddObserver(FableUiManagerObserverView*, void*, FableUiObserverInterfaceView*);
void __fastcall FableUiAddConcurrentExclusiveObserver(FableUiManagerObserverView*, void*, FableUiObserverInterfaceView*);
void __fastcall FableUiSetExclusiveObserver(FableUiManagerObserverView*, void*, FableUiObserverInterfaceView*);
void __fastcall FableUiClearExclusiveObserver(FableUiManagerObserverView*, void*, FableUiObserverInterfaceView*);
bool __fastcall FableUiHasExclusiveObservers(FableUiManagerObserverView*, void*);
void __fastcall FableUiClearObservers(FableUiManagerObserverView*, void*);

inline void FableUiAppendUniqueObserver(FableUiObserverListNode* head, FableUiObserverInterfaceView* observer)
{
    for (FableUiObserverListNode* n=head->Next; n!=head; n=n->Next)
        if (n->Observer==observer) return;
    FableUiObserverListNode* entry=static_cast<FableUiObserverListNode*>(FableUiAllocateObserverNode(12));
    entry->Observer=observer;
    entry->Next=head; entry->Previous=head->Previous;
    head->Previous->Next=entry; head->Previous=entry;
}

inline void FableUiEraseObserver(FableUiObserverListNode* head, FableUiObserverInterfaceView* observer)
{
    FableUiObserverListNode* entry=head->Next;
    while (entry!=head && entry->Observer!=observer) entry=entry->Next;
    if (entry==head) return;
    FableUiObserverInterfaceView* found=entry->Observer;
    static_cast<FableUiObserverNotificationVtable*>(found->Vtable)->Removed(found,0);
    // Notification can modify neighboring links before unlinking this entry.
    entry->Previous->Next=entry->Next;
    entry->Next->Previous=entry->Previous;
    FableUiFreeObserverNode(entry);
}
FABLE_STATIC_ASSERT(offsetof(FableUiManagerObserverVtable, RemoveObserver) == 0x14);
void __fastcall FableUiRemoveObserverRecursive(FableUiComponentDrawView*, void*);
void __fastcall FableUiDie(FableUiComponentDrawView*, void*);
