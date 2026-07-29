class CGSIHeroFatness_Player;
class CGSIHeroFatness_Node;

class CGSIHeroFatness_Manager
{
public:
    CGSIHeroFatness_Player* GetPlayer();
};

class CGSIHeroFatness_Player
{
public:
    void* GetHero();
};

class CGSIHeroFatness_Container
{
public:
    CGSIHeroFatness_Node* LowerBound(
        const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual float GetHeroFatness() const;
};

static const float g_CGSIHeroFatness_Invalid =
    -999.0f;

__declspec(naked)
float CGameScriptInterface::GetHeroFatness() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroFatness_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroFatness_Player::GetHero
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
        call CGSIHeroFatness_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        fld dword ptr [eax + 38h]
        pop esi
        pop ecx
        ret

fallback_component:
        mov eax, dword ptr [esp]
        fld dword ptr [eax + 38h]
        pop ecx
        ret

invalid_hero:
        fld dword ptr [g_CGSIHeroFatness_Invalid]
        pop ecx
        ret
    }
}
