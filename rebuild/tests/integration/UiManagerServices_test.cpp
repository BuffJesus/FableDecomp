#include "fable_ui_observer.h"
#include "fable_ui_manager_singleton.h"
#include <stdio.h>
#include <string.h>

static unsigned char storage[2][0xD0];
static unsigned failAllocation, constructResult, mutation;
extern "C" void* FableFrontEndManagerInstance = 0;
static unsigned Id(void* p) { return !p ? 0 : p == storage[0] ? 1 : 2; }
extern "C" void* FableFrontEndManagerAllocate(unsigned long size)
{ printf(" A%lu", size); return failAllocation ? 0 : storage[0]; }
extern "C" void* __fastcall FableFrontEndManagerConstruct(void* p)
{ printf(" C%u", Id(p)); return constructResult ? storage[constructResult-1] : 0; }

static FableUiObserverListNode nodes[9];
static FableUiObserverListNode others[5];
static FableUiObserverInterfaceView observers[4];
static unsigned Count()
{
    unsigned result=0;
    for(FableUiObserverListNode* n=nodes[0].Next;n!=nodes;n=n->Next) ++result;
    return result;
}
void FableUiFreeObserverNode(void* p)
{
    unsigned index=99;
    for(unsigned i=0;i<9;++i) if(p==nodes+i) index=i;
    for(unsigned i=0;i<5;++i) if(p==others+i) index=16+i;
    printf(" F%u",index);
}
void* FableUiAllocateObserverNode(unsigned size) { printf(" A%u",size); return nodes+7; }
static void __fastcall Notify(FableUiObserverInterfaceView* observer,void*)
{
    printf(" N%u:%u",static_cast<unsigned>(observer-observers),Count());
    if(mutation==1)
    {
        nodes[8].Observer=&observers[3]; nodes[8].Next=nodes;
        nodes[8].Previous=nodes[0].Previous;
        nodes[0].Previous->Next=nodes+8; nodes[0].Previous=nodes+8;
    }
    if(mutation==2)
    {
        FableUiObserverListNode* current=nodes[0].Next;
        while(current->Observer!=observer) current=current->Next;
        FableUiObserverListNode* next=current->Next;
        if(next!=nodes)
        { printf(" X%u",static_cast<unsigned>(next-nodes)); current->Next=next->Next; next->Next->Previous=current; }
    }
}
int main(int argc,char** argv)
{
    if(argc!=2) return 2;
    FILE* f=fopen(argv[1],"r"); if(!f) return 3;
    unsigned op,a,b,c,d;
    while(fscanf(f,"%u %u %u %u %u",&op,&a,&b,&c,&d)==5)
    {
        printf("TRACE");
        if(!op)
        {
            FableFrontEndManagerInstance=a ? storage[1] : 0;
            failAllocation=b; constructResult=c;
            for(unsigned i=0;i<d;++i) printf(" R%u",Id(FableUiGetManager()));
            printf(" G%u",Id(FableFrontEndManagerInstance));
        }
        else
        {
            memset(nodes,0,sizeof(nodes)); mutation=d;
            FableUiObserverNotificationVtable vt={}; vt.Removed=Notify;
            for(unsigned i=0;i<4;++i) observers[i].Vtable=&vt;
            nodes[0].Next=a ? nodes+1 : nodes; nodes[0].Previous=nodes+a;
            for(unsigned i=1;i<=a;++i)
            { nodes[i].Next=i==a ? nodes : nodes+i+1; nodes[i].Previous=nodes+i-1; nodes[i].Observer=observers+(i-1+c)%3; }
            memset(others,0,sizeof(others)); others[0].Next=b ? others+1 : others; others[0].Previous=others+b;
            for(unsigned j=1;j<=b;++j)
            { others[j].Next=j==b ? others : others+j+1; others[j].Previous=others+j-1; others[j].Observer=observers+j-1; }
            FableUiManagerObserverView manager={}; manager.Observers=nodes; manager.ConcurrentExclusiveObservers=others;
            manager.ExclusiveObserver=c ? observers+c-1 : 0;
            if(op==2 || op==4) { manager.Observers=others; manager.ConcurrentExclusiveObservers=nodes; }
            switch(op)
            {
                case 1: FableUiRemoveObserver(&manager,0,observers+b); break;
                case 2: FableUiRemoveConcurrentExclusiveObserver(&manager,0,observers+b); break;
                case 3: FableUiAddObserver(&manager,0,observers+b); break;
                case 4: FableUiAddConcurrentExclusiveObserver(&manager,0,observers+b); break;
                case 5: FableUiSetExclusiveObserver(&manager,0,observers+b); break;
                case 6: FableUiClearExclusiveObserver(&manager,0,observers+b); break;
                case 7: printf(" H%u",static_cast<unsigned>(FableUiHasExclusiveObservers(&manager,0))); break;
                case 8: FableUiClearObservers(&manager,0); break;
            }
            printf(" L");
            for(FableUiObserverListNode* n=nodes[0].Next;n!=nodes;n=n->Next)
            { if(n->Next->Previous!=n || n->Previous->Next!=n) return 4;
              printf(" %u:%u",static_cast<unsigned>(n-nodes),static_cast<unsigned>(n->Observer-observers)); }
            if(nodes[0].Next->Previous!=nodes || nodes[0].Previous->Next!=nodes) return 5;
            printf(" O");
            for(FableUiObserverListNode* n=others[0].Next;n!=others;n=n->Next)
            { if(n->Next->Previous!=n || n->Previous->Next!=n) return 6;
              printf(" %u:%u",16+static_cast<unsigned>(n-others),static_cast<unsigned>(n->Observer-observers)); }
            if(others[0].Next->Previous!=others || others[0].Previous->Next!=others) return 7;
            printf(" E%d",manager.ExclusiveObserver ? static_cast<int>(manager.ExclusiveObserver-observers) : -1);
        }
        printf(" END\n");
    }
    fclose(f); return 0;
}
