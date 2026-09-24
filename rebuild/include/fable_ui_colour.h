#pragma once

#include "fable_ui_state.h"

// Semantic extraction of CComponent's colour state, not its complete ABI or
// ownership model. PDB names agree with retail fields 84/88/8C/90/94/A8/AC.
struct FableUiColourState
{
    FableUiStateColour Colour, TargetColour, InitialColour, ParentColour, RenderColour;
    float ColourTimeElapsed, ColourTime;

    void SetTarget(const FableUiStateColour& target, float duration)
    {
        ColourTime = duration;
        TargetColour = target;
        if (duration > 0.0f)
        {
            InitialColour = Colour;
            ColourTimeElapsed = 0.0f;
        }
        else
        {
            Colour = InitialColour = target;
            // Retail 0052ECA0 leaves the elapsed field untouched here.
        }
    }

    static fable_u8 Interpolate(fable_u8 start, fable_u8 target, float weight)
    {
        float value = start + (static_cast<float>(target) - start) * weight;
        if (value < 0.0f) value = 0.0f;
        if (value > 255.0f) value = 255.0f;
        return static_cast<fable_u8>(value);
    }

    static fable_u8 Modulate(fable_u8 local, fable_u8 parent)
    {
        const float reciprocal = 1.0f / 255.0f;
        const float product = (local * reciprocal) * (parent * reciprocal);
        return static_cast<fable_u8>(product * 255.0f);
    }

    void Update(float delta, bool independent)
    {
        if (ColourTimeElapsed < ColourTime)
        {
            ColourTimeElapsed += delta;
            if (ColourTimeElapsed >= ColourTime)
                Colour = InitialColour = TargetColour;
            else
            {
                const float phase = ColourTimeElapsed / ColourTime;
                const float weight = phase * (2.0f - phase);
                Colour.red = Interpolate(InitialColour.red, TargetColour.red, weight);
                Colour.green = Interpolate(InitialColour.green, TargetColour.green, weight);
                Colour.blue = Interpolate(InitialColour.blue, TargetColour.blue, weight);
                Colour.alpha = Interpolate(InitialColour.alpha, TargetColour.alpha, weight);
            }
        }
        if (independent)
            RenderColour = Colour;
        else
        {
            RenderColour.red = Modulate(Colour.red, ParentColour.red);
            RenderColour.green = Modulate(Colour.green, ParentColour.green);
            RenderColour.blue = Modulate(Colour.blue, ParentColour.blue);
            RenderColour.alpha = Modulate(Colour.alpha, ParentColour.alpha);
        }
    }
};
