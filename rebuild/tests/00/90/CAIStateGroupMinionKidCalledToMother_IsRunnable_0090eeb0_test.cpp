
#include <cstdio>

struct CMessageEvent;
struct CAIStateGroup_MinionKidCalledToMother
{
    CMessageEvent* FindCallHomeEvent();
    bool IsRunnable();
};

static CMessageEvent* g_ret = 0;

CMessageEvent* CAIStateGroup_MinionKidCalledToMother::FindCallHomeEvent()
{
    return g_ret;
}

bool CAIStateGroup_MinionKidCalledToMother::IsRunnable()
{
    return FindCallHomeEvent() != 0;
}

int main()
{
    CAIStateGroup_MinionKidCalledToMother obj;

    g_ret = 0;
    bool r1 = obj.IsRunnable();

    g_ret = (CMessageEvent*)0x12345;
    bool r2 = obj.IsRunnable();

    if (r1 == false && r2 == true)
        printf("PASS_MARKER_OK\n");
    else
        printf("FAIL r1=%d r2=%d\n", (int)r1, (int)r2);

    return 0;
}