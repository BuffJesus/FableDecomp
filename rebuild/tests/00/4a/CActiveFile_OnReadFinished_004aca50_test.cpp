#include <cstdio>

struct CActiveFile {
    void* buffer; // +0
};

static int g_base_called = 0;
static int g_free_called = 0;
static void* g_freed_ptr = 0;

extern "C" void __cdecl my_free(void* p) { g_free_called++; g_freed_ptr = p; }

void __fastcall CActiveFile_Base(CActiveFile* self) { g_base_called++; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base(self);
    void* p = self->buffer;
    if (p)
        my_free(p);
}

int main() {
    int dummy;
    CActiveFile a; a.buffer = &dummy;
    g_base_called = g_free_called = 0; g_freed_ptr = 0;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_base_called == 1) && (g_free_called == 1) && (g_freed_ptr == &dummy);

    CActiveFile b; b.buffer = 0;
    g_base_called = g_free_called = 0; g_freed_ptr = 0;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_base_called == 1) && (g_free_called == 0);

    if (ok1 && ok2)
        printf("004aca50_TEST PASS\n");
    else
        printf("FAIL b=%d f=%d\n", g_base_called, g_free_called);
    return 0;
}