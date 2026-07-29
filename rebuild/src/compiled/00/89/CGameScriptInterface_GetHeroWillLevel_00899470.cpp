class CGSIHeroWill_Player;
class CGSIHeroWill_Node;

class CGSIHeroWill_Manager
{
public:
    CGSIHeroWill_Player* GetPlayer();
};

class CGSIHeroWill_Player
{
public:
    void* GetHero();
};

class CGSIHeroWill_Container
{
public:
    CGSIHeroWill_Node* LowerBound(const long* pKey);
};

class CGSIHeroWill_Component
{
public:
    long GetCategoryLevel(long category);
};

class CGameScriptInterface
{
public:
    virtual long GetHeroWillLevel() const;
};

__declspec(naked)
long CGameScriptInterface::GetHeroWillLevel() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroWill_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroWill_Player::GetHero
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
        call CGSIHeroWill_Container::LowerBound
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
        push 2
        mov ecx, eax
        call CGSIHeroWill_Component::GetCategoryLevel
        pop ecx
        ret
fallback_component:
        mov eax, dword ptr [esp]
        push 2
        mov ecx, eax
        call CGSIHeroWill_Component::GetCategoryLevel
        pop ecx
        ret
failed:
        or eax, -1
        pop ecx
        ret
    }
}
