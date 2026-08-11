// CActionDoCreatureAction::GetActionName @ 0x007d2220
// __fastcall member returning a CCharString by value, constructed from a
// literal C-string with len=-1. The this pointer is unused.

struct CCharString {
    char* m_data;
    int   m_len;
    int   m_ref;
    CCharString(const char* s, int len);
};

static const char* const kActionNameLit = (const char*)0x0123d4b0;

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionNameLit, -1);
}