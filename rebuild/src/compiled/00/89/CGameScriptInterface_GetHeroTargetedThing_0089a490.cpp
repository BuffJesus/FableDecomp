class CScriptThing
{
public:
    CScriptThing();
    void Construct();

private:
    unsigned long m_Data[3];
};

class CGSIHeroTarget_Player;
class CGSIHeroTarget_Node;

class CGSIHeroTarget_Manager
{
public:
    CGSIHeroTarget_Player* GetPlayer();
};

class CGSIHeroTarget_Player
{
public:
    void* GetHero();
};

class CGSIHeroTarget_Container
{
public:
    CGSIHeroTarget_Node* LowerBound(const long* pKey);
};

class CGSIHeroTarget_Thing
{
public:
    CScriptThing BuildScriptThing();
};

class CGameScriptInterface
{
public:
    virtual CScriptThing GetHeroTargetedThing() const;
};

__declspec(naked)
CScriptThing CGameScriptInterface::GetHeroTargetedThing() const
{
    __asm
    {
        push ecx
        mov ecx, dword ptr [ecx + 14h]
        push esi
        call CGSIHeroTarget_Manager::GetPlayer
        mov ecx, eax
        call CGSIHeroTarget_Player::GetHero
        test eax, eax
        je empty_result

        mov ecx, dword ptr [eax + 20h]
        test ch, 1
        je resolve_target

        lea esi, [eax + 44h]
        lea eax, [esp + 4]
        push eax
        mov ecx, esi
        mov dword ptr [esp + 8], 8
        call CGSIHeroTarget_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 8
        jle have_provider
use_sentinel:
        mov eax, esi
have_provider:
        mov ecx, dword ptr [eax + 4]
        jmp call_provider

resolve_target:
        mov ecx, dword ptr [esp + 0Ch]
call_provider:
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 84h]
        test eax, eax
        je empty_result

        mov esi, dword ptr [esp + 0Ch]
        push esi
        mov ecx, eax
        call CGSIHeroTarget_Thing::BuildScriptThing
        mov eax, esi
        pop esi
        pop ecx
        ret 4

empty_result:
        mov esi, dword ptr [esp + 0Ch]
        mov ecx, esi
        call CScriptThing::Construct
        mov eax, esi
        pop esi
        pop ecx
        ret 4
    }
}
