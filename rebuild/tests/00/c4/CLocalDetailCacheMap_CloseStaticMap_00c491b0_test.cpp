#include <cstdio>

static void* g_freed = (void*)0;

extern "C" void __cdecl my_free(void* p) { g_freed = p; }

struct CLocalDetailCacheMap {
    char pad[0x10];
    void* f10;
};

void __fastcall CloseStaticMap(CLocalDetailCacheMap* self);

int main()
{
    CLocalDetailCacheMap obj;
    obj.f10 = (void*)0x12345678;
    CloseStaticMap(&obj);
    if (g_freed == (void*)0x12345678 && obj.f10 == (void*)0) {
        std::printf("00c491b0_TEST PASS\n");
    } else {
        std::printf("FAIL freed=%p f10=%p\n", g_freed, obj.f10);
    }
    return 0;
}