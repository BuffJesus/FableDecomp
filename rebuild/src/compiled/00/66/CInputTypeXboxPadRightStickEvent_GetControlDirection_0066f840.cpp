extern "C" void* __fastcall
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_GetPosition(
    void* event, void*, void* result);
extern "C" float
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_FlipY;
extern "C" float
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_Epsilon;
extern "C" float
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_Unit;

extern "C" __declspec(naked) void* __fastcall
CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840(
    void* input_type, void*, void* result, const void* event)
{
    __asm
    {
        sub esp, 8
        mov ecx, [esp + 10h]
        lea eax, [esp]
        push eax
        call CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_GetPosition
        fld dword ptr [esp + 4]
        fmul dword ptr [CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_FlipY]
        fld dword ptr [esp]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fsqrt
        fst dword ptr [esp + 10h]
        fcomp dword ptr [CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_Epsilon]
        fnstsw ax
        test ah, 41h
        jne store_result
        fld dword ptr [esp + 10h]
        fcomp dword ptr [CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_Unit]
        fnstsw ax
        test ah, 41h
        jne store_result
        fld dword ptr [CInputTypeXboxPadRightStickEvent_GetControlDirection_0066f840_Unit]
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
