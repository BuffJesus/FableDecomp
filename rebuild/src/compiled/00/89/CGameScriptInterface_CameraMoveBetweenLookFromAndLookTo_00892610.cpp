class C3DVector;

extern long CGSICameraLookFromTo_Scale;
extern unsigned char CGSICameraLookFromTo_Callback;

void* __cdecl CGSICameraLookFromTo_Allocate(unsigned long size);
long __cdecl CGSICameraLookFromTo_FloatToLong();

class CGSICameraLookFromTo_Interpolator
{
public:
    CGSICameraLookFromTo_Interpolator* Construct(
        const C3DVector& thirdPosition,
        const C3DVector& firstPosition,
        const C3DVector& fourthPosition,
        const C3DVector& secondPosition,
        long scaledDuration);
};

class CGSICameraLookFromTo_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraLookFromTo_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveBetweenLookFromAndLookTo(
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        const C3DVector& thirdPosition,
        const C3DVector& fourthPosition,
        float duration) const;
};

__declspec(naked)
void CGameScriptInterface::CameraMoveBetweenLookFromAndLookTo(
    const C3DVector& firstPosition,
    const C3DVector& secondPosition,
    const C3DVector& thirdPosition,
    const C3DVector& fourthPosition,
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

        push 80h
        call CGSICameraLookFromTo_Allocate
        mov esi, eax
        add esp, 4
        test esi, esi
        je no_interpolator_storage
        fild dword ptr [CGSICameraLookFromTo_Scale]
        fmul dword ptr [esp + 1Ch]
        call CGSICameraLookFromTo_FloatToLong
        mov ecx, dword ptr [esp + 10h]
        mov edx, dword ptr [esp + 18h]
        push eax
        mov eax, dword ptr [esp + 10h]
        push ecx
        mov ecx, dword ptr [esp + 1Ch]
        push edx
        push eax
        push ecx
        mov ecx, esi
        call CGSICameraLookFromTo_Interpolator::Construct
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
        call CGSICameraLookFromTo_Allocate
        add esp, 4
        test eax, eax
        je no_callback_storage
        mov ecx, dword ptr [esi]
        mov dword ptr [eax], 1
        mov dword ptr [eax + 4], offset CGSICameraLookFromTo_Callback
        mov dword ptr [eax + 8], ecx
        mov dword ptr [esi + 4], eax
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraLookFromTo_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 14h

no_callback_storage:
        xor eax, eax
        mov dword ptr [esi + 4], eax
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraLookFromTo_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 14h

no_interpolator:
        mov dword ptr [esi + 4], 0
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraLookFromTo_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 14h
    }
}
