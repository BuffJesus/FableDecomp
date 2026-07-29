class CScriptThing
{
};

class CGSIDecapitate_Entity;

class CGSIDecapitate_Action
{
public:
    CGSIDecapitate_Action* Construct(
        CGSIDecapitate_Entity* pEntity);
    void Destruct();
};

class CGSIDecapitate_Entity
{
public:
    void Decapitate(CGSIDecapitate_Action* pAction);
};

class CGameScriptInterface
{
public:
    virtual void EntityDecapitate(
        const CScriptThing& entity) const;
};

__declspec(naked)
void CGameScriptInterface::EntityDecapitate(
    const CScriptThing& entity) const
{
    __asm
    {
        sub esp, 114h
        push esi
        mov esi, dword ptr [esp + 11Ch]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 12Ch]
        test al, al
        je finished

        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2Ch]
        mov esi, eax
        test dword ptr [esi + 38h], 20000h
        je finished
        test byte ptr [esi + 6Ch], 0Fh
        je finished
        test byte ptr [esi + 0BCh], 2
        je finished

        push esi
        lea ecx, [esp + 8]
        call CGSIDecapitate_Action::Construct
        push eax
        mov ecx, esi
        call CGSIDecapitate_Entity::Decapitate
        lea ecx, [esp + 4]
        call CGSIDecapitate_Action::Destruct

finished:
        pop esi
        add esp, 114h
        ret 4
    }
}
