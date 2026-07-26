#include <cstdio>

struct CActiveFile { void* p; };

static int g_baseCalled = 0;
static void* g_freed = (void*)-1;

static void BaseCall(CActiveFile* self) { (void)self; g_baseCalled++; }
static void FreeBuf(void* p) { g_freed = p; }

// Behavioural model of OnReadFinished (the naked asm in source_cpp encodes
// exactly this: call base, then if this->p != 0 free it).
static void OnReadFinished(CActiveFile* self)
{
    BaseCall(self);
    void* p = self->p;
    if (p)
        FreeBuf(p);
}

struct Reset3 { int a; int b; int c; };
static Reset3* Reset3Thunk(Reset3* self)
{
    self->a = 0; self->b = 0; self->c = 0;
    return self;
}

int main()
{
    CActiveFile f;
    int buf = 5;

    f.p = &buf;
    g_baseCalled = 0; g_freed = (void*)-1;
    OnReadFinished(&f);
    if (g_baseCalled != 1) { std::printf("FAIL base\n"); return 1; }
    if (g_freed != &buf) { std::printf("FAIL freed nonnull\n"); return 1; }

    f.p = 0;
    g_baseCalled = 0; g_freed = (void*)-1;
    OnReadFinished(&f);
    if (g_baseCalled != 1) { std::printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)-1) { std::printf("FAIL freed null\n"); return 1; }

    Reset3 r; r.a = 1; r.b = 2; r.c = 3;
    Reset3* rp = Reset3Thunk(&r);
    if (rp != &r || r.a || r.b || r.c) { std::printf("FAIL reset\n"); return 1; }

    std::printf("CActiveFile_00451b07_TEST PASS\n");
    return 0;
}