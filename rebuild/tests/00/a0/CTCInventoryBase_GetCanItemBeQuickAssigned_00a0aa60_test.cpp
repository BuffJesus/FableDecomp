#include <stdio.h>
#include <string.h>

#include "fable_render_state_manager.h"

int main()
{
    // CRenderManagerCore has no default constructor (retail builds it from
    // its CDisplayManager); the getter only reads Rendering (+0x3A54).
    static fable_u8 rendererStorage[sizeof(CRenderManagerCore)];
    memset(rendererStorage, 0, sizeof(rendererStorage));
    CRenderManagerCore& renderer =
        *reinterpret_cast<CRenderManagerCore*>(rendererStorage);

    if (offsetof(CRenderManagerCore, Rendering) != 0x3A54)
    {
        printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    renderer.Rendering = true;
    if (!renderer.IsRendering())
    {
        printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    renderer.Rendering = false;
    if (renderer.IsRendering())
    {
        printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_INVENTORY_QUICK_ASSIGN_BEHAVIOR PASS\n");
    return 0;
}
