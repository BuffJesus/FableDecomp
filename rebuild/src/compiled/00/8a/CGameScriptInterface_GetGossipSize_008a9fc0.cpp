struct CCharString
{
    void CopyCtor();
    void Dtor();
};

class CGameScriptInterface
{
public:
    virtual int GetGossipSize(CCharString* gossip) const;
};

extern "C" void map_gossip_subscript();

__declspec(naked)
int CGameScriptInterface::GetGossipSize(CCharString* gossip) const
{
    __asm
    {
        push ecx
        push esi
        lea eax, [esp + 0Ch]
        push eax
        lea ecx, [esp + 8]
        call CCharString::CopyCtor
        lea ecx, [esp + 4]
        push ecx
        mov ecx, 013BAE44h
        call map_gossip_subscript
        mov ecx, dword ptr [eax]
        mov esi, dword ptr [eax + 4]
        sub esi, ecx
        lea ecx, [esp + 4]
        sar esi, 2
        call CCharString::Dtor
        lea ecx, [esp + 0Ch]
        call CCharString::Dtor
        mov eax, esi
        pop esi
        pop ecx
        ret 4
    }
}
