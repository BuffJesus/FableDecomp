// Shared PDB-backed system/display ABI for retail coordinate helpers.
#include <math.h>
#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"
#include "fable_virtual_pixels.h"

float FABLE_FASTCALL GFRoundVXToNearestPixel(double x)
{
    CDisplayManager* displayManager =
        GFGetSystemManager()->GetDisplayManager();
    const double onePixel =
        g_FableVirtualWidth_013961E8 /
        displayManager->GetRenderTargetDimensions().X;
    const fable_i32 pixel =
        static_cast<fable_i32>(floor(x / onePixel));
    return static_cast<float>(pixel * onePixel);
}

float FABLE_FASTCALL GFRoundVYToNearestPixel(double y)
{
    CDisplayManager* displayManager =
        GFGetSystemManager()->GetDisplayManager();
    const double onePixel =
        g_FableVirtualHeight_013961F0 /
        displayManager->GetRenderTargetDimensions().Y;
    const fable_i32 pixel =
        static_cast<fable_i32>(floor(y / onePixel));
    return static_cast<float>(pixel * onePixel);
}

float FABLE_FASTCALL GFAXToVX(float x)
{
    CDisplayManager* displayManager =
        GFGetSystemManager()->GetDisplayManager();
    // Keep the native load of the shared double before the float multiply.
    // A volatile access preserves the previous helper without redeclaring the
    // underlying storage with a conflicting type.
    return static_cast<float>(
        (static_cast<volatile double&>(g_FableVirtualWidth_013961E8) * x) /
        displayManager->GetRenderTargetDimensions().X);
}

