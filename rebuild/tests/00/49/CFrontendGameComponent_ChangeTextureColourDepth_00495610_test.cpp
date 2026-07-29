#include <cstdio>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ChangeTextureColourDepth_00495610.cpp"

static unsigned long g_InitGraphicsCalls;
static CFrontendGameComponent_00495610* g_InitGraphicsThis;

bool CFrontendGameComponent_00495610::InitGraphics()
{
    ++g_InitGraphicsCalls;
    g_InitGraphicsThis = this;
    return false;
}

int main()
{
    CFrontendGameComponent_00495610 component;

    component.ChangeTextureColourDepth(16);
    component.ChangeTextureColourDepth(32);

    if (g_InitGraphicsCalls != 2 || g_InitGraphicsThis != &component)
    {
        std::printf("CFrontendGameComponent_ChangeTextureColourDepth_00495610_test FAIL\n");
        return 1;
    }

    std::printf("CFrontendGameComponent_ChangeTextureColourDepth_00495610_test PASS\n");
    return 0;
}
