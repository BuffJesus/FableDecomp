#include "fable_ui_colour.h"
#include <stdio.h>
#include <string.h>

static FableUiStateColour Unpack(unsigned value)
{
    FableUiStateColour result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned Pack(const FableUiStateColour& value)
{
    unsigned result;
    memcpy(&result, &value, 4);
    return result;
}
int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r");
    if (!input) return 3;
    unsigned mode, current, target, initial, parent, render, independent, argument;
    float elapsed, duration, time;
    while (fscanf(input, "%u %u %u %u %u %u %f %f %u %u %f",
        &mode, &current, &target, &initial, &parent, &render, &elapsed, &duration,
        &independent, &argument, &time) == 11)
    {
        FableUiColourState state;
        state.Colour = Unpack(current);
        state.TargetColour = Unpack(target);
        state.InitialColour = Unpack(initial);
        state.ParentColour = Unpack(parent);
        state.RenderColour = Unpack(render);
        state.ColourTimeElapsed = elapsed;
        state.ColourTime = duration;
        if (mode == 0) state.SetTarget(Unpack(argument), time);
        else state.Update(time, independent != 0);
        printf("%u %u %u %u %.9g %.9g\n", Pack(state.Colour), Pack(state.TargetColour),
            Pack(state.InitialColour), Pack(state.RenderColour),
            state.ColourTimeElapsed, state.ColourTime);
    }
    fclose(input);
    return 0;
}
