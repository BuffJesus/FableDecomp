#include "rebuild_abi.h"
#include <stdio.h>

struct CSkyDef { int tag; };

// Mock the relocation-masked real target for a self-contained run.
static int g_copied = 0;
void __fastcall CSkyDef_Copy_impl(CSkyDef* thisptr, const CSkyDef* src)
{
    if (thisptr && src) { thisptr->tag = src->tag; g_copied = 1; }
}

void __fastcall CSkyDef_Copy(CSkyDef* thisptr, const CSkyDef* src);

int main(void)
{
    CSkyDef dst; dst.tag = 0;
    CSkyDef src; src.tag = 0x1234;
    CSkyDef_Copy(&dst, &src);
    if (g_copied && dst.tag == 0x1234)
        printf("PASS_0046f4eb\n");
    else
        printf("FAIL_0046f4eb\n");
    return 0;
}