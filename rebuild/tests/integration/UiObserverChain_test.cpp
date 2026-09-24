#include "fable_ui_observer.h"
#include "fable_ui_manager_singleton.h"
#include "fable_ui_observer_events.h"
#include <stdio.h>
#include <string.h>
static FableUiComponentDrawView components[5];
static FableUiComponentCountedStorage children[5][4];
static FableUiObserverListNode links[7];
static FableUiManagerObserverView manager;
static FableUiEventTreeNode eventTrees[5][2];
void FableUiFreeEventNode(void* p)
{
    for(unsigned i=0;i<5;++i) if(p==&eventTrees[i][1]) { printf(" T%u",i); return; }
    printf(" BADFREE");
}
extern "C" void* FableFrontEndManagerInstance=0;
extern "C" void* FableFrontEndManagerAllocate(unsigned long) { return 0; }
extern "C" void* __fastcall FableFrontEndManagerConstruct(void*) { return 0; }
void FableUiFreeObserverNode(void* p) { printf(" F%u",static_cast<unsigned>(static_cast<FableUiObserverListNode*>(p)-links)); }
static unsigned ObserverId(FableUiObserverInterfaceView* p)
{
    for(unsigned i=0;i<5;++i) if(p==&components[i].Observer) return i;
    return 99;
}
static void __fastcall Notify(FableUiObserverInterfaceView* p,void*) { printf(" N%u",ObserverId(p)); }
static void __fastcall Removed(FableUiComponentDrawView* p,void*) { printf(" H%u",static_cast<unsigned>(p-components)); }
static void __fastcall Request(FableUiComponentDrawView* p,void*,unsigned state) { printf(" S%u:%u",static_cast<unsigned>(p-components),state); }
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    unsigned operation,shape,mask,duplicate;
    while(fscanf(f,"%u %u %u %u",&operation,&shape,&mask,&duplicate)==4)
    {
        memset(components,0,sizeof(components)); memset(children,0,sizeof(children)); memset(links,0,sizeof(links));
        FableUiComponentDrawVtable ct={}; ct.ObserverRemoved=Removed; ct.RequestState=Request;
        FableUiObserverNotificationVtable ot={}; ot.Removed=Notify;
#ifdef REAL_OBSERVER_CLEANUP
        ot.Removed=reinterpret_cast<void (__fastcall *)(FableUiObserverInterfaceView*,void*)>(FableUiClearObservedEvents);
#endif
        FableUiManagerObserverVtable mt={}; mt.RemoveObserver=FableUiRemoveObserver;
        manager.Vtable=&mt; manager.Observers=links; FableFrontEndManagerInstance=&manager;
        links[0].Next=links[0].Previous=links;
        for(unsigned i=0;i<5;++i)
        {
            components[i].Vtable=&ct; components[i].Observer.Vtable=&ot;
#ifdef REAL_OBSERVER_CLEANUP
            FableUiObserverEventsView* observer=reinterpret_cast<FableUiObserverEventsView*>(&components[i].Observer);
            FableUiEventTreeNode* head=eventTrees[i]; FableUiEventTreeNode* node=head+1;
            memset(head,0,2*sizeof(*head)); head->Parent=head->Left=head->Right=node; node->Parent=head; node->Event=25;
            observer->EventsToObserve.Head=head; observer->EventsToObserve.Count=1;
#endif
            components[i].Children.Begin=components[i].Children.End=children[i]; components[i].Children.CapacityEnd=children[i]+4;
            if(mask&(1<<i))
            {
                FableUiObserverListNode* n=links+i+1; n->Observer=&components[i].Observer;
                n->Previous=links[0].Previous; n->Next=links; links[0].Previous->Next=n; links[0].Previous=n;
            }
        }
        if(duplicate)
        {
            links[6].Observer=&components[0].Observer; links[6].Next=links; links[6].Previous=links[0].Previous;
            links[0].Previous->Next=links+6; links[0].Previous=links+6;
        }
        if(shape==1)
            for(unsigned i=1;i<5;++i) { children[0][i-1].Data=components+i; ++components[0].Children.End; }
        if(shape==2)
            for(unsigned i=0;i<4;++i) { children[i][0].Data=components+i+1; ++components[i].Children.End; }
        if(shape==3)
        {
            children[0][0].Data=components+1; children[0][1].Data=components+2; components[0].Children.End+=2;
            children[1][0].Data=components+3; ++components[1].Children.End;
            children[2][0].Data=components+4; ++components[2].Children.End;
        }
        printf("TRACE"); if(operation) FableUiDie(components,0); else FableUiRemoveObserverRecursive(components,0);
        printf(" L");
        for(FableUiObserverListNode* n=links[0].Next;n!=links;n=n->Next)
        {
            if(n->Next->Previous!=n || n->Previous->Next!=n) return 4;
            printf(" %u:%u",static_cast<unsigned>(n-links),ObserverId(n->Observer));
        }
        if(links[0].Next->Previous!=links || links[0].Previous->Next!=links) return 5;
#ifdef REAL_OBSERVER_CLEANUP
        for(unsigned i=0;i<5;++i)
        {
            FableUiObserverEventsView* observer=reinterpret_cast<FableUiObserverEventsView*>(&components[i].Observer);
            FableUiEventTreeNode* head=eventTrees[i];
            if(!observer->EventsToObserve.Count && (head->Parent || head->Left!=head || head->Right!=head)) return 6;
            printf(" E%u",observer->EventsToObserve.Count);
        }
#endif
        printf(" END\n");
    }
    fclose(f); return 0;
}
