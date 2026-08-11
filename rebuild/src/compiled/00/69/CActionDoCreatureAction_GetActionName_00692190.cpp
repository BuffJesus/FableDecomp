struct CCharString {
    // real member ctor => __fastcall(->__fastcall): this in ECX, (lit,-1) pushed, callee-cleaned.
    CCharString(const char* lit, int len);
    int dummy;
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x125c110, -1);
}