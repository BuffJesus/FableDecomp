class CGSIExperience_Player;
class CGSIExperience_Node;

class CGSIExperience_Manager
{
public:
    CGSIExperience_Player* GetPlayer();
};

class CGSIExperience_Player
{
public:
    void* GetHero();
};

class CGSIExperience_Container
{
public:
    CGSIExperience_Node* LowerBound(const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual long GetHeroExperienceAvailableToSpend() const;
};

__declspec(naked)
long CGameScriptInterface::
    GetHeroExperienceAvailableToSpend() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIExperience_Manager::GetPlayer
        mov ecx, eax
        call CGSIExperience_Player::GetHero
        test eax, eax
        je failed
        test byte ptr [eax + 91h], 1
        jne failed

        mov ecx, dword ptr [eax + 2Ch]
        test ch, 1
        je fallback_value

        push esi
        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 68h
        call CGSIExperience_Container::LowerBound
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
        jmp read_value

fallback_value:
        mov eax, dword ptr [esp]
read_value:
        test eax, eax
        je failed
        mov eax, dword ptr [eax + 14h]
        pop ecx
        ret

failed:
        xor eax, eax
        pop ecx
        ret
    }
}
