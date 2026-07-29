class CGSIWillEnergy_Player;
class CGSIWillEnergy_Node;

class CGSIWillEnergy_Manager
{
public:
    CGSIWillEnergy_Player* GetPlayer();
};

class CGSIWillEnergy_Player
{
public:
    void* GetHero();
};

class CGSIWillEnergy_Container
{
public:
    CGSIWillEnergy_Node* LowerBound(
        const long* pKey);
};

class CGSIWillEnergy_Component
{
public:
    void SetWillEnergyLevel(float level);
};

class CGameScriptInterface
{
public:
    virtual void SetHeroWillEnergyLevel(
        float level) const;
};

__declspec(naked)
void CGameScriptInterface::SetHeroWillEnergyLevel(
    float level) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIWillEnergy_Manager::GetPlayer
        mov ecx, eax
        call CGSIWillEnergy_Player::GetHero
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
        call CGSIWillEnergy_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov ecx, dword ptr [esp + 0Ch]
        mov eax, dword ptr [eax + 4]
        pop esi
        push ecx
        mov ecx, eax
        call CGSIWillEnergy_Component::SetWillEnergyLevel
        pop ecx
        ret 4

fallback_component:
        mov ecx, dword ptr [esp + 8]
        mov eax, dword ptr [esp + 8]
        push ecx
        mov ecx, eax
        call CGSIWillEnergy_Component::SetWillEnergyLevel
finished:
        pop ecx
        ret 4
    }
}
