// CActionDoCreatureAction::GetActionName @ 0072ef40
// __fastcall returning CCharString by value; ignores `this`, builds string from a literal.

class CCharString {
public:
    CCharString(const char* s, int len);
    void* m_data;
};

// The ctor is external (relocation-masked). Provide a declaration only; the
// harness links a real impl for the test but masks its address in the compare.
class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("CActionDoCreatureAction", -1);
}