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