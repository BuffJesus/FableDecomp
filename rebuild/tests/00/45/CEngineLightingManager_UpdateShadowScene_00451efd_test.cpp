#include <cstdio>

struct CEngineLightingManager {
    char pad00[0x18];
    int  mode;      // +0x18
    char pad1c[0x24-0x1c];
    int  handlerA;  // +0x24
    int  handlerB;  // +0x28
};

static int g_lock = 0;
static int g_lastA[3];
static int g_lastB[2];
static int g_calls = 0;

extern "C" void __stdcall LockScope(int id) { g_lock = id; }
extern "C" void __fastcall CallB(int self, int arg) { g_lastB[0]=self; g_lastB[1]=arg; g_calls++; }
extern "C" void __fastcall CallA(void* guard, int handler, int arg) { g_lastA[0]=(int)(guard!=0); g_lastA[1]=handler; g_lastA[2]=arg; g_calls++; }

void __fastcall CEngineLightingManager_UpdateShadowScene(CEngineLightingManager* self, void* /*edx*/, int* arg1)
{
    LockScope(0x122d70e);
    int guard = 0;
    int m = self->mode;
    if (m == 2) {
        CallA(&guard, self->handlerA, (int)arg1);
    } else if (m == 3) {
        CallB(self->handlerB, *arg1);
    }
}

int main() {
    CEngineLightingManager o;
    o.mode = 2; o.handlerA = 0x1111; o.handlerB = 0x2222;
    int a = 0x55;
    g_calls = 0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &a);
    if (g_lock != 0x122d70e) { std::printf("FAIL lock\n"); return 1; }
    if (g_calls != 1 || g_lastA[1] != 0x1111 || g_lastA[2] != (int)&a) { std::printf("FAIL A\n"); return 1; }

    o.mode = 3;
    g_calls = 0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &a);
    if (g_calls != 1 || g_lastB[0] != 0x2222 || g_lastB[1] != a) { std::printf("FAIL B\n"); return 1; }

    o.mode = 7;
    g_calls = 0;
    CEngineLightingManager_UpdateShadowScene(&o, 0, &a);
    if (g_calls != 0) { std::printf("FAIL none\n"); return 1; }

    std::printf("CEngineLightingManager_00451efd_TEST PASS\n");
    return 0;
}