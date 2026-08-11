#include <cstdio>

struct CActiveFile {
    void* field0;
};

static int g_postCalls = 0;
static void* g_freed = (void*)-1;

void __fastcall CActiveFile_PostReadStep(CActiveFile* self) {
    (void)self;
    ++g_postCalls;
}
void __cdecl FreeBlock(void* p) {
    g_freed = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main() {
    // Case 1: non-null field0 -> PostReadStep called, FreeBlock(field0)
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    g_postCalls = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    bool ok1 = (g_postCalls == 1) && (g_freed == &dummy);

    // Case 2: null field0 -> PostReadStep called, FreeBlock NOT called
    CActiveFile b;
    b.field0 = 0;
    g_postCalls = 0; g_freed = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    bool ok2 = (g_postCalls == 1) && (g_freed == (void*)-1);

    if (ok1 && ok2) {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL c1=%d c2=%d\n", ok1, ok2);
    return 1;
}