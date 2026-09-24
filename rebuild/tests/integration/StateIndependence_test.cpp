#include "fable_ui_state_progress.h"
#include <stdio.h>
#include <string.h>
static unsigned independent, present, mutation;
static CUIStateRecoveredLayout state;
static bool __fastcall Independent(FableUiComponentDrawView* view, void*)
{
    printf(" I");
    if (mutation == 1) static_cast<FableUiStateProgressView*>(view)->TargetState = 9;
    return independent != 0;
}
static CUIStateRecoveredLayout* __fastcall Find(FableUiComponentDrawView* view, void*, unsigned requested)
{
    printf(" F%u", requested);
    if (mutation == 2) { independent = !independent; state.stateChangeFlag ^= 0x60; }
    return present ? &state : 0;
}
int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    unsigned method, flags;
    while (fscanf(input, "%u %u %u %u %u", &method, &independent, &flags, &present, &mutation) == 5)
    {
        FableUiStateProgressView component; memset(&component, 0, sizeof(component));
        FableUiComponentDrawVtable table = {}; table.IsIndependent = Independent; table.FindStateForQuery = Find;
        component.Vtable = &table; component.TargetState = 3; state.stateChangeFlag = flags;
        printf("TRACE");
        bool result = method ? FableUiStateZoomIndependent(&component, 0) : FableUiStatePositionIndependent(&component, 0);
        printf(" END %u %u\n", result, component.TargetState);
    }
    fclose(input); return 0;
}
