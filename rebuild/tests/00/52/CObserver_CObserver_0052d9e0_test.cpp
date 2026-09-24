#include "fable_ui_observer_events.h"
#include <stdio.h>
#include <string.h>
FableUiObserveEventVtable FableUiObserverVtable={};
static FableUiObserverEventsView observer;
static FableUiEventTreeNode node;
static bool valid;
void* FableUiAllocateEventNode(unsigned size)
{ valid=size==20 && observer.Vtable==&FableUiObserverVtable && observer.EventsToObserve.Head==0; return &node; }
int main()
{
    memset(&observer,0xA5,sizeof(observer)); memset(&node,0xCD,sizeof(node));
    if(FableUiConstructObserver(&observer,0)!=&observer || !valid || observer.EventsToObserve.Head!=&node || observer.EventsToObserve.Count || observer.PreventObservation || node.Colour || node.Parent || node.Left!=&node || node.Right!=&node || node.Event!=static_cast<int>(0xCDCDCDCD) || observer.EventsToObserve.Unrecovered08[0]!=0xA5) return 1;
    puts("FRONTEND_0052d9e0_TEST PASS"); return 0;
}
