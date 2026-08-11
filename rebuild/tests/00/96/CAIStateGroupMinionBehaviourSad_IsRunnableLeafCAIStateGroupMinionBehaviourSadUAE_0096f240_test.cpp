#include <stdio.h>

struct CAIStateGroup_MinionBehaviourSad {
    char pad[0x3c];
    bool m_ran;      // +0x3c
    bool CanRun();
    bool IsRunnableLeaf();
};

// standalone CanRun: models the retail 0x96f180 helper. Here it just reports m_ran.
bool CAIStateGroup_MinionBehaviourSad::CanRun()
{
    return m_ran;
}

bool CAIStateGroup_MinionBehaviourSad::IsRunnableLeaf()
{
    if (!m_ran)
    {
        if (CanRun())
            m_ran = true;
    }
    return m_ran;
}

int main()
{
    CAIStateGroup_MinionBehaviourSad a;
    // already-ran: returns true, stays true
    a.m_ran = true;
    bool r1 = a.IsRunnableLeaf();

    // not-yet-ran, CanRun()==false (m_ran false) -> stays false
    a.m_ran = false;
    bool r2 = a.IsRunnableLeaf();

    if (r1 == true && r2 == false)
    {
        printf("IsRunnableLeaf_OK\n");
        return 0;
    }
    printf("IsRunnableLeaf_FAIL\n");
    return 1;
}