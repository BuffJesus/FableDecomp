#pragma once
#include "fable_ui_component_draw.h"

// Semantic global names; retail stores these at 013B8768, 01375CD4/8,
// and 013B876C/70 respectively. Integration must supply these bindings.
extern unsigned char FableUiCoordinateConversionEnabled;
extern FableUiStateVector2 FableUiCoordinateSourceExtent;
extern FableUiStateVector2 FableUiCoordinateDestinationExtent;

FableUiStateVector2* __fastcall FableUiConvertCoordinates(FableUiStateVector2* output, void*, FableUiStateVector2 value);
void __fastcall FableUiUpdatePosition(FableUiComponentDrawView*, void*, float delta);
void __fastcall FableUiUpdateZoom(FableUiComponentDrawView*, void*, float delta);
FableUiStateVector2* __fastcall FableUiConvertCoordinatesInverse(FableUiStateVector2* output, void*, FableUiStateVector2 value);
void __fastcall FableUiChangePosition(FableUiComponentDrawView*, void*, const FableUiStateVector2*, float, bool);
void __fastcall FableUiChangePositionDelta(FableUiComponentDrawView*, void*, const FableUiStateVector2*, float, bool);
void __fastcall FableUiChangeZoom(FableUiComponentDrawView*, void*, const FableUiStateVector2*, float, bool);
void __fastcall FableUiChangeZoomDelta(FableUiComponentDrawView*, void*, const FableUiStateVector2*, float, bool);

// Context presence at 013B86A0 gates CManager::GetUIScale. Its concrete owner
// remains unrecovered; this function only tests it for null.
extern void* FableUiScaleContext;
float __cdecl FableUiGetCoordinateWidth();
float __cdecl FableUiGetCoordinateHeight();
// Recovered singleton accessor 0041E5F2; allocation/construction remain services.
void* __cdecl FableUiGetManager();
FableUiStateVector2* __fastcall FableUiGetManagerScale(void* manager, void*, FableUiStateVector2* output);

inline float FableUiVectorAxis(float initial, float target, float duration, double elapsed, float roundedElapsed)
{
    const double difference = static_cast<double>(target) - initial;
    // These stores are present in retail; volatile prevents VC7.1 from keeping
    // all intermediates at extended precision and changing cancellation results.
    const volatile float acceleration = static_cast<float>((difference * -2.0) / (static_cast<double>(duration) * duration));
    const volatile float velocity = static_cast<float>((difference * 2.0) / duration);
    const volatile float travel = static_cast<float>(velocity * elapsed);
    const float halfAcceleration = acceleration * 0.5f;
    return static_cast<float>(halfAcceleration * (static_cast<double>(roundedElapsed) * roundedElapsed) + travel + initial);
}

// Shared ease-out motion in UpdatePosition and UpdateZoom. Elapsed time is
// stored as float, but retail retains the extended sum for endpoint comparison.
inline void FableUiAdvanceVector(FableUiStateVector2& value, FableUiStateVector2& initial,
    const FableUiStateVector2& target, float& elapsed, float duration, float delta)
{
    if (elapsed < duration)
    {
        const double advanced = static_cast<double>(elapsed) + delta;
        elapsed = static_cast<float>(advanced);
        value.x = FableUiVectorAxis(initial.x, target.x, duration, advanced, elapsed);
        value.y = FableUiVectorAxis(initial.y, target.y, duration, advanced, elapsed);
        if (advanced >= duration) value = initial = target;
    }
}
