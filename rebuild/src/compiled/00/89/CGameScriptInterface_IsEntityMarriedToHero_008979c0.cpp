class CScriptThing
{
};

class CGSIMarried_Node;

class CGSIMarried_Container
{
public:
    CGSIMarried_Node* LowerBound(const long* pKey);
};

class CGSIMarried_Component
{
public:
    bool IsMarriedToHero();
};

class CGameScriptInterface
{
public:
    virtual bool IsEntityMarriedToHero(
        const CScriptThing& entity) const;
};

__declspec(naked)
bool CGameScriptInterface::IsEntityMarriedToHero(
    const CScriptThing& entity) const
{
    __asm
    {
        mov ecx, dword ptr [esp + 4]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 2Ch]
        test eax, eax
        je failed
        test byte ptr [eax + 91h], 1
        jne failed
        cmp byte ptr [eax + 10h], 1
        jne failed
        test dword ptr [eax + 38h], 800000h
        je failed

        push esi
        lea ecx, [esp + 8]
        lea esi, [eax + 44h]
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 0D7h
        call CGSIMarried_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 0D7h
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov ecx, dword ptr [eax + 4]
        call CGSIMarried_Component::IsMarriedToHero
        test al, al
        pop esi
        je failed
        mov al, 1
        ret 4

failed:
        xor al, al
        ret 4
    }
}
