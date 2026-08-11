// CActionDoCreatureAction::GetActionName  @ 00798d30
// __fastcall CCharString GetActionName(CActionDoCreatureAction* this)
// Body: return CCharString((const char*)0x0126C12C, -1);
// The this-pointer is unused; a CCharString is constructed in the hidden
// return buffer from a literal string address and length -1.

class CCharString {
public:
    // Ctor at 0x0099EBF3, __fastcall(this, const char* s, int len)
    CCharString(const char* s, int len);
private:
    char* m_p;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0126C12C, -1);
}