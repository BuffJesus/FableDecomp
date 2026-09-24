#include "fable_frontend_animation.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    const float backgroundDurations[] = {8, 8, 8, 2};
    const float beamDurations[] = {2, 2, 2};
    FableFrontendAnimation background, beam;
    background.Initialise(4, backgroundDurations); beam.Initialise(3, beamDurations);
    float time;
    while (fscanf(input, "%f", &time) == 1)
    {
        FableFrontendAnimation* animations[] = {&background, &beam};
        for (unsigned group = 0; group < 2; ++group)
        {
            FableFrontendAnimation& animation = *animations[group]; animation.Update(time);
            unsigned swapBits; memcpy(&swapBits, &animation.Swap.LastSwapTime, 4);
            printf("%u %u %u %u", animation.Swap.CurrentState, animation.Swap.TargetState, animation.Swap.Seed, swapBits);
            for (unsigned child = 0; child < animation.Count; ++child) printf(" %u", animation.Children[child].RenderColour.alpha);
            printf("\n");
        }
    }
    fclose(input); return 0;
}
