#include "rebuild_abi.h"
#include <cstdio>

struct CLightningDef { int x; };

static CLightningDef* g_last_this = 0;
static void* g_last_src = 0;

extern "C" void __fastcall CLightningDef_Copy_impl(CLightningDef* thisptr, void* src)
{
    g_last_this = thisptr;
    g_last_src = src;
}

void __fastcall CLightningDef_Copy(CLightningDef* thisptr, void* src);

int main()
{
    CLightningDef obj;
    obj.x = 7;
    int srcbuf = 0;
    CLightningDef_Copy(&obj, &srcbuf);
    if (g_last_this == &obj && g_last_src == &srcbuf) {
        printf("PASS_00461ed3\n");
    } else {
        printf("FAIL_00461ed3\n");
    }
    return 0;
}