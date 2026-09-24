#include "fable_ui_observer_events.h"
#include <stdio.h>
#include <string.h>
FableUiObserveEventVtable FableUiObserverVtable={};
static FableUiObserverEventsView observer;
static FableUiEventTreeNode nodes[256];
static bool live[256];
static unsigned allocated,mode;
void* FableUiAllocateEventNode(unsigned size)
{ if(allocated>=256) return 0; printf(" A%u:%u",allocated,size); live[allocated]=true; return nodes+allocated++; }
void FableUiFreeEventNode(void* p)
{ unsigned i=static_cast<unsigned>(static_cast<FableUiEventTreeNode*>(p)-nodes); if(i>=256 || !live[i]) printf(" BADFREE"); else live[i]=false; printf(" F%u",i); }
static void __fastcall Process(FableUiObserverInterfaceView*,void*,unsigned event)
{
    printf(" P%u:%u",event,observer.EventsToObserve.Count);
    if(mode) FableUiClearObservedEvents(&observer,0);
}
static int Id(FableUiEventTreeNode* p) { return p ? static_cast<int>(p-nodes) : -1; }
static void Snapshot()
{
    printf(" D%u",observer.EventsToObserve.Count);
    for(unsigned i=0;i<allocated;++i) if(live[i])
    { FableUiEventTreeNode& n=nodes[i]; printf(" V%u:%u:%d:%d:%d",i,n.Colour,Id(n.Parent),Id(n.Left),Id(n.Right)); }
}
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    unsigned op,prevent,count;
    FableUiObserverVtable.Unrecovered00[1]=reinterpret_cast<void*>(Process);
    FableUiObserverVtable.ObserveEvent=FableUiObserveEvent;
    while(fscanf(f,"%u %u %u %u",&op,&mode,&prevent,&count)==4)
    {
        memset(nodes,0xCD,sizeof(nodes)); memset(live,0,sizeof(live)); memset(&observer,0xA5,sizeof(observer)); allocated=0;
        printf("TRACE"); FableUiConstructObserver(&observer,0); observer.PreventObservation=static_cast<unsigned char>(prevent);
        if(count>128) return 6;
        int values[128]; for(unsigned i=0;i<count;++i) if(fscanf(f,"%d",values+i)!=1) return 4;
        if(op==4) for(unsigned i=0;i<count;++i) FableUiObserveEvent(&observer,0,values[i]);
        for(unsigned i=0;i<count;++i)
        {
            int event=values[i];
            if(op==4 || (op==3 && (i&1))) { FableUiRemoveObservedEvent(&observer,0,event); Snapshot(); }
            else if(op==2) FableUiObserveAllEvents(&observer,0);
            else if(op==1)
            {
                FableUiEventInsertResult result;
                if(FableUiInsertObservedEvent(&observer.EventsToObserve,0,&result,&event)!=&result) return 5;
                printf(" I%d:%u",Id(result.Node),static_cast<unsigned>(result.Inserted));
            }
            else FableUiObserveEvent(&observer,0,event);
        }
        printf(" C%u:%u",observer.EventsToObserve.Count,observer.PreventObservation);
        for(unsigned i=0;i<allocated;++i) if(live[i])
        { FableUiEventTreeNode& n=nodes[i]; printf(" T%u:%u:%d:%d:%d:%d",i,n.Colour,Id(n.Parent),Id(n.Left),Id(n.Right),n.Event); }
        FableUiClearObservedEvents(&observer,0); FableUiFreeEventNode(nodes);
        unsigned remaining=0; for(unsigned i=0;i<allocated;++i) if(live[i]) ++remaining;
        printf(" LIVE%u END\n",remaining);
    }
    fclose(f); return 0;
}
