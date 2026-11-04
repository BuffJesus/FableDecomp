#include "fable_volume_types.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
void CVolumeTexture::Uninitialise()
{
    if (PD3DVolumeTexture)
    {
        PD3DVolumeTexture->AddRef();
        if (PD3DVolumeTexture->Release() == 1)
        {
            CSystemManager* system = GFGetSystemManager();
            system->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(PD3DVolumeTexture, 1.0f);
        }
        ReleaseTexture();
        PData = 0;
        PD3DVolumeTexture = 0;
        ByteLength = 0;
    }
}
