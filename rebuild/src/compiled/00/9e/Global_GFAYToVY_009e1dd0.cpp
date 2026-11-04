#include "fable_display_viewport.h"
#include "fable_gfuninitialise.h"
#include "fable_virtual_pixels.h"
float FABLE_FASTCALL GFAYToVY(float y)
{
    CSystemManager* system = GFGetSystemManager();
    double h = g_FableVirtualHeight_013961F0;
    y = (float)(h * y);
    return y / system->GetDisplayManager()->GetRenderTargetDimensions().Y;
}
