// CActionDoCreatureAction::GetActionName @ 0x00743dd0
// Real __fastcall member returning CCharString by value: this in ECX,
// hidden struct-return pointer on the stack (ret 4). this is unused.

class CCharString {
public:
    CCharString(const char* text, long length);
    long storage_;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}