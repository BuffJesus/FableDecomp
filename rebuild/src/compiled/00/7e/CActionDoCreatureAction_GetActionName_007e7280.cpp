// CActionDoCreatureAction::GetActionName @ 0x007e7280
// Returns a CCharString constructed from a fixed string literal.

class CCharString {
public:
    CCharString(const char* s, int n);
    void* p;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x01270934, -1);
}