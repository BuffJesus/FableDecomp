#include "fable_render_texture.h"

void CRenderManagerCoreAttachTextureView::AttachTextureToStage(
    fable_i32 stage,
    const CTextureAttachView* texture)
{
    void* deviceTexture =
        texture != 0
            ? texture->deviceTexture00
            : 0;
    if (attachedTextures3D00[stage] != deviceTexture)
    {
        attachedTextures3D00[stage] = deviceTexture;
        reinterpret_cast<FableSetTextureCall>(
            displayDevice3CF0->vtable[0x41])(
                displayDevice3CF0,
                stage,
                deviceTexture);
    }
    if (activeTextureStageCount3CB8 < stage + 1)
    {
        activeTextureStageCount3CB8 = stage + 1;
    }
}
