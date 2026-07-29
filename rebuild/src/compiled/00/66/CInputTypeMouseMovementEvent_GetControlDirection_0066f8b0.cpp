extern "C" void* __fastcall
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_GetMovement(
    void* event, void*, void* result);
extern "C" float
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_Scale;
extern "C" float
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_FlipY;
extern "C" float
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_Unit;

extern "C" __declspec(naked) void* __fastcall
CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0(
    void* input_type, void*, void* result, const void* event)
{
    __asm
    {
        sub esp, 0ch
        mov ecx, [esp + 14h]
        lea eax, [esp]
        push eax
        call CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_GetMovement
        fld dword ptr [eax]
        fmul dword ptr [CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_Scale]
        mov ecx, [eax + 4]
        mov [esp + 4], ecx
        fld dword ptr [esp + 4]
        fmul dword ptr [CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_Scale]
        fmul dword ptr [CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_FlipY]
        fld st(1)
        fmul st(0), st(2)
        fld st(1)
        fmul st(0), st(2)
        faddp st(1), st(0)
        fsqrt
        fcom dword ptr [CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_Unit]
        fnstsw ax
        test ah, 41h
        jne store_unscaled
        fdivr dword ptr [CInputTypeMouseMovementEvent_GetControlDirection_0066f8b0_Unit]
        mov eax, [esp + 10h]
        fld st(0)
        fmulp st(3), st(0)
        fmulp st(1), st(0)
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax + 4]
        add esp, 0ch
        ret 8

    store_unscaled:
        mov eax, [esp + 10h]
        fstp st(0)
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax + 4]
        add esp, 0ch
        ret 8
    }
}
