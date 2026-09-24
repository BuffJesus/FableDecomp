#pragma once

#include "fable_frontend_random.h"

struct FableUiSwapEntry
{
    fable_u32 State;
    float Dwell;
};

// Decision portion of CSwappingStateComponent::Update (0054738E..005474B7).
// The caller must first run the changing-state base update, then supply its
// completion notification. This does not replace native child ownership.
struct FableUiSwappingState
{
    fable_u32 CurrentState, TargetState, Seed;
    float LastSwapTime;

    void Initialise(float time)
    {
        LastSwapTime = time;
        Seed = 13;
    }

    bool SelectAfterUpdate(float time, bool changedStateLastUpdate, bool random,
        const FableUiSwapEntry* states, unsigned count, fable_u32& requested)
    {
        unsigned index;
        if (changedStateLastUpdate)
        {
            for (index = 0; index < count; ++index)
                if (states[index].State == CurrentState)
                {
                    LastSwapTime = time;
                    break;
                }
            return false;
        }
        if (CurrentState != TargetState) return false;
        for (index = 0; index < count; ++index)
            if (states[index].State == TargetState) break;
        if (index == count || !(time - LastSwapTime >= states[index].Dwell))
            return false;
        // Retail passes the state ID as the excluded random INDEX. Do not
        // silently change that behavior for non-contiguous state tables.
        unsigned next = random ? FableFrontendRandomFrame(count, TargetState, Seed)
            : (index + 1 < count ? index + 1 : 0);
        requested = states[next].State;
        LastSwapTime = time;
        return true;
    }
};
