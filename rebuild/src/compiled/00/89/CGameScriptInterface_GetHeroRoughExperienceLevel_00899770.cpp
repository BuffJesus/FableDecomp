class CGSIRoughExperience_Player;
class CGSIRoughExperience_Node;

class CGSIRoughExperience_Manager
{
public:
    CGSIRoughExperience_Player* GetPlayer();
};

class CGSIRoughExperience_Player
{
public:
    void* GetHero();
};

class CGSIRoughExperience_Container
{
public:
    CGSIRoughExperience_Node* LowerBound(
        const long* pKey);
};

class CGSIRoughExperience_Component
{
public:
    long GetRoughExperienceLevel();
};

class CGameScriptInterface
{
public:
    virtual long GetHeroRoughExperienceLevel() const;
};

__declspec(naked)
long CGameScriptInterface::GetHeroRoughExperienceLevel() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        call CGSIRoughExperience_Manager::GetPlayer
        mov ecx, eax
        call CGSIRoughExperience_Player::GetHero
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
        call CGSIRoughExperience_Container::LowerBound
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
        jmp query_component

fallback_component:
        mov eax, dword ptr [esp]
query_component:
        test eax, eax
        je failed
        mov ecx, eax
        call CGSIRoughExperience_Component::GetRoughExperienceLevel
        pop ecx
        ret

failed:
        xor eax, eax
        pop ecx
        ret
    }
}
