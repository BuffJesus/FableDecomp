class CGSIHeroHearing_Player;
class CGSIHeroHearing_Node;

class CGSIHeroHearing_Manager
{
public:
    CGSIHeroHearing_Player* GetPlayer();
};

class CGSIHeroHearing_Player
{
public:
    void* GetHero();
};

class CGSIHeroHearing_Container
{
public:
    CGSIHeroHearing_Node* LowerBound(
        const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual float GetDistanceHeroCanBeHeardFrom() const;
};

static const float g_CGSIHeroHearing_Zero = 0.0f;

__declspec(naked)
float CGameScriptInterface::
GetDistanceHeroCanBeHeardFrom() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroHearing_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroHearing_Player::GetHero
        test eax, eax
        je invalid_hero
        test byte ptr [eax + 91h], 1
        jne invalid_hero
        test dword ptr [eax + 24h], 400000h
        je fallback_component

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 36h
        call CGSIHeroHearing_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 36h
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        fld dword ptr [eax + 14h]
        pop esi
        pop ecx
        ret

fallback_component:
        mov eax, dword ptr [esp]
        fld dword ptr [eax + 14h]
        pop ecx
        ret

invalid_hero:
        fld dword ptr [g_CGSIHeroHearing_Zero]
        pop ecx
        ret
    }
}
