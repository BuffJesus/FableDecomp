class C3DVector;

extern float CGSICameraBetweenLookingAt_ClampThreshold;
extern long CGSICameraBetweenLookingAt_Scale;
extern unsigned char CGSICameraBetweenLookingAt_Callback;

void* __cdecl CGSICameraBetweenLookingAt_Allocate(unsigned long size);
long __cdecl CGSICameraBetweenLookingAt_FloatToLong();

class CGSICameraBetweenLookingAt_Interpolator
{
public:
    CGSICameraBetweenLookingAt_Interpolator* Construct(
        const C3DVector& thirdPosition,
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        long scaledDuration,
        float blend);
};

class CGSICameraBetweenLookingAt_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraBetweenLookingAt_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveBetweenLookingAt(
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        const C3DVector& thirdPosition,
        float duration,
        float blend) const;
};

__declspec(naked)
void CGameScriptInterface::CameraMoveBetweenLookingAt(
    const C3DVector& firstPosition,
    const C3DVector& secondPosition,
    const C3DVector& thirdPosition,
    float duration,
    float blend) const
{
    __asm
    {
        push ecx
        fld dword ptr [CGSICameraBetweenLookingAt_ClampThreshold]
        push esi
        fld dword ptr [esp + 1Ch]
        fucompp
        fnstsw ax
        test ah, 44h
        jp blend_ready
        mov dword ptr [esp + 1Ch], 3E4CCCCDh
blend_ready:
        mov eax, dword ptr [ecx + 4]
        mov ecx, dword ptr [eax + 34h]
        mov edx, dword ptr [ecx]
        lea eax, [esp + 4]
        push eax
        call dword ptr [edx + 24h]

        push 90h
        call CGSICameraBetweenLookingAt_Allocate
        mov esi, eax
        add esp, 4
        test esi, esi
        je no_interpolator_storage
        fild dword ptr [CGSICameraBetweenLookingAt_Scale]
        mov ecx, dword ptr [esp + 1Ch]
        push ecx
        fmul dword ptr [esp + 1Ch]
        call CGSICameraBetweenLookingAt_FloatToLong
        mov edx, dword ptr [esp + 14h]
        mov ecx, dword ptr [esp + 18h]
        push eax
        mov eax, dword ptr [esp + 14h]
        push edx
        push eax
        push ecx
        mov ecx, esi
        call CGSICameraBetweenLookingAt_Interpolator::Construct
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
        call CGSICameraBetweenLookingAt_Allocate
        add esp, 4
        test eax, eax
        je no_callback_storage
        mov ecx, dword ptr [esi]
        mov dword ptr [eax], 1
        mov dword ptr [eax + 4], offset CGSICameraBetweenLookingAt_Callback
        mov dword ptr [eax + 8], ecx
        mov dword ptr [esi + 4], eax
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraBetweenLookingAt_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 14h

no_callback_storage:
        xor eax, eax
        mov dword ptr [esi + 4], eax
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraBetweenLookingAt_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 14h

no_interpolator:
        mov dword ptr [esi + 4], 0
        mov ecx, dword ptr [esp + 0Ch]
        call CGSICameraBetweenLookingAt_Receiver::SetScriptedCameraMode
        pop esi
        pop ecx
        ret 14h
    }
}
