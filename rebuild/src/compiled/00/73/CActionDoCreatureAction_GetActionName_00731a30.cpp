struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123a784, -1);
}