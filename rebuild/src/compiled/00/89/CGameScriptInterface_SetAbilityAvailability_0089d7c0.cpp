enum EHeroAbility
{
    EHeroAbility_First = 0
};

class CGSIAbility_Player;
class CGSIAbility_Node;

class CGSIAbility_Manager
{
public:
    CGSIAbility_Player* GetPlayer();
};

class CGSIAbility_Player
{
public:
    void* GetHero();
};

class CGSIAbility_Container
{
public:
    CGSIAbility_Node* LowerBound(const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual void SetAbilityAvailability(
        EHeroAbility ability,
        bool available) const;
};

__declspec(naked)
void CGameScriptInterface::SetAbilityAvailability(
    EHeroAbility ability,
    bool available) const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIAbility_Manager::GetPlayer
        mov ecx, eax
        call CGSIAbility_Player::GetHero
        test eax, eax
        je finished
        test byte ptr [eax + 91h], 1
        jne finished
        mov cl, byte ptr [eax + 34h]
        test cl, cl
        jns finished

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 0A7h
        call CGSIAbility_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 0A7h
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        mov ecx, dword ptr [eax + 1Ch]
        mov dl, byte ptr [esp + 10h]
        mov eax, dword ptr [esp + 0Ch]
        mov byte ptr [eax + ecx], dl
        pop esi

finished:
        pop ecx
        ret 8
    }
}
