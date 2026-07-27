#include "fable_video_system.h"

#include <stdio.h>
#include <string.h>

int main()
{
    CVideoSysRecoveredLayout videoSystem;
    CTextureRendererRecoveredLayout renderer;
    memset(&videoSystem, 0, sizeof(videoSystem));
    memset(&renderer, 0, sizeof(renderer));

    void* const expectedTexture =
        reinterpret_cast<void*>(0x12345678);
    videoSystem.textureRenderer20 = &renderer;
    renderer.texture164 = expectedTexture;

    if (videoSystem.GetTexture() != expectedTexture)
        return 1;

    renderer.texture164 = 0;
    if (videoSystem.GetTexture() != 0)
        return 2;

    printf("FABLETLC_CVIDEOSYS_GET_TEXTURE PASS\n");
    return 0;
}
