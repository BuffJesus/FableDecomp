#include <cstdio>

struct CActiveFile {
    void* p0;
    void* p1;
};

static int g_handler_calls = 0;
static int g_free_calls = 0;
static void* g_handler_a = 0;
static void* g_handler_b = 0;
static void* g_freed = 0;

void __fastcall Handler(void* a, void* b, char* c)
{
    g_handler_calls++;
    g_handler_a = a;
    g_handler_b = b;
    *c = 1;
}

void __cdecl FreeMem(void* p)
{
    g_free_calls++;
    g_freed = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Handler(self->p0, self->p1, &local);
    if (self->p0)
        FreeMem(self->p0);
}

int main()
{
    int dummyA = 11, dummyB = 22;
    CActiveFile f;
    f.p0 = &dummyA;
    f.p1 = &dummyB;
    CActiveFile_OnReadFinished(&f);
    if (g_handler_calls != 1) { std::printf("FAIL handler calls %d\n", g_handler_calls); return 1; }
    if (g_handler_a != &dummyA) { std::printf("FAIL handler a\n"); return 1; }
    if (g_handler_b != &dummyB) { std::printf("FAIL handler b\n"); return 1; }
    if (g_free_calls != 1) { std::printf("FAIL free calls %d\n", g_free_calls); return 1; }
    if (g_freed != &dummyA) { std::printf("FAIL freed ptr\n"); return 1; }

    // null branch: no free
    CActiveFile f2;
    f2.p0 = 0;
    f2.p1 = &dummyB;
    CActiveFile_OnReadFinished(&f2);
    if (g_handler_calls != 2) { std::printf("FAIL handler calls2 %d\n", g_handler_calls); return 1; }
    if (g_free_calls != 1) { std::printf("FAIL free calls2 %d\n", g_free_calls); return 1; }

    std::printf("CActiveFile_004532c3_TEST PASS\n");
    return 0;
}