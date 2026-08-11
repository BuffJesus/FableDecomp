#include <cstdio>

struct CActiveFile {
    void* field0;
};

static int g_base_called = 0;
static int g_free_called = 0;
static void* g_free_arg = (void*)0;

void __fastcall CActiveFile_base(CActiveFile* self) { g_base_called++; (void)self; }
void free_helper(void* p) { g_free_called++; g_free_arg = p; }

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->field0;
    if (p)
        free_helper(p);
}

int main()
{
    // Case 1: field0 non-null -> base called, free called with field0
    CActiveFile a;
    a.field0 = (void*)0xDEAD;
    g_base_called = g_free_called = 0; g_free_arg = 0;
    OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_free_called == 1) && (g_free_arg == (void*)0xDEAD);

    // Case 2: field0 null -> base called, free NOT called
    CActiveFile b;
    b.field0 = (void*)0;
    g_base_called = g_free_called = 0; g_free_arg = 0;
    OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_free_called == 0);

    if (ok1 && ok2)
        printf("00567880_TEST PASS\n");
    else
        printf("FAIL b=%d f=%d\n", g_base_called, g_free_called);
    return 0;
}