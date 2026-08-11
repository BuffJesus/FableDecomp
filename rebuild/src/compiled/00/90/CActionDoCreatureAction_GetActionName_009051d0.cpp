// CActionDoCreatureAction::GetActionName  @ 0x009051d0
// Returns a CCharString by value, constructed from a fixed string literal
// with length -1 (NUL-terminated). 'this' is unused.

class CCharString {
public:
    CCharString(const char* s, int len);
private:
    char* m_data;
};

// The literal at 0x128d850 (content irrelevant to codegen shape).
extern const char kActionName[];

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionName, -1);
}