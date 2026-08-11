#include <stdio.h>

struct CActiveFile;

extern "C" void __cdecl fable_free(void* p);
void __fastcall CActiveFileBase_OnReadFinished(CActiveFile* self);

struct CActiveFile {
    void* field0;
};

void __fastcall OnReadFinished(CActiveFile* self);

static int g_baseCalled = 0;
static void* g_freed = (void*)-1;

void __fastcall CActiveFileBase_OnReadFinished(CActiveFile* self)
{
    (void)self;
    g_baseCalled++;
}

extern "C" void __cdecl fable_free(void* p)
{
    g_freed = p;
}

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFileBase_OnReadFinished(self);
    void* p = self->field0;
    if (p != 0)
        fable_free(p);
}

int main()
{
    int dummy = 123;

    CActiveFile a;
    a.field0 = &dummy;
    g_baseCalled = 0; g_freed = (void*)-1;
    OnReadFinished(&a);
    if (g_baseCalled != 1) { printf("FAIL base1\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free1\n"); return 1; }

    CActiveFile b;
    b.field0 = 0;
    g_baseCalled = 0; g_freed = (void*)-1;
    OnReadFinished(&b);
    if (g_baseCalled != 1) { printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)-1) { printf("FAIL free2\n"); return 1; }

    printf("004ba460_TEST PASS\n");
    return 0;
}