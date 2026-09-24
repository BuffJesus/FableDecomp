#include "fable_ui_state_progress.h"
#include <stdio.h>
#include <string.h>

static CUIStateRecoveredLayout target;
static unsigned mode;
static CUIStateRecoveredLayout* __fastcall Find(FableUiComponentDrawView*, void*, unsigned state)
{ printf(" F%u", state); return &target; }
static void VectorEvent(char kind, FableUiComponentDrawView* view, const FableUiStateVector2* delta, float time, bool linear)
{
    unsigned x, y, t; memcpy(&x, &delta->x, 4); memcpy(&y, &delta->y, 4); memcpy(&t, &time, 4);
    printf(" %c%u:%u:%u:%u", kind, x, y, t, linear);
    if (mode == 1 && kind == 'P') { view->Zoom = target.zoom; view->Colour = target.colour; }
    if (mode == 2 && kind == 'P') { target.stateChangeFlag = 2; target.linearChange = 1; }
}
static void __fastcall Position(FableUiComponentDrawView* view, void*, const FableUiStateVector2* delta, float time, bool linear)
{ VectorEvent('P', view, delta, time, linear); }
static void __fastcall Zoom(FableUiComponentDrawView* view, void*, const FableUiStateVector2* delta, float time, bool linear)
{ VectorEvent('Z', view, delta, time, linear); }
static void __fastcall Colour(FableUiComponentDrawView*, void*, const FableUiStateColour* delta, float time, bool linear)
{
    unsigned colour, t; memcpy(&colour, delta, 4); memcpy(&t, &time, 4);
    printf(" C%u:%u:%u", colour, t, linear);
}
int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    unsigned values[16];
    while (fscanf(input, "%u", &values[0]) == 1)
    {
        for (unsigned i = 1; i < 16; ++i) if (fscanf(input, "%u", &values[i]) != 1) return 4;
        FableUiStateProgressView component; memset(&component, 0, sizeof(component));
        FableUiComponentDrawVtable table = {};
        table.FindState = Find; table.ChangePositionDelta = Position; table.ChangeZoomDelta = Zoom; table.ChangeColour = Colour;
        component.Vtable = &table; component.CurrentState = values[0]; component.TargetState = values[1];
        target.stateChangeFlag = values[2]; target.linearChange = static_cast<unsigned char>(values[3]);
        memcpy(&component.Position, &values[4], 8); memcpy(&target.position, &values[6], 8);
        memcpy(&component.Zoom, &values[8], 8); memcpy(&target.zoom, &values[10], 8);
        memcpy(&component.Colour, &values[12], 4); memcpy(&target.colour, &values[13], 4);
        memcpy(&component.UpdateTime, &values[14], 4); mode = values[15];
        printf("TRACE"); FableUiProcessChangeState(&component, 0); printf(" END\n");
    }
    fclose(input); return 0;
}
