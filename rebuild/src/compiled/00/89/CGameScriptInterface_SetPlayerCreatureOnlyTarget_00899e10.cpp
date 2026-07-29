class CScriptThing
{
};

class CGSIOnlyTarget_Player;
class CGSIOnlyTarget_Node;

class CGSIOnlyTarget_Manager
{
public:
    CGSIOnlyTarget_Player* GetPlayer();
};

class CGSIOnlyTarget_Player
{
public:
    void* GetHero();
};

class CGSIOnlyTarget_Container
{
public:
    CGSIOnlyTarget_Node* LowerBound(
        const long* pKey);
};

class CGSIOnlyTarget_Component
{
public:
    void SetOnlyTarget(void* pThing);
};

class CGameScriptInterface
{
public:
    virtual void SetPlayerCreatureOnlyTarget(
        const CScriptThing& thing) const;
};

__declspec(naked)
void CGameScriptInterface::SetPlayerCreatureOnlyTarget(
    const CScriptThing& thing) const
{
    __asm
    {
        mov eax, dword ptr [ecx + 8]
        mov ecx, dword ptr [eax + 1Ch]
        push esi
        push edi
        call CGSIOnlyTarget_Manager::GetPlayer
        mov ecx, eax
        call CGSIOnlyTarget_Player::GetHero
        mov edi, dword ptr [esp + 0Ch]
        mov edx, dword ptr [edi]
        mov ecx, edi
        mov esi, eax
        call dword ptr [edx + 12Ch]
        test al, al
        je finished
        test esi, esi
        je finished
        test byte ptr [esi + 91h], 1
        jne finished

        mov eax, dword ptr [edi]
        mov ecx, edi
        call dword ptr [eax + 2Ch]
        mov edi, eax
        mov eax, dword ptr [esi + 20h]
        test ah, 1
        je finished

        lea ecx, [esp + 0Ch]
        add esi, 44h
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 10h], 8
        call CGSIOnlyTarget_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 8
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        push edi
        mov ecx, eax
        call CGSIOnlyTarget_Component::SetOnlyTarget
finished:
        pop edi
        pop esi
        ret 4
    }
}
