#include <cstdio>

struct CShaderRenderManager {
    char pad[0xa4];
    unsigned int flags;   // +0xa4
};

void __fastcall OnPostViewportChanged(CShaderRenderManager* self)
{
    self->flags |= 0x80000000u;
}

int main()
{
    CShaderRenderManager m;
    m.flags = 0;
    OnPostViewportChanged(&m);
    bool ok = (m.flags == 0x80000000u);

    m.flags = 0x1;
    OnPostViewportChanged(&m);
    ok = ok && (m.flags == 0x80000001u);

    // idempotent OR
    OnPostViewportChanged(&m);
    ok = ok && (m.flags == 0x80000001u);

    if (ok) printf("PARITY_OK\n");
    else printf("PARITY_FAIL\n");
    return 0;
}