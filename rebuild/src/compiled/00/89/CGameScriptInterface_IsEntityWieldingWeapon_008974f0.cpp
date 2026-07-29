class CScriptThing
{
};

class CGSIWielding_Node;
class CGSIWielding_Weapon;

class CGSIWielding_Container
{
public:
    CGSIWielding_Node* LowerBound(const long* pKey);
};

class CGSIWielding_Component
{
public:
    CGSIWielding_Weapon* GetWieldedWeapon();
};

class CGameScriptInterface
{
public:
    virtual bool IsEntityWieldingWeapon(
        const CScriptThing& entity) const;
};

__declspec(naked)
bool CGameScriptInterface::IsEntityWieldingWeapon(
    const CScriptThing& entity) const
{
    __asm
    {
        mov ecx, dword ptr [esp + 4]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 2Ch]
        test eax, eax
        je early_failure
        test byte ptr [eax + 91h], 1
        jne early_failure
        test byte ptr [eax + 28h], 40h
        je early_failure

        push esi
        lea ecx, [esp + 8]
        lea esi, [eax + 44h]
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 0Ch], 46h
        call CGSIWielding_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 46h
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov ecx, dword ptr [eax + 4]
        call CGSIWielding_Component::GetWieldedWeapon
        test eax, eax
        pop esi
        je failure
        mov ecx, dword ptr [eax + 28h]
        test ch, 1
        je failure
        mov eax, 1
        ret 4

failure:
        xor eax, eax
        ret 4

early_failure:
        xor al, al
        ret 4
    }
}
