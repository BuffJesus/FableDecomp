#include "rebuild_abi.h"
#include <cstdio>
static void* g_this = 0; static long g_val = 0;
struct FableAddLearned_005eedcf_sub
{
    void helper(long* parg);
};
void FableAddLearned_005eedcf_sub::helper(long* parg)
{
    g_this = (void*)this; g_val = *parg;
}
struct FableAddLearned_005eedcf
{
    char pad[0x1bc];
    FableAddLearned_005eedcf_sub sub;
    void AddLearnedExpression(long arg);
};
int main()
{
    static FableAddLearned_005eedcf obj;
    obj.AddLearnedExpression(0x1234);
    if (g_this != (void*)((char*)&obj + 0x1bc)) { std::printf("BAD_THIS\n"); return 1; }
    if (g_val != 0x1234) { std::printf("BAD_VAL\n"); return 1; }
    std::printf("ADDLEARNED_005eedcf_OK\n");
    return 0;
}