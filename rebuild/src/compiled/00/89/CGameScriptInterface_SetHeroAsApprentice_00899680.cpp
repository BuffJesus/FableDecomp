class CGSIHeroApprentice_Player;
class CGSIHeroApprentice_Node;

class CGSIHeroApprentice_Manager
{
public:
    CGSIHeroApprentice_Player* GetPlayer();
};

class CGSIHeroApprentice_Player
{
public:
    void* GetHero();
};

class CGSIHeroApprentice_Container
{
public:
    CGSIHeroApprentice_Node* LowerBound(
        const long* pKey);
};

class CGSIHeroApprentice_Component
{
public:
    void ResetTrainingStatus();
};

class CGameScriptInterface
{
public:
    virtual void SetHeroAsApprentice(
        bool apprentice) const;
};

__declspec(naked)
void CGameScriptInterface::SetHeroAsApprentice(
    bool apprentice) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroApprentice_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroApprentice_Player::GetHero
        test eax, eax
        je finished
        test byte ptr [eax + 91h], 1
        jne finished
        test byte ptr [eax + 20h], 10h
        je fallback_component

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 4
        call CGSIHeroApprentice_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_entry
use_sentinel:
        mov eax, esi
have_entry:
        mov eax, dword ptr [eax + 4]
        pop esi
        jmp have_component

fallback_component:
        mov eax, dword ptr [esp + 8]
have_component:
        mov cl, byte ptr [esp + 8]
        test cl, cl
        je reset_training
        mov dword ptr [eax + 114h], 1
        pop ecx
        ret 4

reset_training:
        mov ecx, eax
        call CGSIHeroApprentice_Component::ResetTrainingStatus
finished:
        pop ecx
        ret 4
    }
}
