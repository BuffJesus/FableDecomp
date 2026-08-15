// CShaderRenderManager::OnPostViewportChanged @ 0x00c3d460
// or dword ptr [ecx+0xa4], 0x80000000 ; ret

struct CShaderRenderManager {
    char pad[0xa4];
    unsigned int flags;   // +0xa4
};

void __fastcall OnPostViewportChanged(CShaderRenderManager* self)
{
    self->flags |= 0x80000000u;
}