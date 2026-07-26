#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile_0046638b {
    void* field0;
    void* field4;
};

static int g_fast = 0;
static int g_cdecl = 0;

extern void __fastcall sub_fastcall_0046638b(void* a, void* b, char* out);
extern void __cdecl sub_cdecl_0046638b(void* p);

void __fastcall sub_fastcall_0046638b(void* a, void* b, char* out) {
    (void)a; (void)b;
    *out = 1;
    g_fast++;
}
void __cdecl sub_cdecl_0046638b(void* p) {
    (void)p;
    g_cdecl++;
}

extern void __fastcall OnReadFinished_0046638b(CActiveFile_0046638b* thisptr);

int main() {
    // Case A: field0 non-null -> both callees fire.
    int dummyA = 0, dummyB = 0;
    CActiveFile_0046638b a;
    a.field0 = &dummyA;
    a.field4 = &dummyB;
    OnReadFinished_0046638b(&a);

    // Case B: field0 null -> only fastcall fires, cdecl skipped.
    CActiveFile_0046638b b;
    b.field0 = 0;
    b.field4 = &dummyB;
    OnReadFinished_0046638b(&b);

    if (g_fast == 2 && g_cdecl == 1) {
        printf("OK_0046638b\n");
    } else {
        printf("FAIL_0046638b f=%d c=%d\n", g_fast, g_cdecl);
    }
    return 0;
}