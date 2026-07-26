#include <stdio.h>
#include <string.h>

#include "fable_shader_render_manager.h"

int main()
{
    CShaderRenderManager manager;
    memset(&manager, 0xA5, sizeof(manager));

    manager.updateFlags3D8 = 0;
    manager.OnPostViewportChanged();
    if (manager.updateFlags3D8 != 0x10000)
    {
        return 1;
    }

    manager.updateFlags3D8 = 0xA55A0201;
    manager.OnPostViewportChanged();
    if (manager.updateFlags3D8 != 0xA55B0201)
    {
        return 2;
    }

    printf("FABLETLC_POST_VIEWPORT_CHANGED_BEHAVIOR PASS\n");
    return 0;
}
