#include <cstdio>

struct CPlayerInventoryDef;

/* Provide the tail-call target so the test links standalone. */
static int g_called = 0;
extern "C" void CPlayerInventoryDef_Copy_impl_stub() {}

/* Mirror the function under test locally for a deterministic call check. */
extern void __fastcall CPlayerInventoryDef_Copy(CPlayerInventoryDef* thisptr, void* src);

/* Define the impl the thunk jumps to. */
void __fastcall CPlayerInventoryDef_Copy_impl(CPlayerInventoryDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

int main()
{
    CPlayerInventoryDef* obj = (CPlayerInventoryDef*)0;
    int srcbuf = 0x1234;
    CPlayerInventoryDef_Copy(obj, &srcbuf);
    if (g_called == 1)
        printf("OK_462e28\n");
    else
        printf("FAIL_462e28\n");
    return 0;
}