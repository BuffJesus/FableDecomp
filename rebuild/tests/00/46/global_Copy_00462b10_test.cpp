#include "rebuild_abi.h"
#include <cstdio>

struct CBrainDef;

// The thunk target is not linked in the test; provide a stub so the tail-call
// resolves. The parity gate only compares the thunk's bytes.
extern "C" void mark_called();
static int g_called = 0;
void mark_called() { g_called = 1; }

void __fastcall CBrainDef_Copy_impl(CBrainDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    mark_called();
}

void __fastcall CBrainDef_Copy(CBrainDef* thisptr, void* src);

int main()
{
    CBrainDef* self = (CBrainDef*)0;
    int local = 7;
    CBrainDef_Copy(self, &local);
    if (g_called)
        printf("OK_462b10_THUNK\n");
    else
        printf("FAIL_462b10\n");
    return 0;
}