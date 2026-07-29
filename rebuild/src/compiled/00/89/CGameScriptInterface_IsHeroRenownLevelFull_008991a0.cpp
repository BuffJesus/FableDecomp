class CGSIRenownFull_Player;
class CGSIRenownFull_Node;

class CGSIRenownFull_Manager
{
public:
    CGSIRenownFull_Player* GetPlayer();
};

class CGSIRenownFull_Player
{
public:
    void* GetHero();
};

class CGSIRenownFull_Container
{
public:
    CGSIRenownFull_Node* LowerBound(
        const long* pKey);
};

class CGSIRenownFull_Component
{
public:
    float GetPercentageThroughRenownLevel();
};

class CGameScriptInterface
{
public:
    virtual bool IsHeroRenownLevelFull() const;
};

static const float g_CGSIRenownFull_One = 1.0f;
static const float g_CGSIRenownFull_Tolerance =
    0.0001f;

__declspec(naked)
bool CGameScriptInterface::IsHeroRenownLevelFull() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIRenownFull_Manager::GetPlayer
        mov ecx, eax
        call CGSIRenownFull_Player::GetHero
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
        call CGSIRenownFull_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 4
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov eax, dword ptr [eax + 4]
        pop esi
        jmp compare_percentage

fallback_component:
        mov eax, dword ptr [esp]
compare_percentage:
        mov ecx, eax
        call CGSIRenownFull_Component::GetPercentageThroughRenownLevel
        fld dword ptr [g_CGSIRenownFull_One]
        fsub dword ptr [g_CGSIRenownFull_Tolerance]
        fcompp
        fnstsw ax
        test ah, 5
        jp not_full
        mov eax, 1
        pop ecx
        ret

not_full:
        xor eax, eax
        pop ecx
        ret

invalid_hero:
        xor al, al
        pop ecx
        ret
    }
}
