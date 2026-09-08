#include <cstdio>
#include "engine/CExpressionSubDef.h"

struct CExpressionSubDef_Methods : CExpressionSubDef {
    void BaseClone(const CExpressionSubDef* source);
};

static int g_base_called = 0;
static void* g_base_self = 0;
static const void* g_base_src = 0;

void CExpressionSubDef_Methods::BaseClone(const CExpressionSubDef* src) {
    g_base_called = 1;
    g_base_self = this;
    g_base_src = src;
}

void __fastcall CExpressionSubDef_Copy(
    CExpressionSubDef_Methods* self, int /*edx*/, const CExpressionSubDef* src);

int main()
{
    CExpressionSubDef_Methods dst; dst.ExpressionDef = 111;
    CExpressionSubDef src; src.ExpressionDef = 999;

    CExpressionSubDef_Copy(&dst, 0, &src);

    if (g_base_called == 1 && g_base_self == &dst && g_base_src == (const void*)&src && dst.ExpressionDef == 999)
        printf("004e0ca7_TEST PASS\n");
    else
        printf("FAIL called=%d self=%d src=%d ExpressionDef=%d\n",
               g_base_called, g_base_self==&dst, g_base_src==(void*)&src, dst.ExpressionDef);
    return 0;
}
