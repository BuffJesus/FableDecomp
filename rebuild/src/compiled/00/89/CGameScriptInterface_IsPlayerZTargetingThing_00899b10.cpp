class CScriptThing
{
};

class CGSIZTarget_Player;
class CGSIZTarget_Node;

class CGSIZTarget_Manager
{
public:
    CGSIZTarget_Player* GetPlayer();
};

class CGSIZTarget_Player
{
public:
    void* GetHero();
};

class CGSIZTarget_Container
{
public:
    CGSIZTarget_Node* LowerBound(
        const long* pKey);
};

class CGameScriptInterface
{
public:
    virtual bool IsPlayerZTargetingThing(
        const CScriptThing& thing) const;
};

__declspec(naked)
bool CGameScriptInterface::IsPlayerZTargetingThing(
    const CScriptThing& thing) const
{
    __asm
    {
        mov ecx, dword ptr [ecx + 14h]
        push esi
        push edi
        call CGSIZTarget_Manager::GetPlayer
        mov ecx, dword ptr [esp + 0Ch]
        mov esi, eax
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 2Ch]
        mov edi, eax
        mov al, byte ptr [esi + 20Ah]
        test al, al
        je failed
        test edi, edi
        je failed
        test byte ptr [edi + 91h], 1
        jne failed

        mov ecx, esi
        call CGSIZTarget_Player::GetHero
        test eax, eax
        je failed
        test byte ptr [eax + 91h], 1
        jne failed
        mov ecx, dword ptr [eax + 20h]
        test ch, 1
        je failed

        lea ecx, [esp + 0Ch]
        lea esi, [eax + 44h]
        push ecx
        mov ecx, esi
        mov dword ptr [esp + 10h], 8
        call CGSIZTarget_Container::LowerBound
        mov esi, dword ptr [esi + 4]
        cmp eax, esi
        je use_sentinel
        cmp dword ptr [eax], 8
        jle have_component
use_sentinel:
        mov eax, esi
have_component:
        mov ecx, dword ptr [eax + 4]
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 84h]
        cmp eax, edi
        jne failed
        pop edi
        mov al, 1
        pop esi
        ret 4

failed:
        pop edi
        xor al, al
        pop esi
        ret 4
    }
}
