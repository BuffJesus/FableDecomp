
struct CMessageEvent;
struct CAIStateGroup_MinionKidCalledToMother
{
    CMessageEvent* FindCallHomeEvent();
    bool IsRunnable();
};

bool CAIStateGroup_MinionKidCalledToMother::IsRunnable()
{
    return FindCallHomeEvent() != 0;
}