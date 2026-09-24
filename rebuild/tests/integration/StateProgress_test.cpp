#include "fable_ui_state_progress.h"
#include <stdio.h>
#include <string.h>

static unsigned sequence, calls;
static bool __fastcall Query(FableUiComponentDrawView* component, void*)
{
    ++calls;
    if (sequence < 4) return ((sequence >> (calls - 1)) & 1) != 0;
    return FableUiHasCompletedStateChange(component, 0);
}

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r");
    if (!input) return 3;
    unsigned pending, done, queued, before;
    while (fscanf(input, "%u %u %u %u %u", &pending, &done, &queued, &before, &sequence) == 5)
    {
        FableUiStateProgressView component;
        memset(&component, 0, sizeof(component));
        FableUiComponentDrawVtable table = {};
        FableUiStateTaskNode head = {}, task = {};
        table.HasCompletedStateChange = Query;
        component.Vtable = &table;
        component.StatesBeingDone = pending;
        component.StatesDone = done;
        component.StatesToDo = &head;
        component.PreviousUpdateChanged = static_cast<unsigned char>(before);
        head.Next = queued ? &task : &head;
        calls = 0;
        bool complete = FableUiHasCompletedStateChange(&component, 0);
        bool changed = FableUiChangedStateLastUpdate(&component, 0);
        printf("%u %u %u\n", complete, changed, calls);
    }
    fclose(input);
    return 0;
}
