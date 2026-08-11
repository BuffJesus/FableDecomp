#include <stdio.h>

static int g_base_called = 0;
static void* g_freed = 0;

struct CActiveFile {
    void* field0;
    void OnReadFinished();
};

void __fastcall sub_882b80(void* self) { g_base_called = 1; (void)self; }
void __cdecl sub_bfea14(void* p) { g_freed = p; }

void CActiveFile::OnReadFinished()
{
    sub_882b80(this);
    void* p = this->field0;
    if (p) {
        sub_bfea14(p);
    }
}

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    g_base_called = 0; g_freed = 0;
    a.OnReadFinished();
    if (!g_base_called) { printf("FAIL base\n"); return 1; }
    if (g_freed != &dummy) { printf("FAIL free\n"); return 1; }

    CActiveFile b;
    b.field0 = 0;
    g_base_called = 0; g_freed = (void*)1;
    b.OnReadFinished();
    if (!g_base_called) { printf("FAIL base2\n"); return 1; }
    if (g_freed != (void*)1) { printf("FAIL free2\n"); return 1; }

    printf("ONREADFINISHED_OK\n");
    return 0;
}