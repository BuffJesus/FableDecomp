#include <cstdio>

struct CActiveFile { void* m_buffer; };

static int g_helper_calls = 0;
static void* g_freed = (void*)-1;

void __fastcall CActiveFile_Helper(void* self) { (void)self; ++g_helper_calls; }
void the_free(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    // case 1: non-null buffer -> helper called, free(buffer)
    int dummy;
    CActiveFile a; a.m_buffer = &dummy;
    g_helper_calls = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_helper_calls == 1) && (g_freed == &dummy);

    // case 2: null buffer -> helper called, no free
    CActiveFile b; b.m_buffer = 0;
    g_helper_calls = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_helper_calls == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) { printf("ONREADFINISHED_OK\n"); return 0; }
    printf("FAIL o1=%d o2=%d\n", ok1, ok2);
    return 1;
}