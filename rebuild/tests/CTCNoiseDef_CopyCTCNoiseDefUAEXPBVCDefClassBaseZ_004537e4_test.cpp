#include <cstdio>
struct CDefClassBase;
struct CTCNoiseDef;
static int g_called = 0;
static const void* g_self = 0;
static const void* g_other = 0;
void __fastcall CTCNoiseDef_Copy_impl(CTCNoiseDef* self, void* edx, const CDefClassBase* other)
{
    g_called = 1;
    g_self = self;
    g_other = other;
    (void)edx;
}
void __fastcall CTCNoiseDef_Copy(CTCNoiseDef* self, void* edx, const CDefClassBase* other)
{
    CTCNoiseDef_Copy_impl(self, edx, other);
}
int main()
{
    char selfObj[8];
    char otherObj[8];
    CTCNoiseDef_Copy((CTCNoiseDef*)selfObj, 0, (const CDefClassBase*)otherObj);
    if (g_called == 1 && g_self == (const void*)selfObj && g_other == (const void*)otherObj) {
        std::printf("CTCNoiseDef_004537e4_TEST PASS\n");
        return 0;
    }
    std::printf("CTCNoiseDef_004537e4_TEST FAIL\n");
    return 1;
}