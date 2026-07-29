class C3DVector;
class CScriptThing;

void* __cdecl CGSICameraMoveToThing_Allocate(unsigned long size);

class CGSICameraMoveToThing_Interpolator
{
public:
    CGSICameraMoveToThing_Interpolator* Construct(
        const C3DVector& position,
        void* pTarget);
};

class CGSICameraMoveToThing_ModeArguments
{
public:
    void Construct(
        CGSICameraMoveToThing_Interpolator* pInterpolator);
};

class CGSICameraMoveToThing_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraMoveToThing_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveToPosAndLookAtThing(
        const C3DVector& position,
        const CScriptThing& target,
        float duration) const;
};

__declspec(naked)
void CGameScriptInterface::CameraMoveToPosAndLookAtThing(
    const C3DVector& position,
    const CScriptThing& target,
    float duration) const
{
    __asm
    {
        push esi
        mov esi, dword ptr [esp + 0Ch]
        mov eax, dword ptr [esi]
        push edi
        mov edi, ecx
        mov ecx, esi
        call dword ptr [eax + 12Ch]
        test al, al
        je finished

        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2Ch]
        test byte ptr [eax + 6Ch], 4
        je finished

        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 2Ch]
        mov ecx, dword ptr [edi + 4]
        mov ecx, dword ptr [ecx + 34h]
        mov edx, dword ptr [ecx]
        mov esi, eax
        lea eax, [esp + 10h]
        push eax
        call dword ptr [edx + 24h]

        push 6Ch
        call CGSICameraMoveToThing_Allocate
        add esp, 4
        test eax, eax
        je no_interpolator_storage
        mov ecx, dword ptr [esp + 0Ch]
        push esi
        push ecx
        mov ecx, eax
        call CGSICameraMoveToThing_Interpolator::Construct
        jmp have_interpolator
no_interpolator_storage:
        xor eax, eax
have_interpolator:
        sub esp, 8
        mov ecx, esp
        push eax
        call CGSICameraMoveToThing_ModeArguments::Construct
        mov ecx, dword ptr [esp + 18h]
        call CGSICameraMoveToThing_Receiver::SetScriptedCameraMode
finished:
        pop edi
        pop esi
        ret 0Ch
    }
}
