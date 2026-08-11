// CActionDoCreatureAction::GetActionName @ 006af060
// __fastcall, returns CCharString BY VALUE (hidden return pointer).
// Retail: push esi; esi=[esp+8]=hidden ret buf; push -1; push 0x123c110;
//         mov ecx,esi; call CCharString(const char*,int); mov eax,esi; pop esi; ret 4.

struct CCharString {
    char* p;
    // this(ecx)=self, stack args (const char*, int)
    CCharString(const char* s, int len);
};

class CActionDoCreatureAction {
    int dummy;
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123c110, -1);
}