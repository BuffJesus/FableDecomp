// CActionDoCreatureAction::GetActionName  @ 0x00905160
// Struct-return __fastcall member: returns CCharString("DoCreatureAction", -1) by value.
// The 'this' pointer is unused by the body.

struct CCharString {
    char* m_pData;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    int dummy;
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}