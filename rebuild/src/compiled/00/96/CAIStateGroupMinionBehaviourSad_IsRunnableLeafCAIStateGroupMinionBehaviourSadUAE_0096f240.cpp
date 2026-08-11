// CAIStateGroup_MinionBehaviourSad::IsRunnableLeaf
// bool __fastcall (this)

struct CAIStateGroup_MinionBehaviourSad {
    char pad[0x38]; // after implicit vptr(4) -> field lands at 0x3c
    bool m_ran;      // +0x3c
    bool CanRun();   // non-virtual, direct call (defined elsewhere)
    virtual bool IsRunnableLeaf();
};

bool CAIStateGroup_MinionBehaviourSad::IsRunnableLeaf()
{
    if (!m_ran)
    {
        if (CanRun())
            m_ran = true;
    }
    return m_ran;
}