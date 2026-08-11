// CActionDoCreatureAction::GetActionName @ 0071b0e0
struct CCharString {
    char* p;
    CCharString(const char* s, int n);
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123b314, -1);
}