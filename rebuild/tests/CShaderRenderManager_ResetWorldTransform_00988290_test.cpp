#include <cstdio>

struct CShaderRenderManager {
    char pad_0[0x1e8];
    unsigned char inited;            // 0x1e8
    char pad_1e9[0x1f0 - 0x1e9];
    float m[16];                     // 0x1f0 .. 0x22c
    char pad_230[0x3d8 - 0x230];
    unsigned int dirtyFlags;         // 0x3d8
};

__declspec(naked) void __fastcall CShaderRenderManager_ResetWorldTransform(CShaderRenderManager* self)
{
    __asm {
        mov  dl, byte ptr [ecx+0x1e8]
        xor  eax, eax
        cmp  dl, al
        jne  skip
        mov  edx, 0x3f800000
        mov  dword ptr [ecx+0x1f4], eax
        mov  dword ptr [ecx+0x1f8], eax
        mov  dword ptr [ecx+0x1fc], eax
        mov  dword ptr [ecx+0x200], eax
        mov  dword ptr [ecx+0x208], eax
        mov  dword ptr [ecx+0x20c], eax
        mov  dword ptr [ecx+0x210], eax
        mov  dword ptr [ecx+0x214], eax
        mov  dword ptr [ecx+0x21c], eax
        mov  dword ptr [ecx+0x220], eax
        mov  dword ptr [ecx+0x224], eax
        mov  dword ptr [ecx+0x228], eax
        mov  dword ptr [ecx+0x1f0], edx
        mov  dword ptr [ecx+0x204], edx
        mov  dword ptr [ecx+0x218], edx
        mov  dword ptr [ecx+0x22c], edx
        or   dword ptr [ecx+0x3d8], 0xc990
        mov  byte  ptr [ecx+0x1e8], 1
      skip:
        ret
    }
}

static void zero(CShaderRenderManager* o) {
    for (int i = 0; i < (int)sizeof(*o); ++i) ((char*)o)[i] = 0;
}

int main()
{
    CShaderRenderManager a;
    zero(&a);
    CShaderRenderManager_ResetWorldTransform(&a);

    int ok = 1;
    for (int i = 0; i < 16; ++i) {
        float exp = (i == 0 || i == 5 || i == 10 || i == 15) ? 1.0f : 0.0f;
        if (a.m[i] != exp) ok = 0;
    }
    if (a.dirtyFlags != 0xc990u) ok = 0;
    if (a.inited != 1) ok = 0;

    // already initialized -> early return, no writes
    CShaderRenderManager b;
    zero(&b);
    b.inited = 1;
    CShaderRenderManager_ResetWorldTransform(&b);
    if (b.dirtyFlags != 0) ok = 0;
    if (b.m[0] != 0.0f) ok = 0;
    if (b.inited != 1) ok = 0;

    if (ok) {
        std::printf("CShaderRenderManager_00988290_TEST PASS\n");
        return 0;
    }
    std::printf("CShaderRenderManager_00988290_TEST FAIL\n");
    return 1;
}