#include "fable_ui_observer_events.h"
#include <stdio.h>
#include <string.h>
FableUiObserveEventVtable FableUiObserverVtable={};
static FableUiObserveEventVtable secondTable={};
static FableUiEventTreeNode nodes[8];
static unsigned flip;
void* FableUiAllocateEventNode(unsigned size) { printf(" A%u",size); return nodes; }
void FableUiFreeEventNode(void* p) { printf(" F%u",static_cast<unsigned>(static_cast<FableUiEventTreeNode*>(p)-nodes)); }
static void __fastcall Observe0(FableUiObserverEventsView* p,void*,int event)
{ printf(" O0:%d",event); if(flip) p->Vtable=&secondTable; }
static void __fastcall Observe1(FableUiObserverEventsView* p,void*,int event)
{ printf(" O1:%d",event); if(flip) p->Vtable=&FableUiObserverVtable; }
int main(int argc,char** argv)
{
    if(argc!=2) return 2; FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    unsigned count,shape,prevent,op; int event;
    const int values[7]={(-2147483647-1),-2,0,1,25,37,2147483647};
    const unsigned balanced[7]={3,1,5,0,2,4,6};
    while(fscanf(f,"%u %u %d %u %u %u",&count,&shape,&event,&prevent,&flip,&op)==6)
    {
        FableUiObserverEventsView observer; memset(&observer,0xA5,sizeof(observer)); memset(nodes,0xCD,sizeof(nodes));
        FableUiObserverVtable.ObserveEvent=Observe0; secondTable.ObserveEvent=Observe1;
        printf("TRACE");
        if(FableUiConstructObserver(&observer,0)!=&observer || observer.Vtable!=&FableUiObserverVtable) return 4;
        if(observer.PreventObservation || observer.EventsToObserve.Count || nodes[0].Event!=static_cast<int>(0xCDCDCDCD) || observer.EventsToObserve.Unrecovered08[0]!=0xA5 || observer.Unrecovered11[0]!=0xA5) return 5;
        for(unsigned j=0;j<7;++j)
        {
            unsigned index=shape==0 ? j : shape==1 ? 6-j : balanced[j]; if(index>=count) continue;
            FableUiEventTreeNode* node=nodes+index+1; node->Event=values[index]; node->Left=node->Right=0; node->Colour=1;
            FableUiEventTreeNode* parent=nodes; FableUiEventTreeNode** link=&nodes[0].Parent;
            while(*link) { parent=*link; link=node->Event<parent->Event ? &parent->Left : &parent->Right; }
            node->Parent=parent; *link=node;
        }
        if(count) { nodes[0].Left=nodes+1; nodes[0].Right=nodes+count; }
        observer.EventsToObserve.Count=count; observer.PreventObservation=static_cast<unsigned char>(prevent);
        if(op==0) printf(" R%u",static_cast<unsigned>(FableUiAcceptsEvent(&observer,0,event)));
        else if(op==1)
        {
            FableUiEventTreeNode* result;
            if(FableUiFindObservedEvent(&observer.EventsToObserve,0,&result,&event)!=&result) return 6;
            printf(" R%u",static_cast<unsigned>(result-nodes));
        }
        else FableUiObserveAllEvents(&observer,0);
        FableUiClearObservedEvents(&observer,0);
        if(observer.EventsToObserve.Count || nodes[0].Parent || nodes[0].Left!=nodes || nodes[0].Right!=nodes) return 7;
        printf(" C%u:%u",observer.EventsToObserve.Count,observer.PreventObservation); FableUiFreeEventNode(nodes); printf(" END\n");
    }
    fclose(f); return 0;
}
