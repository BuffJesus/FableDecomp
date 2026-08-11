// CActionDoCreatureAction::GetActionName  @ 0x00903570
// Returns a CCharString by value, constructed from a fixed string literal.
// The 'this' (CActionDoCreatureAction*) is passed in ecx but unused; the
// hidden struct-return buffer is the sole cleaned stack arg (ret 4).

struct CCharString {
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x128d70c, -1);
}