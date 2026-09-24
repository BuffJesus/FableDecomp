#include "fable_ui_observer.h"
#include <stdio.h>
#include <string.h>
static FableUiComponentDrawView nodes[5];
static FableUiComponentCountedStorage entries[5][4], replacement[2];
static FableUiManagerObserverView managers[2];
static unsigned mode, alternating, calls;
void* __cdecl FableUiGetManager()
{
    unsigned id=alternating ? calls%2 : 0; ++calls; printf(" M%u",id); return &managers[id];
}
static void __fastcall Remove(FableUiManagerObserverView* manager,void*,FableUiObserverInterfaceView* observer)
{
    FableUiComponentDrawView* node=reinterpret_cast<FableUiComponentDrawView*>(reinterpret_cast<unsigned char*>(observer)-offsetof(FableUiComponentDrawView,Observer));
    printf(" R%u:%u",static_cast<unsigned>(manager-managers),static_cast<unsigned>(node-nodes));
    if (mode==3 && node==nodes) *nodes[0].Children.End++=replacement[0];
}
static void __fastcall Removed(FableUiComponentDrawView* node,void*)
{
    printf(" H%u",static_cast<unsigned>(node-nodes));
    if (mode==1 && node==nodes) { nodes[0].Children.Begin=replacement; nodes[0].Children.End=replacement+2; }
    if (mode==2 && node==nodes+1) nodes[0].Children.End=nodes[0].Children.Begin+1;
}
static void __fastcall Request(FableUiComponentDrawView* node,void*,unsigned state)
{
    printf(" S%u:%u",static_cast<unsigned>(node-nodes),state);
    if (mode==4) { nodes[0].Children.Begin=replacement; nodes[0].Children.End=replacement+1; }
}
int main(int argc,char** argv)
{
    if (argc!=2) return 2;
    FILE* input=fopen(argv[1],"r"); if(!input) return 3;
    unsigned operation,shape;
    while(fscanf(input,"%u %u %u %u",&operation,&shape,&mode,&alternating)==4)
    {
        memset(nodes,0,sizeof(nodes)); memset(entries,0,sizeof(entries)); calls=0;
        FableUiComponentDrawVtable table={}; table.ObserverRemoved=Removed; table.RequestState=Request;
        FableUiManagerObserverVtable managerTable={}; managerTable.RemoveObserver=Remove;
        managers[0].Vtable=managers[1].Vtable=&managerTable;
        replacement[0].Data=&nodes[4]; replacement[1].Data=&nodes[2];
        for(unsigned i=0;i<5;++i)
        {
            nodes[i].Vtable=&table; nodes[i].Children.Begin=nodes[i].Children.End=entries[i]; nodes[i].Children.CapacityEnd=entries[i]+4;
            nodes[i].ChildrenToDelete.Begin=replacement; nodes[i].ChildrenToDelete.End=replacement+1;
        }
        if(shape)
        {
            entries[0][0].Data=&nodes[1]; entries[0][1].Data=&nodes[2]; nodes[0].Children.End=entries[0]+2;
            if(shape==2) { entries[1][0].Data=&nodes[3]; nodes[1].Children.End=entries[1]+1; }
            if(shape==3)
            {
                nodes[0].Children.End=entries[0]+1;
                for(unsigned n=1;n<3;++n) { entries[n][0].Data=&nodes[n+1]; nodes[n].Children.End=entries[n]+1; }
            }
        }
        printf("TRACE"); if(operation) FableUiDie(nodes,0); else FableUiRemoveObserverRecursive(nodes,0); printf(" END\n");
    }
    fclose(input); return 0;
}
