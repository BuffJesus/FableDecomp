#include "fable_ui_deletion.h"
#include <stdio.h>
#include <string.h>
static unsigned char arena[8192];
static bool active[512], recording;
static unsigned allocations, mutation;
static FableUiComponentDrawView parent, child, foreign[2];
static unsigned Id(void* p) { return static_cast<unsigned>((static_cast<unsigned char*>(p)-arena)/16); }
void* __cdecl FableUiListAllocate(unsigned size)
{
    if (size!=12 || allocations>=512) return 0;
    unsigned id=allocations++; active[id]=true;
    if (recording) printf(" A%u",id);
    return arena+id*16;
}
void __cdecl FableUiListFree(void* p)
{
    unsigned id=Id(p);
    if (id>=allocations || !active[id]) { printf(" BADFREE"); return; }
    active[id]=false; if (recording) printf(" F%u",id);
}
static FableUiComponentDrawView* Value(unsigned i, unsigned pattern)
{ return ((i+pattern)%3)==0 ? &parent : &foreign[(i+pattern)%3-1]; }
static unsigned ValueId(FableUiComponentDrawView* p)
{ return p==&parent ? 0 : static_cast<unsigned>(p-foreign)+1; }
static void Make(FableUiDeletion& value,unsigned method,unsigned count,unsigned pattern)
{
    value.Method=method; value.AssociatedParents=FableUiCreateDeletionList();
    for (unsigned i=0;i<count;++i) FableUiAppendDeletionParent(value.AssociatedParents,Value(i,pattern));
}
static FableUiDeletion* __fastcall Get(FableUiComponentDrawView* c,void*)
{ printf(" G"); return FableUiGetDeletion(c,0); }
static void __fastcall Set(FableUiComponentDrawView* c,void*,FableUiDeletion value)
{ printf(" S%u",value.Method); FableUiSetDeletion(c,0,value); }
static void __fastcall Remove(FableUiComponentDrawView*,void*,unsigned index)
{
    printf(" R%u",index);
    if (mutation)
    {
        FableUiDestroyDeletionParents(&child.Deletion.AssociatedParents,0);
        child.Deletion.Method=9; child.Deletion.AssociatedParents=FableUiCreateDeletionList();
    }
}
static void Dump(const FableUiDeletion& value)
{
    FableUiDeletionParentNode* head=value.AssociatedParents;
    printf(" %u:H%u",value.Method,Id(head));
    for (FableUiDeletionParentNode* n=head->Next;n!=head;n=n->Next) printf(" %u:N%u",ValueId(n->Parent),Id(n));
    printf(" /");
}
int main(int argc,char** argv)
{
    if (argc!=2) return 2;
    FILE* input=fopen(argv[1],"r"); if (!input) return 3;
    unsigned v[6];
    while (fscanf(input,"%u %u %u %u %u %u",v,v+1,v+2,v+3,v+4,v+5)==6)
    {
        allocations=0; recording=false; mutation=v[5]; memset(active,0,sizeof(active)); memset(arena,0xCC,sizeof(arena));
        FableUiComponentDrawVtable table={}; table.GetDeletion=Get; table.SetDeletion=Set; table.RemoveChildAt=Remove;
        child.Vtable=parent.Vtable=&table;
        FableUiComponentCountedStorage entry={&child,0}; parent.Children.Begin=&entry; parent.Children.End=&entry+1;
        Make(child.Deletion,v[1],v[2],v[4]);
        FableUiDeletion other;
        if (v[0]!=3) Make(other,v[1]+1,v[3],v[4]+1);
        printf("TRACE"); recording=true;
        switch(v[0])
        {
        case 0: FableUiProcessLiveChildDeletion(&parent,0); break;
        case 1:
            if (FableUiAssignDeletionParents(&child.Deletion.AssociatedParents,0,&other.AssociatedParents)!=&child.Deletion.AssociatedParents) return 4;
            break;
        case 2: FableUiSetDeletion(&child,0,other); break;
        case 3: if (FableUiCopyDeletion(&other,0,&child.Deletion)!=&other) return 5; break;
        case 4: FableUiDestroyDeletionParents(&child.Deletion.AssociatedParents,0); break;
        case 5: FableUiAssignDeletionParents(&child.Deletion.AssociatedParents,0,&child.Deletion.AssociatedParents); break;
        }
        printf(" END"); if (v[0]!=4) Dump(child.Deletion); else printf(" X /");
        if (v[0]!=2) Dump(other); else printf(" X /");
        printf(" LIVE"); for (unsigned i=0;i<allocations;++i) if (active[i]) printf(" %u",i);
        printf("\n");
    }
    fclose(input); return 0;
}
