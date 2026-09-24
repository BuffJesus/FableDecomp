#pragma once

#include "fable_ui_colour.h"
#include "fable_ui_swapping.h"

// Bridge from the flat visual checkpoint to the recovered colour/swap rules.
// This still owns no native component tree or rendering order.
// Its timing is checked against retail's full 29/22-node coastal hierarchies;
// it is intentionally specific to their colour-only, simultaneous state modes.
struct FableFrontendAnimation
{
    FableUiSwappingState Swap;
    FableUiColourState Children[4];
    FableUiSwapEntry States[4];
    float Durations[4];
    unsigned Count;
    float LastUpdateTime;
    bool PendingTransition;

    void Initialise(unsigned count, const float* durations)
    {
        Count = count;
        LastUpdateTime = 0.0f;
        PendingTransition = false;
        Swap.CurrentState = Swap.TargetState = 0;
        Swap.Initialise(0.0f);
        const FableUiStateColour white = {255, 255, 255, 255};
        for (unsigned i = 0; i < count; ++i)
        {
            States[i].State = i;
            States[i].Dwell = 0.0f;
            Durations[i] = durations[i];
            Children[i].Colour = white;
            Children[i].Colour.alpha = i == 0 ? 255 : 0;
            Children[i].InitialColour = Children[i].TargetColour = Children[i].Colour;
            Children[i].RenderColour = Children[i].Colour;
            Children[i].ParentColour = white;
            Children[i].ColourTimeElapsed = Children[i].ColourTime = 0.0f;
        }
    }

    void Update(float time)
    {
        const float delta = time - LastUpdateTime;
        LastUpdateTime = time;
        bool complete = true;
        for (unsigned i = 0; i < Count; ++i)
        {
            Children[i].Update(delta, false);
            if (Children[i].ColourTimeElapsed < Children[i].ColourTime)
                complete = false;
        }
        // ChangeState queues work. Each child first advances its old colour,
        // then ProcessChangeState starts the queued fade on the next update.
        const bool starting = PendingTransition;
        if (starting)
        {
            PendingTransition = false;
            for (unsigned i = 0; i < Count; ++i)
            {
                FableUiStateColour target = {255, 255, 255, 0};
                target.alpha = Swap.TargetState == i ? 255 : 0;
                if (Children[i].Colour.alpha != target.alpha)
                    Children[i].SetTarget(target, Durations[i]);
            }
        }
        const bool changed = !starting && complete && Swap.CurrentState != Swap.TargetState;
        if (changed) Swap.CurrentState = Swap.TargetState;
        fable_u32 requested;
        if (Swap.SelectAfterUpdate(time, changed, true, States, Count, requested))
        {
            Swap.TargetState = requested;
            PendingTransition = true;
        }
    }
};
