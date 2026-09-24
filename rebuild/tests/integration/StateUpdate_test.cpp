#include "fable_ui_state_progress.h"
#include <stdio.h>
#include <string.h>

static FableUiStateProgressView parent, children[3];
static CUIStateRecoveredLayout state;
static unsigned present, complete, mode, childMask;
static CUIStateRecoveredLayout* __fastcall Find(FableUiComponentDrawView*, void*, unsigned target)
{ printf(" F%u", target); return present ? &state : 0; }
static bool __fastcall Complete(FableUiComponentDrawView* component, void*)
{
    if (component == &parent) { printf(" Q"); return complete != 0; }
    unsigned index = static_cast<FableUiStateProgressView*>(component) - children;
    printf(" Q%u", index);
    if (mode == 3) parent.Children.End = parent.Children.Begin;
    return (childMask & (1 << index)) != 0;
}
static void __fastcall Advance(FableUiComponentDrawView*, void*) { printf(" A"); }
void __fastcall FableUiBaseComponentUpdate(FableUiComponentDrawView* component, void*, float delta)
{
    unsigned bits; memcpy(&bits, &delta, 4); printf(" B%u", bits);
    if (mode == 1) parent.PreviousUpdateChanged = 0;
    if (mode == 2) parent.PreviousUpdateChanged = 1;
}
int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    unsigned value[14];
    while (fscanf(input, "%u", &value[0]) == 1)
    {
        for (unsigned i = 1; i < 14; ++i) if (fscanf(input, "%u", &value[i]) != 1) return 4;
        memset(&parent, 0, sizeof(parent)); memset(children, 0, sizeof(children));
        FableUiComponentDrawVtable table = {}; table.FindState = Find;
        table.HasCompletedStateChange = Complete; table.UpdateStateChange = Advance;
        parent.Vtable = &table; parent.TargetState = 5;
        present = value[1]; complete = value[2]; state.stateChangeFlag = value[3]; mode = value[4];
        memcpy(&parent.PositionTimeElapsed, &value[5], 24);
        parent.ParentColour.red = 1; parent.ParentColour.green = 2;
        parent.ParentColour.blue = 3; parent.ParentColour.alpha = 4;
        parent.ParentPosition.x = 2.0f; parent.ParentPosition.y = 3.0f;
        parent.ParentZoom.x = 4.0f; parent.ParentZoom.y = 5.0f;
        FableUiComponentCountedStorage entries[3] = {};
        for (unsigned i = 0; i < 3; ++i) { children[i].Vtable = &table; entries[i].Data = &children[i]; }
        parent.Children.Begin = entries; parent.Children.End = entries+value[11]; parent.Children.CapacityEnd = entries+3;
        childMask = value[12]; float delta; memcpy(&delta, &value[13], 4);
        unsigned result = 0; printf("TRACE");
        if (value[0] == 0) FableUiChangingStateUpdate(&parent, 0, delta);
        else if (value[0] == 1) result = FableUiInternalChanged(&parent, 0);
        else result = FableUiChildrenChanged(&parent, 0);
        unsigned px, py, zx, zy, colour;
        memcpy(&px, &parent.ParentPosition.x, 4); memcpy(&py, &parent.ParentPosition.y, 4);
        memcpy(&zx, &parent.ParentZoom.x, 4); memcpy(&zy, &parent.ParentZoom.y, 4);
        memcpy(&colour, &parent.ParentColour, 4);
        printf(" END %u %u %u %u %u %u %u %u\n", result, parent.PreviousUpdateChanged,
            px, py, zx, zy, colour, parent.Children.Size());
    }
    fclose(input); return 0;
}
