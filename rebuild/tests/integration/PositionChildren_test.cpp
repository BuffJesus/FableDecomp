#include "fable_ui_child_update.h"
#include "fable_ui_deletion.h"
#include "fable_ui_counted_children.h"
#include "fable_ui_state_progress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateSourceExtent, FableUiCoordinateDestinationExtent;
static FableUiComponentDrawView parent, children[4];
static FableUiPositionChildNode head, nodes[3];
static unsigned independent, zoom, relative, mutation, zoomCalls;
static bool live;
static bool retiring;
static bool frame, recording;
static unsigned acceptForeign;
static FableUiComponentCountedStorage entries[4], retiringEntries[8];
void* __cdecl FableUiListAllocate(unsigned bytes) { if (recording) printf(" a"); return malloc(bytes); }
void __cdecl FableUiListFree(void* p) { if (recording) printf(" f"); free(p); }
void __cdecl FableUiDeleteReference(FableReferenceCount* p) { free(p); }
void* __cdecl FableUiAllocateChildStorage(unsigned bytes) { return malloc(bytes); }
void __cdecl FableUiFreeChildStorage(void* p) { free(p); }
static unsigned Bits(float value) { unsigned result; memcpy(&result, &value, 4); return result; }
static bool __fastcall PositionIndependent(FableUiComponentDrawView*, void*)
{
    printf(" I");
    if (mutation == 2) parent.ParentZoom.x = 3;
    return independent != 0;
}
static bool __fastcall ZoomIndependent(FableUiComponentDrawView*, void*)
{
    printf(" Q%u", zoomCalls);
    return ((zoom >> (zoomCalls++ % 2)) & 1) != 0;
}
static bool __fastcall Relative(FableUiComponentDrawView*, void*)
{
    printf(" R");
    if (mutation == 2) parent.Position.y = 29;
    return relative != 0;
}
static void Event(char kind, FableUiComponentDrawView* child, const FableUiStateVector2* value)
{
    printf(" %c%u:%u:%u:%u", kind, static_cast<unsigned>(child-children), Bits(value->x), Bits(value->y), value == &parent.Position);
    if (kind == 'Z' && mutation == 1) { parent.Position.x = 17; parent.Position.y = -23; }
    if (kind == 'Z' && mutation == 3) { nodes[0].Component = &children[3]; entries[0].Data = &children[3]; }
}
static FableUiComponentDrawView* __fastcall GetParent(FableUiComponentDrawView* child, void*)
{ printf(" G%u", static_cast<unsigned>(child-children)); return child->Parent; }
static void __fastcall SetParent(FableUiComponentDrawView* child, void*, FableUiComponentDrawView* value)
{ printf(" T%u", static_cast<unsigned>(child-children)); child->Parent=value; }
static FableUiComponentDrawView* __fastcall GetPositionParent(FableUiComponentDrawView* child, void*)
{ printf(" A%u", static_cast<unsigned>(child-children)); return child->PositionParent; }
static bool __fastcall AcceptForeign(FableUiComponentDrawView* child, void*)
{ printf(" F%u", static_cast<unsigned>(child-children)); return acceptForeign != 0; }
static void __fastcall Colour(FableUiComponentDrawView* child, void*, const FableUiStateColour* value)
{ unsigned bits; memcpy(&bits,value,4); printf(" C%u:%u", static_cast<unsigned>(child-children),bits); }
static void __fastcall Update(FableUiComponentDrawView* child, void*, float delta)
{ printf(" U%u:%u", static_cast<unsigned>(child-children),Bits(delta)); }
static void __fastcall RootZoom(FableUiComponentDrawView*,void*,float) { printf(" J"); }
static void __fastcall RootPosition(FableUiComponentDrawView*,void*,float) { printf(" K"); }
static void __fastcall RootColour(FableUiComponentDrawView*,void*,float) { printf(" L"); }
static bool __fastcall Completed(FableUiComponentDrawView* child,void*)
{ printf(" H%u",static_cast<unsigned>(child-children)); return frame && mutation==1; }
static unsigned __fastcall CurrentState(FableUiComponentDrawView* child,void*)
{ printf(" O%u",static_cast<unsigned>(child-children)); return 2; }
static void __fastcall RequestState(FableUiComponentDrawView* child,void*,unsigned state)
{ printf(" X%u:%u",static_cast<unsigned>(child-children),state); }
static FableUiDeletion* __fastcall GetDeletion(FableUiComponentDrawView* child,void*)
{ printf(" D%u",static_cast<unsigned>(child-children)); return FableUiGetDeletion(child,0); }
static void __fastcall SetDeletion(FableUiComponentDrawView* child,void*,FableUiDeletion value)
{ printf(" B%u:%u",static_cast<unsigned>(child-children),value.Method); FableUiSetDeletion(child,0,value); }
static void __fastcall Remove(FableUiComponentDrawView* p,void*,unsigned index)
{
    printf(" E%u",index);
    FableUiRemoveChildAt(p,0,index);
}
static void __fastcall Die(FableUiComponentDrawView* child,void*) { printf(" Y%u",static_cast<unsigned>(child-children)); }
static void __fastcall Position(FableUiComponentDrawView* child, void*, const FableUiStateVector2* value) { Event('P',child,value); }
static void __fastcall Zoom(FableUiComponentDrawView* child, void*, const FableUiStateVector2* value) { Event('Z',child,value); }
static void __fastcall RelativePosition(FableUiComponentDrawView* child, void*, const FableUiStateVector2* value) { Event('p',child,value); }
static void __fastcall RelativeZoom(FableUiComponentDrawView* child, void*, const FableUiStateVector2* value) { Event('z',child,value); }
int main(int argc, char** argv)
{
    if (argc < 2) return 2;
    live = argc > 2;
    retiring = live && strcmp(argv[2], "--retiring") == 0;
    frame = live && strcmp(argv[2], "--frame") == 0;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    unsigned v[24];
    while (fscanf(input, "%u", v) == 1)
    {
        for (unsigned i = 1; i < 24; ++i) if (fscanf(input, "%u", v+i) != 1) return 4;
        memset(&parent, 0, sizeof(parent)); memset(&head, 0, sizeof(head)); memset(nodes, 0, sizeof(nodes));
        FableUiComponentDrawVtable table = {};
        table.IsPositionIndependent = PositionIndependent; table.IsZoomIndependent = ZoomIndependent; table.UseRelativePosition = Relative;
        table.SetParentPosition = Position; table.SetParentZoom = Zoom;
        table.SetRelativeParentPosition = RelativePosition; table.SetRelativeParentZoom = RelativeZoom;
        table.GetParent=GetParent; table.SetParent=SetParent; table.GetPositionParent=GetPositionParent;
        table.AcceptForeignParent=AcceptForeign; table.SetParentColour=Colour; table.Update=Update;
        table.UpdateZoom=RootZoom; table.UpdatePosition=RootPosition; table.UpdateColour=RootColour;
        table.HasCompletedStateChange=Completed; table.GetDeletion=GetDeletion; table.SetDeletion=SetDeletion; table.RemoveChildAt=Remove;
        table.Die=Die;
        table.GetCurrentState=CurrentState; table.RequestState=RequestState;
        parent.Vtable = &table; parent.PositionChildren = &head;
        for (unsigned j = 0; j < 4; ++j) children[j].Vtable = &table;
        independent=v[1]; zoom=v[2]; relative=v[3]; FableUiCoordinateConversionEnabled=static_cast<unsigned char>(v[4]); mutation=v[5]; zoomCalls=0;
        memcpy(&parent.Position,v+6,8); memcpy(&parent.ParentPosition,v+8,8); memcpy(&parent.ParentZoom,v+10,8);
        memcpy(&parent.RelativeParentPosition,v+12,8); memcpy(&parent.RelativeParentZoom,v+14,8);
        memcpy(&parent.RenderZoom,v+16,8); memcpy(&parent.RelativeRenderZoom,v+18,8);
        memcpy(&FableUiCoordinateSourceExtent,v+20,8); memcpy(&FableUiCoordinateDestinationExtent,v+22,8);
        head.Left = head.Right = head.Parent = &head;
        for (unsigned n = 0; n < 3; ++n) nodes[n].Component = &children[n];
        if (v[0])
        {
            head.Left=head.Right=head.Parent=&nodes[0]; nodes[0].Parent=&head;
            if (v[0] == 2)
            {
                head.Parent=&nodes[1]; head.Right=&nodes[2]; nodes[1].Parent=&head;
                nodes[1].Left=&nodes[0]; nodes[1].Right=&nodes[2]; nodes[0].Parent=nodes[2].Parent=&nodes[1];
            }
            if (v[0] == 3)
            {
                head.Right=&nodes[2]; nodes[0].Right=&nodes[1]; nodes[1].Right=&nodes[2];
                nodes[1].Parent=&nodes[0]; nodes[2].Parent=&nodes[1];
            }
            if (v[0] == 4)
            {
                head.Left=&nodes[2]; nodes[0].Left=&nodes[1]; nodes[1].Left=&nodes[2];
                nodes[1].Parent=&nodes[0]; nodes[2].Parent=&nodes[1];
            }
        }
        if (live)
        {
            acceptForeign=(v[0] >> 3) & 1;
            entries[0].Data=&children[0]; parent.Children.Begin=entries; parent.Children.End=parent.Children.CapacityEnd=entries+1;
            parent.ChildrenToDelete = parent.Children;
            unsigned colour=0x12345678; memcpy(&parent.RenderColour,&colour,4);
            for (unsigned c=0;c<4;++c)
            {
                children[c].Parent=(v[0]&3)==0 ? 0 : ((v[0]&3)==1 ? &parent : &children[c]);
                children[c].PositionParent=(v[0]&4) ? &parent : ((v[0]&3)==3 ? &children[c] : 0);
            }
        }
        if (frame)
        {
            recording=false; head.Left=&head;
            for (unsigned f=0;f<4;++f)
            {
                entries[f].Data=&children[f]; entries[f].Info=0;
                children[f].Deletion.Method=(v[0]>>4)&3;
                children[f].Deletion.AssociatedParents=FableUiCreateDeletionList();
                if (children[f].Deletion.Method==3) FableUiAppendDeletionParent(children[f].Deletion.AssociatedParents,&parent);
            }
            parent.Children.End=entries+3; parent.Children.CapacityEnd=entries+4;
            retiringEntries[0]=entries[3]; parent.ChildrenToDelete.Begin=retiringEntries;
            parent.ChildrenToDelete.End=retiringEntries+1; parent.ChildrenToDelete.CapacityEnd=retiringEntries+8;
        }
        printf("TRACE");
        recording=frame;
        if (frame) FableUiBaseComponentUpdate(&parent,0,0.25f);
        else if (retiring) FableUiPrepareAndUpdateRetiringChild(&parent,0,0.25f);
        else if (live) FableUiPrepareAndUpdateLiveChild(&parent,0,0.25f);
        else FableUiUpdatePositionChildren(&parent);
        printf(" END");
        if (frame)
        {
            printf(" %u:%u:%u",parent.Children.Size(),parent.ChildrenToDelete.Size(),Bits(parent.Time));
            for (unsigned a=0;a<parent.Children.Size();++a) printf(" V%u",static_cast<unsigned>(parent.Children.Begin[a].Data-children));
            for (unsigned b=0;b<parent.ChildrenToDelete.Size();++b) printf(" W%u",static_cast<unsigned>(parent.ChildrenToDelete.Begin[b].Data-children));
            recording=false;
            for (unsigned d=0;d<4;++d) FableUiDestroyDeletionParents(&children[d].Deletion.AssociatedParents,0);
        }
        printf("\n");
    }
    fclose(input); return 0;
}
