#include <cstdio>

struct CAIStateGroup_HoverAttackUnlessTargetIsFleeing
{
    int fleeing;
    bool IsTargetFleeing();
    bool IsRunnableLeaf();
};

// local definition of the helper for standalone link
bool CAIStateGroup_HoverAttackUnlessTargetIsFleeing::IsTargetFleeing()
{
    return fleeing != 0;
}

bool CAIStateGroup_HoverAttackUnlessTargetIsFleeing::IsRunnableLeaf()
{
    return !IsTargetFleeing();
}

int main()
{
    CAIStateGroup_HoverAttackUnlessTargetIsFleeing a;
    a.fleeing = 0;   // not fleeing -> runnable (true)
    CAIStateGroup_HoverAttackUnlessTargetIsFleeing b;
    b.fleeing = 1;   // fleeing -> not runnable (false)

    bool ra = a.IsRunnableLeaf();
    bool rb = b.IsRunnableLeaf();

    if (ra == true && rb == false)
    {
        printf("RUNNABLE_LEAF_OK\n");
        return 0;
    }
    printf("FAIL ra=%d rb=%d\n", (int)ra, (int)rb);
    return 1;
}