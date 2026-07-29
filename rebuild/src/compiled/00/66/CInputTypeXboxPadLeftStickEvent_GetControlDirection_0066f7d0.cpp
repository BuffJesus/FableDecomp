extern "C" void* __fastcall
CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_GetPosition(
    void* event, void*, void* result);
extern "C" float
CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_FlipY;
extern "C" float
CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_Epsilon;
extern "C" float
CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_Unit;

extern "C" __declspec(naked) void* __fastcall
CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0(
    void* input_type, void*, void* result, const void* event)
{
    __asm
    {
        sub esp, 8
        mov ecx, [esp + 10h]
        lea eax, [esp]
        push eax
        call CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_GetPosition
        fld dword ptr [esp + 4]
        fmul dword ptr [CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_FlipY]
        fld dword ptr [esp]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fsqrt
        fst dword ptr [esp + 10h]
        fcomp dword ptr [CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_Epsilon]
        fnstsw ax
        test ah, 41h
        jne store_result
        fld dword ptr [esp + 10h]
        fcomp dword ptr [CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_Unit]
        fnstsw ax
        test ah, 41h
        jne store_result
        fld dword ptr [CInputTypeXboxPadLeftStickEvent_GetControlDirection_0066f7d0_Unit]
        fdiv dword ptr [esp + 10h]
        fld st(0)
        fmulp st(2), st(0)
        fmulp st(2), st(0)

    store_result:
        mov eax, [esp + 0ch]
        fstp dword ptr [eax]
        fstp dword ptr [eax + 4]
        add esp, 8
        ret 8
    }
}
