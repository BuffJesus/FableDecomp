#include <cstdio>

struct COpinionReactionMaskDef;

// Provide the impl the thunk tail-calls so the test links and runs.
static int g_called = 0;
void __fastcall COpinionReactionMaskDef_Copy_impl(COpinionReactionMaskDef* thisptr, void* src)
{
    (void)thisptr; (void)src;
    g_called = 1;
}

void __fastcall COpinionReactionMaskDef_Copy(COpinionReactionMaskDef* thisptr, void* src);

int main()
{
    COpinionReactionMaskDef* self = (COpinionReactionMaskDef*)0x1000;
    void* src = (void*)0x2000;
    COpinionReactionMaskDef_Copy(self, src);
    if (g_called == 1)
        printf("PASS_0045dd00\n");
    else
        printf("FAIL_0045dd00\n");
    return 0;
}