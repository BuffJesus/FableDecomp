struct CCharString { char* p; CCharString(const char* s, int n); };
struct CActionDoCreatureAction { CCharString GetActionName(); };

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123b46c, -1);
}