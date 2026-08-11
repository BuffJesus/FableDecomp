// CActionDoCreatureAction::GetActionName  (retail 0x009035f0)
// __fastcall, returns CCharString by value (hidden retbuf as first stack arg).
// Body: return CCharString("...", -1);  -- 'this' unused.

class CCharString {
public:
    CCharString(const char *s, long n);   // 0x0099ebf0, out-of-line ctor
private:
    char *m_p;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
    // some data so 'this' is a real object (unused by the body)
    int m_dummy;
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}