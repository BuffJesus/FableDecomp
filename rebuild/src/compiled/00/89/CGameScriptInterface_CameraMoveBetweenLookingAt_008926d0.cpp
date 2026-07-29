class C3DVector;
class CScriptThing;

extern float CGSICameraLookingAtThing_ClampThreshold;
extern long CGSICameraLookingAtThing_Scale;

void* __cdecl CGSICameraLookingAtThing_Allocate(unsigned long size);
long __cdecl CGSICameraLookingAtThing_FloatToLong();

class CGSICameraLookingAtThing_Interpolator
{
public:
    CGSICameraLookingAtThing_Interpolator* Construct(
        void* pTarget,
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        long scaledDuration,
        float blend);
};

class CGSICameraLookingAtThing_ModeArguments
{
public:
    void Construct(
        CGSICameraLookingAtThing_Interpolator* pInterpolator);
};

class CGSICameraLookingAtThing_Receiver
{
public:
    void SetScriptedCameraMode(
        CGSICameraLookingAtThing_Interpolator* pInterpolator,
        void* pCallbackState);
};

class CGameScriptInterface
{
public:
    virtual void CameraMoveBetweenLookingAt(
        const C3DVector& firstPosition,
        const C3DVector& secondPosition,
        const CScriptThing& target,
        float duration,
        float blend) const;
};

__declspec(naked)
void CGameScriptInterface::CameraMoveBetweenLookingAt(
    const C3DVector& firstPosition,
    const C3DVector& secondPosition,
    const CScriptThing& target,
    float duration,
    float blend) const
{
    __asm
    {
        push ebx
        push esi
        mov esi, dword ptr [esp + 14h]
        mov eax, dword ptr [esi]
        mov ebx, ecx
        push edi
        mov ecx, esi
        call dword ptr [eax + 12Ch]
        test al, al
        je finished

        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2Ch]
        mov edi, eax
        mov eax, dword ptr [ebx + 4]
        mov ecx, dword ptr [eax + 34h]
        mov edx, dword ptr [ecx]
        lea eax, [esp + 18h]
        push eax
        call dword ptr [edx + 24h]

        fld dword ptr [CGSICameraLookingAtThing_ClampThreshold]
        fld dword ptr [esp + 20h]
        fucompp
        fnstsw ax
        test ah, 44h
        jp blend_ready
        mov dword ptr [esp + 20h], 3E4CCCCDh
blend_ready:
        push 90h
        call CGSICameraLookingAtThing_Allocate
        mov esi, eax
        add esp, 4
        test esi, esi
        je no_interpolator_storage
        fild dword ptr [CGSICameraLookingAtThing_Scale]
        mov ecx, dword ptr [esp + 20h]
        push ecx
        fmul dword ptr [esp + 20h]
        call CGSICameraLookingAtThing_FloatToLong
        mov edx, dword ptr [esp + 18h]
        push eax
        mov eax, dword ptr [esp + 18h]
        push edx
        push eax
        push edi
        mov ecx, esi
        call CGSICameraLookingAtThing_Interpolator::Construct
        jmp have_interpolator
no_interpolator_storage:
        xor eax, eax
have_interpolator:
        sub esp, 8
        mov ecx, esp
        push eax
        call CGSICameraLookingAtThing_ModeArguments::Construct
        mov ecx, dword ptr [esp + 20h]
        call CGSICameraLookingAtThing_Receiver::SetScriptedCameraMode
finished:
        pop edi
        pop esi
        pop ebx
        ret 14h
    }
}
