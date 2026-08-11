#include <cstdio>

struct CRegion;

struct Singleton {
    void* method(void* arg);
};

static void* g_last_arg;
static void* g_ret_val = (void*)0xABCD1234;
Singleton g_singleton_13ca828;

void* Singleton::method(void* arg) {
    g_last_arg = arg;
    return g_ret_val;
}

struct CRegion {
    void* f0;
};

void __fastcall SetMiniMapRegionExitTextOffsetX(CRegion* self, int edx, void* arg);

int main() {
    CRegion r; r.f0 = 0;
    void* a = (void*)0x55667788;
    SetMiniMapRegionExitTextOffsetX(&r, 0, a);
    if (r.f0 == g_ret_val && g_last_arg == a) {
        std::printf("009d5060_TEST PASS\n");
    } else {
        std::printf("FAIL f0=%p arg=%p\n", r.f0, g_last_arg);
    }
    return 0;
}