class CGSIHeroStrength_Player;
class CGSIHeroStrength_Node;

class CGSIHeroStrength_Manager
{
public:
    CGSIHeroStrength_Player* GetPlayer();
};

class CGSIHeroStrength_Player
{
public:
    void* GetHero();
};

class CGSIHeroStrength_Container
{
public:
    CGSIHeroStrength_Node* LowerBound(const long* pKey);
};

class CGSIHeroStrength_Component
{
public:
    long GetCategoryLevel(long category);
};

class CGameScriptInterface
{
public:
    virtual long GetHeroStrengthLevel() const;
};

__declspec(naked)
long CGameScriptInterface::GetHeroStrengthLevel() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroStrength_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroStrength_Player::GetHero
        test eax, eax
        je failed
        test byte ptr [eax + 91h], 1
        jne failed
        mov ecx, dword ptr [eax + 2Ch]
        test ch, 1
        je fallback_component
        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 68h
        call CGSIHeroStrength_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 68h
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        pop esi
        push 0
        mov ecx, eax
        call CGSIHeroStrength_Component::GetCategoryLevel
        pop ecx
        ret
fallback_component:
        mov eax, dword ptr [esp]
        push 0
        mov ecx, eax
        call CGSIHeroStrength_Component::GetCategoryLevel
        pop ecx
        ret
failed:
        or eax, -1
        pop ecx
        ret
    }
}
