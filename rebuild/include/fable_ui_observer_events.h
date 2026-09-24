#pragma once
#include "fable_ui_observer_lifetime.h"

struct FableUiEventTreeNode
{
    unsigned char Colour;
    unsigned char Unrecovered01[3];
    FableUiEventTreeNode* Parent;
    FableUiEventTreeNode* Left;
    FableUiEventTreeNode* Right;
    int Event;
};
struct FableUiEventSet
{
    FableUiEventTreeNode* Head;
    unsigned Count;
    unsigned char Unrecovered08[4];
};
struct FableUiObserverEventsView
{
    void* Vtable;
    FableUiEventSet EventsToObserve;
    unsigned char PreventObservation;
    unsigned char Unrecovered11[3];
};
struct FableUiObserveEventVtable
{
    void* Unrecovered00[3];
    void (__fastcall *ObserveEvent)(FableUiObserverEventsView*, void*, int);
};
FABLE_STATIC_ASSERT(sizeof(FableUiEventTreeNode)==20);
FABLE_STATIC_ASSERT(offsetof(FableUiEventTreeNode, Event)==16);
FABLE_STATIC_ASSERT(offsetof(FableUiObserverEventsView, EventsToObserve)==4);
FABLE_STATIC_ASSERT(offsetof(FableUiObserverEventsView, PreventObservation)==16);
extern FableUiObserveEventVtable FableUiObserverVtable;
void* FableUiAllocateEventNode(unsigned);
void FableUiFreeEventNode(void*);
FableUiObserverEventsView* __fastcall FableUiConstructObserver(FableUiObserverEventsView*,void*);
bool __fastcall FableUiAcceptsEvent(FableUiObserverEventsView*,void*,int);
void __fastcall FableUiObserveAllEvents(FableUiObserverEventsView*,void*);
void __fastcall FableUiClearObservedEvents(FableUiObserverEventsView*,void*);
void __fastcall FableUiFreeEventTree(FableUiEventSet*,void*,FableUiEventTreeNode*);
FableUiEventTreeNode** __fastcall FableUiFindEventInRange(FableUiEventTreeNode**,const int*,FableUiEventTreeNode*,FableUiEventTreeNode*,const void*);
FableUiEventTreeNode** __fastcall FableUiFindObservedEvent(FableUiEventSet*,void*,FableUiEventTreeNode**,const int*);
struct FableUiEventInsertResult { FableUiEventTreeNode* Node; bool Inserted; };
void __cdecl FableUiRotateEventTreeLeft(FableUiEventTreeNode*,FableUiEventTreeNode**);
void __cdecl FableUiRotateEventTreeRight(FableUiEventTreeNode*,FableUiEventTreeNode**);
void __cdecl FableUiBalanceEventTree(FableUiEventTreeNode*,FableUiEventTreeNode**);
FableUiEventTreeNode** __fastcall FableUiLinkEventNode(FableUiEventSet*,void*,FableUiEventTreeNode**,FableUiEventTreeNode*,FableUiEventTreeNode*,const int*,bool);
FableUiEventInsertResult* __fastcall FableUiInsertObservedEvent(FableUiEventSet*,void*,FableUiEventInsertResult*,const int*);
void __fastcall FableUiObserveEvent(FableUiObserverEventsView*,void*,int);
FableUiEventTreeNode* __cdecl FableUiEraseEventTreeNode(FableUiEventTreeNode*,FableUiEventTreeNode**,FableUiEventTreeNode**,FableUiEventTreeNode**);
void __fastcall FableUiRemoveObservedEvent(FableUiObserverEventsView*,void*,int);
