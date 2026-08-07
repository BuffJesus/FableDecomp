#include "rebuild_abi.h"

// CTCInventoryAbilities::AddLearnedExpression @ 0x005eedcf
struct FableAddLearned_005eedcf_sub
{
    void helper(long* parg);
};
struct FableAddLearned_005eedcf
{
    char pad[0x1bc];
    FableAddLearned_005eedcf_sub sub;   // +0x1bc
    void AddLearnedExpression(long arg);
};

void FableAddLearned_005eedcf::AddLearnedExpression(long arg)
{
    this->sub.helper(&arg);
}