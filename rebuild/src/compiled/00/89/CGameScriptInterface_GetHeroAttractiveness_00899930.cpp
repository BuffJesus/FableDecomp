class CGSIHeroAttractiveness_Player;
class CGSIHeroAttractiveness_Node;

class CGSIHeroAttractiveness_Manager
{
public:
    CGSIHeroAttractiveness_Player* GetPlayer();
};

class CGSIHeroAttractiveness_Player
{
public:
    void* GetHero();
};

class CGSIHeroAttractiveness_Container
{
public:
    CGSIHeroAttractiveness_Node* LowerBound(
        const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual float GetHeroAttractiveness() const;
};

static const float g_CGSIHeroAttractiveness_Zero =
    0.0f;

__declspec(naked)
float CGameScriptInterface::GetHeroAttractiveness() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroAttractiveness_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroAttractiveness_Player::GetHero
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
        call CGSIHeroAttractiveness_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        fld dword ptr [eax + 09Ch]
        pop esi
        pop ecx
        ret

fallback_component:
        mov eax, dword ptr [esp]
        fld dword ptr [eax + 09Ch]
        pop ecx
        ret

invalid_hero:
        fld dword ptr [g_CGSIHeroAttractiveness_Zero]
        pop ecx
        ret
    }
}
