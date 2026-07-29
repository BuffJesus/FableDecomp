class CGSIIncreaseRenown_Player;
class CGSIIncreaseRenown_Node;

class CGSIIncreaseRenown_Manager
{
public:
    CGSIIncreaseRenown_Player* GetPlayer();
};

class CGSIIncreaseRenown_Player
{
public:
    void* GetHero();
};

class CGSIIncreaseRenown_Container
{
public:
    CGSIIncreaseRenown_Node* LowerBound(
        const long* pKey);
};

class CGSIIncreaseRenown_Component
{
public:
    void UpgradeRenownLevel();
};

class CGameScriptInterface
{
public:
    virtual void IncreaseHeroRenownLevel() const;
};

__declspec(naked)
void CGameScriptInterface::IncreaseHeroRenownLevel() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIIncreaseRenown_Manager::GetPlayer
        mov ecx, eax
        call CGSIIncreaseRenown_Player::GetHero
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
        call CGSIIncreaseRenown_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        pop esi
        mov ecx, eax
        call CGSIIncreaseRenown_Component::UpgradeRenownLevel
        pop ecx
        ret

fallback_component:
        mov eax, dword ptr [esp]
        mov ecx, eax
        call CGSIIncreaseRenown_Component::UpgradeRenownLevel
finished:
        pop ecx
        ret
    }
}
