class CScriptThing
{
};

class CGSISetAttack_Entity;

class CGSISetAttack_Action
{
public:
    CGSISetAttack_Action* Construct(
        CGSISetAttack_Entity* pAttacker,
        CGSISetAttack_Entity* pTarget);
    void Destruct();
};

class CGSISetAttack_Entity
{
public:
    void Dispatch(CGSISetAttack_Action* pAction);
};

class CGameScriptInterface
{
public:
    virtual void EntitySetAttackThingImmediately(
        const CScriptThing& attacker,
        const CScriptThing& target,
        bool unknown0,
        bool unknown1) const;
};

__declspec(naked)
void CGameScriptInterface::EntitySetAttackThingImmediately(
    const CScriptThing& attacker,
    const CScriptThing& target,
    bool unknown0,
    bool unknown1) const
{
    __asm
    {
        mov ecx, dword ptr [esp + 4]
        mov eax, dword ptr [ecx]
        sub esp, 118h
        push esi
        call dword ptr [eax + 2Ch]

        mov ecx, dword ptr [esp + 124h]
        mov edx, dword ptr [ecx]
        mov esi, eax
        call dword ptr [edx + 2Ch]

        test esi, esi
        je finished
        mov dl, byte ptr [esi + 91h]
        mov cl, 1
        test dl, cl
        jne finished
        test eax, eax
        je finished
        test byte ptr [eax + 91h], cl
        jne finished
        test byte ptr [esi + 6Ch], 8
        je finished

        push eax
        push esi
        lea ecx, [esp + 0Ch]
        call CGSISetAttack_Action::Construct
        push eax
        mov ecx, esi
        call CGSISetAttack_Entity::Dispatch
        lea ecx, [esp + 4]
        call CGSISetAttack_Action::Destruct

finished:
        pop esi
        add esp, 118h
        ret 10h
    }
}
