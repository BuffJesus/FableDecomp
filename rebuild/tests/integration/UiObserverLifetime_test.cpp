#include "fable_ui_observer_lifetime.h"
#include "fable_ui_observer_events.h"
#include <stdio.h>
#include <string.h>
FableUiManagerObserverVtable FableUiObservableVtable={};
static FableUiManagerObserverView manager;
static FableUiObserverListNode pool[64];
static bool live[64];
static unsigned allocated,mutation,acceptMask;
static bool mutated;
static FableUiObserverEventsView observers[5];
static FableUiEventTreeNode eventNodes[5][2];
static FableUiObserverInterfaceView* ObserverAt(unsigned i)
{ return reinterpret_cast<FableUiObserverInterfaceView*>(observers+i); }
static unsigned ObserverId(FableUiObserverInterfaceView* p)
{ for(unsigned i=0;i<5;++i) if(p==ObserverAt(i)) return i; return 99; }
static unsigned NodeId(FableUiObserverListNode* p)
{
    if(!p) return 98;
    for(unsigned i=0;i<64;++i) if(p==pool+i) return i;
    return 99;
}
void* FableUiAllocateObserverNode(unsigned size)
{
    if(allocated==64) return 0;
    unsigned id=allocated++; live[id]=true;
    printf(" A%u:%u",id,size); return pool+id;
}
void FableUiFreeObserverNode(void* p)
{
    unsigned id=NodeId(static_cast<FableUiObserverListNode*>(p));
    if(id>=64 || !live[id]) { printf(" BADFREE"); return; }
    live[id]=false; printf(" F%u",id);
}
static void Mutate(unsigned event)
{
    if(mutated) return; mutated=true;
    switch(mutation)
    {
        case 1: FableUiClearObservers(&manager,0); break;
        case 2: FableUiAddObserver(&manager,0,ObserverAt(4)); FableUiAddConcurrentExclusiveObserver(&manager,0,ObserverAt(4)); break;
        case 3: manager.ExclusiveObserver=ObserverAt(4); break;
        case 5: FableUiDispatchEvent(&manager,0,event+1); break;
    }
}
static bool __fastcall Accept(FableUiObserverInterfaceView* p,void*,unsigned event)
{
    unsigned id=ObserverId(p); printf(" Q%u:%u",id,event);
#ifdef REAL_EVENT_FILTER
    return FableUiAcceptsEvent(observers+id,0,static_cast<int>(event));
#else
    // Clearing the exclusive target during a successful query would make retail
    // dereference null. Exercise list clearing only on the snapshot path.
    if(mutation!=4) Mutate(event);
    return (acceptMask&(1<<id))!=0;
#endif
}
static void __fastcall Process(FableUiObserverInterfaceView* p,void*,unsigned event)
{
    printf(" P%u:%u",ObserverId(p),event);
    if(mutation==4) manager.ExclusiveObserver=ObserverAt(4);
}
static void Append(FableUiObserverListNode* head,unsigned id)
{
    FableUiObserverListNode* n=static_cast<FableUiObserverListNode*>(FableUiAllocateObserverNode(12));
    n->Observer=ObserverAt(id); n->Next=head; n->Previous=head->Previous; head->Previous->Next=n; head->Previous=n;
}
static void PrintList(FableUiObserverListNode* head)
{
    for(FableUiObserverListNode* n=head->Next;n!=head;n=n->Next)
    {
        if(n->Next->Previous!=n || n->Previous->Next!=n) { printf(" BADLINK"); return; }
        printf(" %u:%u",NodeId(n),ObserverId(n->Observer));
    }
    if(head->Next->Previous!=head || head->Previous->Next!=head) printf(" BADHEAD");
}
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    unsigned normal,concurrent,pattern,exclusive,event;
    while(fscanf(f,"%u %u %u %u %u %u %u",&normal,&concurrent,&pattern,&exclusive,&acceptMask,&mutation,&event)==7)
    {
        memset(&manager,0xA5,sizeof(manager)); memset(pool,0xCD,sizeof(pool)); memset(live,0,sizeof(live)); allocated=0; mutated=false;
        FableUiObserverEventVtable vt={}; vt.AcceptsEvent=Accept; vt.ProcessEvent=Process;
        memset(observers,0,sizeof(observers)); memset(eventNodes,0,sizeof(eventNodes));
        for(unsigned i=0;i<5;++i)
        {
            observers[i].Vtable=&vt;
            FableUiEventTreeNode* head=eventNodes[i]; FableUiEventTreeNode* node=head+1;
            bool registered=(acceptMask&(1<<i))!=0;
            head->Parent=registered ? node : 0; head->Left=head->Right=registered ? node : head;
            node->Parent=head; node->Event=static_cast<int>(event);
            observers[i].EventsToObserve.Head=head; observers[i].EventsToObserve.Count=registered ? 1 : 0;
            observers[i].PreventObservation=i==pattern ? 1 : 0;
        }
        printf("TRACE");
        if(FableUiConstructObservable(&manager,0)!=&manager || manager.Vtable!=&FableUiObservableVtable || manager.ExclusiveObserver) return 4;
        if(manager.Observers->Observer!=reinterpret_cast<FableUiObserverInterfaceView*>(0xCDCDCDCD) || manager.ConcurrentExclusiveObservers->Observer!=reinterpret_cast<FableUiObserverInterfaceView*>(0xCDCDCDCD)) return 5;
        for(unsigned i=0;i<normal;++i) Append(manager.Observers,(i+pattern)%3);
        for(unsigned i=0;i<concurrent;++i) Append(manager.ConcurrentExclusiveObservers,(i+pattern+1)%3);
        manager.ExclusiveObserver=exclusive ? ObserverAt(exclusive-1) : 0;
        FableUiDispatchEvent(&manager,0,event);
        printf(" L"); PrintList(manager.Observers); printf(" C"); PrintList(manager.ConcurrentExclusiveObservers);
        FableUiObserverInterfaceView* target=FableUiGetExclusiveObserver(&manager,0);
        printf(" E%d",target ? static_cast<int>(ObserverId(target)) : -1);
        FableUiDestroyObservable(&manager,0);
        unsigned remaining=0; for(unsigned i=0;i<allocated;++i) if(live[i]) ++remaining;
        printf(" LIVE%u END\n",remaining);
    }
    fclose(f); return 0;
}
