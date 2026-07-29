class CGSIHeroRenownLevel_Player;
class CGSIHeroRenownLevel_Node;

class CGSIHeroRenownLevel_Manager
{
public:
    CGSIHeroRenownLevel_Player* GetPlayer();
};

class CGSIHeroRenownLevel_Player
{
public:
    void* GetHero();
};

class CGSIHeroRenownLevel_Container
{
public:
    CGSIHeroRenownLevel_Node* LowerBound(
        const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual long GetHeroRenownLevel() const;
};

__declspec(naked)
long CGameScriptInterface::GetHeroRenownLevel() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIHeroRenownLevel_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroRenownLevel_Player::GetHero
        test eax, eax
        je failed
        test byte ptr [eax + 91h], 1
        jne failed
        test byte ptr [eax + 20h], 10h
        je fallback_component

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 4
        call CGSIHeroRenownLevel_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        mov eax, dword ptr [eax + 70h]
        pop esi
        pop ecx
        ret

fallback_component:
        mov eax, dword ptr [esp]
        mov eax, dword ptr [eax + 70h]
        pop ecx
        ret

failed:
        or eax, -1
        pop ecx
        ret
    }
}
