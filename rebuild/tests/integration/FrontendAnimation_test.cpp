#include "fable_frontend_animation.h"
#include <stdio.h>
#include <stdlib.h>

static void Require(bool condition, const char* message)
{
    if (!condition) { printf("FRONTEND_ANIMATION FAIL %s\n", message); exit(1); }
}
int main()
{
    const float durations[] = {8, 8, 8, 2};
    const float sunbeams[] = {2, 2, 2};
    FableFrontendAnimation background, sunbeam;
    background.Initialise(4, durations);
    sunbeam.Initialise(3, sunbeams);
    background.Update(0);
    sunbeam.Update(0);
    Require(background.Swap.TargetState == 3 && sunbeam.Swap.TargetState == 2,
        "independent retail initial targets");
    Require(background.Children[0].RenderColour.alpha == 255 &&
        background.Children[3].RenderColour.alpha == 0, "first update retains starting colours");
    background.Update(0.25f);
    sunbeam.Update(0.25f);
    Require(background.Children[0].RenderColour.alpha == 255 &&
        background.Children[3].RenderColour.alpha == 0 && !background.PendingTransition,
        "queued fade starts after the next colour update");
    background.Update(1.25f);
    Require(background.Children[3].RenderColour.alpha == 191 &&
        background.Children[0].RenderColour.alpha == 195, "independent child fade durations");
    background.Update(2.25f);
    Require(background.Swap.CurrentState == 0 && background.Swap.TargetState == 3 &&
        background.Children[3].RenderColour.alpha == 255, "short incoming fade does not finish parent");
    fable_u32 seed = background.Swap.Seed;
    background.Update(8.25f);
    Require(background.Swap.CurrentState == 3 && background.Swap.TargetState == 3 &&
        background.Swap.Seed == seed && background.Swap.LastSwapTime == 8.25f,
        "completion notification defers next random draw");
    background.Update(8.375f);
    Require(background.Swap.CurrentState == 3 && background.Swap.TargetState == 2 &&
        background.Children[3].RenderColour.alpha == 255 &&
        background.Children[2].RenderColour.alpha == 0, "next update requests next state");
    seed = background.Swap.Seed;
    background.Update(8.5f);
    background.Update(500);
    Require(background.Swap.CurrentState == 2 && background.Swap.TargetState == 2 &&
        background.Swap.Seed == seed, "stall completes only the active transition");
    background.Update(500.125f);
    Require(background.Swap.TargetState == 1, "no skipped random targets after stall");
    sunbeam.Update(2.25f);
    Require(sunbeam.Swap.CurrentState == 2 && sunbeam.Swap.TargetState == 2,
        "sunbeam completion independent of background updates");
    sunbeam.Update(2.375f);
    Require(sunbeam.Swap.TargetState == 0, "sunbeam retains its own seed sequence");
    background.Initialise(4, durations);
    Require(background.Swap.Seed == 13 && background.Swap.CurrentState == 0 &&
        background.Swap.TargetState == 0 && background.Children[0].Colour.alpha == 255 && !background.PendingTransition,
        "reinitialization clears prior transition");
    puts("FABLETLC_FRONTEND_ANIMATION PASS");
    return 0;
}
