// CActionDoCreatureAction::GetActionName @ 007dfab0
// __fastcall member returning CCharString by value (hidden retptr on stack, this in ecx, ret 4).
// Constructs a CCharString from the string literal at 0x123d734 with length -1.

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123d734, -1);
}