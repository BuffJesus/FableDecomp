// CActionDoCreatureAction::GetActionName @ 0071a7f0
struct CCharString {
    char* storage_;
    CCharString(const char* text, long length);
};

struct CActionDoCreatureAction {
    int dummy;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("ACTION_DO_CREATURE_ACTION", -1);
}