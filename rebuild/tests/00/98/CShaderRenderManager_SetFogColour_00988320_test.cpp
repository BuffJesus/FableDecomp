#include <cstdio>

struct Colour { float r; float g; float b; };
struct CShaderRenderManager {
    char pad[0x380];
    Colour fogColour;
    char pad2[0x3d8 - 0x380 - 12];
    unsigned int flags;
};

void __fastcall CShaderRenderManager_SetFogColour(CShaderRenderManager* self, void* /*edx*/, const Colour* c)
{
    *(Colour*)((char*)self + 0x380) = *c;
    self->flags |= 0x8u;
}

int main()
{
    CShaderRenderManager m;
    m.fogColour.r = 0.0f; m.fogColour.g = 0.0f; m.fogColour.b = 0.0f;
    m.flags = 0x1;
    Colour c; c.r = 1.5f; c.g = 2.5f; c.b = 3.5f;
    CShaderRenderManager_SetFogColour(&m, 0, &c);
    if (m.fogColour.r == 1.5f && m.fogColour.g == 2.5f && m.fogColour.b == 3.5f && m.flags == 0x9u) {
        std::printf("CShaderRenderManager_00988320_TEST PASS\n");
        return 0;
    }
    std::printf("CShaderRenderManager_00988320_TEST FAIL\n");
    return 1;
}