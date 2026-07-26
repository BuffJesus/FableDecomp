#include "rebuild_abi.h"
#include <stdio.h>

struct CActiveFile {
    void* field0;
};

static int g_freed = 0;
static void* g_last = 0;

extern "C" void __cdecl sub_free_46fedb(void* p);
extern void __fastcall OnReadFinished_0046fedb(CActiveFile* thisptr);

extern "C" void __cdecl sub_free_46fedb(void* p) {
    g_freed = 1;
    g_last = p;
}

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    OnReadFinished_0046fedb(&a);

    CActiveFile b;
    b.field0 = 0;
    OnReadFinished_0046fedb(&b);

    if (g_freed == 1 && g_last == &dummy) {
        printf("PASS_0046fedb\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}