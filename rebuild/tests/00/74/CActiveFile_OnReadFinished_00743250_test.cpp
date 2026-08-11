#include <cstdio>

struct CActiveFile {
    void* field0;
};

static int g_base_called = 0;
static void* g_freed = 0;

void __fastcall CActiveFile_Base(CActiveFile* self) { g_base_called++; (void)self; }
void CActiveFile_FreeHelper(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base(self);
    void* p = self->field0;
    if (p)
        CActiveFile_FreeHelper(p);
}

int main() {
    CActiveFile a;
    int dummy = 42;
    a.field0 = &dummy;
    CActiveFile_OnReadFinished(&a);
    if (g_base_called == 1 && g_freed == &dummy) {
        // null case
        CActiveFile b;
        b.field0 = 0;
        g_freed = (void*)1;
        CActiveFile_OnReadFinished(&b);
        if (g_base_called == 2 && g_freed == (void*)1)
            printf("00743250_TEST PASS\n");
    }
    return 0;
}