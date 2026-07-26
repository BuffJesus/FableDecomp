#include "rebuild_abi.h"
#include <cstdio>

struct CFactionDef;

/* Provide the impl so the thunk links; record that it was reached. */
static int g_hit = 0;
extern "C" void mark_hit() { g_hit = 1; }

void __fastcall CFactionDef_Copy_impl(CFactionDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    mark_hit();
}

void __fastcall CFactionDef_Copy_thunk(CFactionDef* thisptr, void* src);

int main()
{
    int dummy = 0;
    CFactionDef_Copy_thunk((CFactionDef*)&dummy, &dummy);
    if (g_hit) {
        printf("OK_0045d2f7\n");
    } else {
        printf("FAIL_0045d2f7\n");
    }
    return 0;
}