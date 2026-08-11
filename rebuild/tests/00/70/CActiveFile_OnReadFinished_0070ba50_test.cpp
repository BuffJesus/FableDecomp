#include <cstdio>

struct CActiveFile { void* ptr; };

static int g_base_called = 0;
static void* g_freed = (void*)0;

extern void __fastcall base_OnReadFinished_impl(CActiveFile* self);
extern "C" void free_impl(void* p);

void __fastcall base_OnReadFinished_impl(CActiveFile* self) { g_base_called++; }
extern "C" void free_impl(void* p) { g_freed = p; }

void __fastcall OnReadFinished(CActiveFile* self);

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.ptr = &dummy;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&a);
    if (g_base_called != 1) { std::printf("FAIL base\n"); return 1; }
    if (g_freed != &dummy) { std::printf("FAIL free\n"); return 1; }

    CActiveFile b;
    b.ptr = (void*)0;
    g_base_called = 0; g_freed = (void*)0;
    OnReadFinished(&b);
    if (g_base_called != 1) { std::printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)0) { std::printf("FAIL free2\n"); return 1; }

    std::printf("0070ba50_TEST PASS\n");
    return 0;
}