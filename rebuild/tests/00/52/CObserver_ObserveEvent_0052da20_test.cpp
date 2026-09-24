#include "fable_ui_observer_events.h"
#include <stdio.h>
static FableUiObserverEventsView observer;
static FableUiEventTreeNode head,node;
static unsigned stage,calls;
static bool present;
FableUiEventTreeNode** __fastcall FableUiFindObservedEvent(FableUiEventSet* set,void*,FableUiEventTreeNode** result,const int*)
{ if(set!=&observer.EventsToObserve || stage) return 0; stage=1; *result=present ? &node : &head; return result; }
FableUiEventInsertResult* __fastcall FableUiInsertObservedEvent(FableUiEventSet* set,void*,FableUiEventInsertResult* result,const int*)
{ if(set!=&observer.EventsToObserve || stage!=1) return 0; stage=2; result->Node=&node; result->Inserted=!present; return result; }
static void __fastcall Process(FableUiObserverInterfaceView* p,void*,unsigned event)
{ if(p==reinterpret_cast<FableUiObserverInterfaceView*>(&observer) && event==25 && stage==2) ++calls; }
int main()
{
 FableUiObserverEventVtable table={}; table.ProcessEvent=Process; observer.Vtable=&table; observer.EventsToObserve.Head=&head;
 for(unsigned i=0;i<3;++i) { stage=0; present=i==1; FableUiObserveEvent(&observer,0,i==2 ? 26 : 25); if(stage!=2 || calls!=1) return 1; }
 puts("OBSERVER_0052da20_TEST PASS"); return 0;
}
