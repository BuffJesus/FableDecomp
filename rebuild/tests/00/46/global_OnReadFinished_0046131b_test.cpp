#include "rebuild_abi.h"
#include <cstdio>

struct CActiveFile
{
    void* field_0;
};

extern "C" int g_called;
int g_called = 0;
extern "C" void __cdecl sub_79d6f9(void* p);
void __cdecl sub_79d6f9(void* p) { (void)p; g_called = 1; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* thisptr);

int main()
{
    CActiveFile a;
    a.field_0 = 0;
    CActiveFile_OnReadFinished(&a);
    if (g_called != 0) { printf("FAIL_0046131b\n"); return 1; }

    int dummy = 7;
    CActiveFile b;
    b.field_0 = &dummy;
    CActiveFile_OnReadFinished(&b);
    if (g_called != 1) { printf("FAIL_0046131b\n"); return 1; }

    printf("OK_0046131b\n");
    return 0;
}