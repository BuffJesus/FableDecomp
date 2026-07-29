class CGSIHeroSkill_Player;
class CGSIHeroSkill_Node;

class CGSIHeroSkill_Manager
{
public:
    CGSIHeroSkill_Player* GetPlayer();
};

class CGSIHeroSkill_Player
{
public:
    void* GetHero();
};

class CGSIHeroSkill_Container
{
public:
    CGSIHeroSkill_Node* LowerBound(const long* pKey);
};

class CGSIHeroSkill_Component
{
public:
    long GetCategoryLevel(long category);
};

class CGameScriptInterface
{
public:
    virtual long GetHeroSkillLevel() const;
};

__declspec(naked)
long CGameScriptInterface::GetHeroSkillLevel() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroSkill_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroSkill_Player::GetHero
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
        call CGSIHeroSkill_Container::LowerBound
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
        push 1
        mov ecx, eax
        call CGSIHeroSkill_Component::GetCategoryLevel
        pop ecx
        ret

fallback_component:
        mov eax, dword ptr [esp]
        push 1
        mov ecx, eax
        call CGSIHeroSkill_Component::GetCategoryLevel
        pop ecx
        ret

failed:
        or eax, -1
        pop ecx
        ret
    }
}
