#include <cstdio>

extern "C" void* g_movieTexture = 0;   // ds:0x013b8ac0
extern "C" void* g_movieDevice  = 0;   // ds:0x013b8ab8

__declspec(naked) void FreeTextures()
{
    __asm {
        mov     eax, dword ptr [g_movieTexture]
        test    eax, eax
        je      done
        mov     ecx, dword ptr [g_movieDevice]
        mov     edx, dword ptr [ecx]
        push    eax
        call    dword ptr [edx+0x118]
        mov     dword ptr [g_movieTexture], 0
    done:
        ret
    }
}

static void*  g_releasedArg  = 0;
static int    g_releaseCalls = 0;

static void __stdcall ReleaseTexture(void* tex)
{
    g_releasedArg = tex;
    g_releaseCalls++;
}

static void* g_vtable[80];
static void* g_deviceObj[1];

int main()
{
    for (int i = 0; i < 80; ++i) g_vtable[i] = 0;
    g_vtable[0x118 / 4] = (void*)&ReleaseTexture;
    g_deviceObj[0] = (void*)&g_vtable[0];
    g_movieDevice  = (void*)&g_deviceObj[0];

    int dummyTex = 0xABCD;
    g_movieTexture = (void*)&dummyTex;
    FreeTextures();
    if (g_releaseCalls != 1)               { std::printf("FAIL call count %d\n", g_releaseCalls); return 1; }
    if (g_releasedArg != (void*)&dummyTex) { std::printf("FAIL released arg\n"); return 1; }
    if (g_movieTexture != 0)               { std::printf("FAIL cache not cleared\n"); return 1; }

    g_releaseCalls = 0;
    g_movieTexture = 0;
    FreeTextures();
    if (g_releaseCalls != 0)               { std::printf("FAIL null path called %d\n", g_releaseCalls); return 1; }
    if (g_movieTexture != 0)               { std::printf("FAIL null path dirtied\n"); return 1; }

    std::printf("OK_0x00547b90\n");
    return 0;
}