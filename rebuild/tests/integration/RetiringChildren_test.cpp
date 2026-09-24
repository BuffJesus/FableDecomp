#include "fable_ui_counted_children.h"
#include <stdio.h>
#include <string.h>
static FableUiComponentDrawView parent, children[4];
static FableReferenceCount references[4];
static unsigned complete, current;
static FableUiComponentCountedStorage allocated[32];
void* __cdecl FableUiAllocateChildStorage(unsigned bytes) { printf(" A%u",bytes); return allocated; }
void __cdecl FableUiFreeChildStorage(void*) { printf(" V"); }
static void __fastcall Die(FableUiComponentDrawView* child,void*) { printf(" Y%u",static_cast<unsigned>(child-children)); }
static bool __fastcall Completed(FableUiComponentDrawView* child, void*)
{ printf(" H%u", static_cast<unsigned>(child-children)); return complete != 0; }
static unsigned __fastcall Current(FableUiComponentDrawView* child, void*)
{ printf(" S%u", static_cast<unsigned>(child-children)); return current; }
static void __fastcall Request(FableUiComponentDrawView* child, void*, unsigned state)
{ printf(" R%u:%u", static_cast<unsigned>(child-children),state); }
static void __fastcall SetParent(FableUiComponentDrawView* child, void*, FableUiComponentDrawView* value)
{ printf(" P%u", static_cast<unsigned>(child-children)); child->Parent=value; }
static void __fastcall Destroy(void* object)
{ printf(" D%u", static_cast<unsigned>(static_cast<FableUiComponentDrawView*>(object)-children)); }
void __cdecl FableUiDeleteReference(FableReferenceCount* info)
{ printf(" F%u", static_cast<unsigned>(info-references)); }
int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input=fopen(argv[1],"r"); if (!input) return 3;
    unsigned v[14];
    while (fscanf(input,"%u",v)==1)
    {
        for (unsigned i=1;i<14;++i) if (fscanf(input,"%u",v+i)!=1) return 4;
        memset(&parent,0,sizeof(parent)); memset(children,0,sizeof(children));
        FableUiComponentDrawVtable table={}; table.HasCompletedStateChange=Completed;
        table.GetCurrentState=Current; table.RequestState=Request; table.SetParent=SetParent;
        table.Die=Die;
        FableUiComponentCountedStorage entries[4]={};
        FableUiComponentCountedStorage retired[16]={};
        unsigned shapes[]={0,2,1,3,1,5};
        complete=v[2]; current=v[3];
        for (unsigned r=0;r<4;++r)
        {
            children[r].Vtable=&table; children[r].Parent=&parent; children[r].Deletion.Method=v[4];
            references[r].owners=v[5]; references[r].destroy=Destroy; references[r].object=&children[r];
            entries[r].Data=&children[v[6+r]];
            entries[r].Info=v[10+r]<4 ? &references[v[10+r]] : 0;
        }
        // Count entries after initializing every potentially shared block.
        for (unsigned r3=0;r3<v[0];++r3) if (entries[r3].Info) ++entries[r3].Info->owners;
        parent.ChildrenToDelete.Begin=entries; parent.ChildrenToDelete.End=entries+v[0]; parent.ChildrenToDelete.CapacityEnd=entries+4;
        if (v[2]==6)
        {
            parent.Children=parent.ChildrenToDelete;
            for (unsigned r4=0;r4<v[3];++r4) { retired[r4]=entries[(r4+2)%4]; if(retired[r4].Info) ++retired[r4].Info->owners; }
            parent.ChildrenToDelete.Begin=retired; parent.ChildrenToDelete.End=retired+v[3]; parent.ChildrenToDelete.CapacityEnd=retired+(v[4] ? 16 : v[3]);
            parent.ShapeChildren.Begin=shapes; parent.ShapeChildren.End=parent.ShapeChildren.CapacityEnd=shapes+6;
        }
        printf("TRACE");
        unsigned result;
        if (v[2] == 2) result=static_cast<unsigned>(FableUiFindCountedChild(entries,entries+v[0],entries+v[1],0)-entries);
        else if (v[2] == 3) result=static_cast<unsigned>(FableUiMoveCountedChildren(entries+v[1],entries+v[0],entries,0,0)-entries);
        else if (v[2] == 4) { FableUiEraseCountedChild(&parent.ChildrenToDelete,0,entries+v[1]); result=parent.ChildrenToDelete.Size(); }
        else if (v[2] == 5) { FableUiReallocateCountedChildren(&parent.ChildrenToDelete,0,entries+v[1],entries,0,v[3],v[4]!=0); result=parent.ChildrenToDelete.Size(); }
        else if (v[2] == 6) { FableUiRemoveChildAt(&parent,0,v[1]); result=parent.Children.Size(); }
        else { FableUiFinishRetiringChild(&parent,v[1]); result=static_cast<unsigned>(parent.ChildrenToDelete.End-entries); }
        printf(" END %u",result);
        for (unsigned e=0;e<4;++e) printf(" %u %u",entries[e].Data ? static_cast<unsigned>(entries[e].Data-children)+1 : 0,
            entries[e].Info ? static_cast<unsigned>(entries[e].Info-references)+1 : 0);
        for (unsigned c=0;c<4;++c) printf(" %u %u",references[c].owners,children[c].Parent != 0);
        if (v[2]>=4)
        {
            printf(" EXTRA %u %u",parent.ChildrenToDelete.Size(),static_cast<unsigned>(parent.ChildrenToDelete.CapacityEnd-parent.ChildrenToDelete.Begin));
            for (unsigned x=0;x<parent.ChildrenToDelete.Size();++x)
            {
                const FableUiComponentCountedStorage& e=parent.ChildrenToDelete.Begin[x];
                printf(" %u %u",e.Data ? static_cast<unsigned>(e.Data-children)+1 : 0,e.Info ? static_cast<unsigned>(e.Info-references)+1 : 0);
            }
            if (v[2]==6) { printf(" SHAPE"); for (unsigned s=0;s<parent.ShapeChildren.Size();++s) printf(" %u",shapes[s]); }
        }
        printf("\n");
    }
    fclose(input); return 0;
}
