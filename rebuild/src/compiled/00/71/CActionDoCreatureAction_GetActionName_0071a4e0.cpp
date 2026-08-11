// CActionDoCreatureAction::GetActionName @ 0071a4e0
// __fastcall, returns CCharString BY VALUE (hidden return pointer).
// Retail: push esi; esi=[esp+8]=hidden ret buf; push -1; push 0x12659e4;
//         mov ecx,esi; call CCharString(const char*,int); mov eax,esi; pop esi; ret 4.

struct CCharString {
    char* p;
    CCharString(const char* s, int len);
};

class CActionDoCreatureAction {
    int dummy;
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x12659e4, -1);
}