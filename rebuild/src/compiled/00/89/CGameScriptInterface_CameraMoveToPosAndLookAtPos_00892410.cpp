class C3DVector;

void* __cdecl CGSICameraMove_Allocate(unsigned long size);
extern unsigned char CGSICameraMove_Callback;

class CGSICameraMove_Interpolator
{
public:
    CGSICameraMove_Interpolator* Construct(
        const C3DVector& position,
        const C3DVector& lookAtPosition);
};

class CGSICameraMove_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraMove_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveToPosAndLookAtPos(
        const C3DVector& position,
        const C3DVector& lookAtPosition,
        float duration) const;
};

__declspec(naked)
void CGameScriptInterface::CameraMoveToPosAndLookAtPos(
    const C3DVector& position,
    const C3DVector& lookAtPosition,
    float duration) const
{
    __asm
    {
        push ecx
        mov eax, dword ptr [ecx + 4]
        mov ecx, dword ptr [eax + 34h]
        mov edx, dword ptr [ecx]
        push esi
        lea eax, [esp + 4]
        push eax
        call dword ptr [edx + 24h]

        push 6Ch
        call CGSICameraMove_Allocate
        add esp, 4
        test eax, eax
        je no_interpolator_storage
        mov ecx, dword ptr [esp + 10h]
        mov edx, dword ptr [esp + 0Ch]
        push ecx
        push edx
        mov ecx, eax
        call CGSICameraMove_Interpolator::Construct
        jmp have_interpolator
no_interpolator_storage:
        xor eax, eax
have_interpolator:
        sub esp, 8
        test eax, eax
        mov esi, esp
        mov dword ptr [esi], eax
        je no_interpolator

        push 0Ch
        call CGSICameraMove_Allocate
        add esp, 4
        test eax, eax
        je no_callback_storage
        mov ecx, dword ptr [esi]
        mov dword ptr [eax], 1
        mov dword ptr [eax + 4], offset CGSICameraMove_Callback
        mov dword ptr [eax + 8], ecx
        mov dword ptr [esi + 4], eax
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraMove_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 0Ch

no_callback_storage:
        xor eax, eax
        mov dword ptr [esi + 4], eax
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraMove_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 0Ch

no_interpolator:
        mov dword ptr [esi + 4], 0
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraMove_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 0Ch
    }
}
