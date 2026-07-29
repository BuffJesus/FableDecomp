class CGSIWillEnergyGet_Player;
class CGSIWillEnergyGet_Node;

class CGSIWillEnergyGet_Manager
{
public:
    CGSIWillEnergyGet_Player* GetPlayer();
};

class CGSIWillEnergyGet_Player
{
public:
    void* GetHero();
};

class CGSIWillEnergyGet_Container
{
public:
    CGSIWillEnergyGet_Node* LowerBound(
        const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual float GetHeroWillEnergyLevel() const;
};

static const float g_CGSIWillEnergyGet_Zero = 0.0f;

__declspec(naked)
float CGameScriptInterface::GetHeroWillEnergyLevel() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIWillEnergyGet_Manager::GetPlayer
        mov ecx, eax
        call CGSIWillEnergyGet_Player::GetHero
        test eax, eax
        je invalid_hero
        test byte ptr [eax + 91h], 1
        jne invalid_hero
        test byte ptr [eax + 20h], 10h
        je fallback_component

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 4
        call CGSIWillEnergyGet_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        fild dword ptr [eax + 58h]
        pop esi
        fidiv dword ptr [eax + 5Ch]
        pop ecx
        ret

fallback_component:
        mov eax, dword ptr [esp]
        fild dword ptr [eax + 58h]
        fidiv dword ptr [eax + 5Ch]
        pop ecx
        ret

invalid_hero:
        fld dword ptr [g_CGSIWillEnergyGet_Zero]
        pop ecx
        ret
    }
}
