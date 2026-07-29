extern "C" void* CInputTypeMouseButtonEvent_0066f060_Table3[12];
extern "C" void* CInputTypeMouseButtonEvent_0066f060_Table2[12];
extern "C" void* CInputTypeMouseButtonEvent_0066f060_Table1[12];

extern "C" __declspec(naked) long __fastcall
CInputTypeMouseButtonEvent_GetEventTypeFromGeneralButtonType_0066f060(
    void* input_type, void*, long general_type)
{
    __asm
    {
        mov eax, [esp + 4]
        dec eax
        je general_one
        dec eax
        je general_two
        dec eax
        jne invalid

        mov ecx, [ecx + 4]
        lea eax, [ecx - 1]
        cmp eax, 0Bh
        ja invalid
        jmp dword ptr [eax * 4 + CInputTypeMouseButtonEvent_0066f060_Table3]

    return_6:
        mov eax, 6
        ret 4
    return_9:
        mov eax, 9
        ret 4
    return_12:
        mov eax, 0Ch
        ret 4
    return_32:
        mov eax, 20h
        ret 4
    return_33:
        mov eax, 21h
        ret 4
    return_34:
        mov eax, 22h
        ret 4
    return_35:
        mov eax, 23h
        ret 4
    return_36:
        mov eax, 24h
        ret 4

    general_two:
        mov ecx, [ecx + 4]
        lea eax, [ecx - 1]
        cmp eax, 0Bh
        ja invalid
        jmp dword ptr [eax * 4 + CInputTypeMouseButtonEvent_0066f060_Table2]

    return_5:
        mov eax, 5
        ret 4
    return_8:
        mov eax, 8
        ret 4
    return_11:
        mov eax, 0Bh
        ret 4
    return_27:
        mov eax, 1Bh
        ret 4
    return_28:
        mov eax, 1Ch
        ret 4
    return_29:
        mov eax, 1Dh
        ret 4
    return_30:
        mov eax, 1Eh
        ret 4
    return_31:
        mov eax, 1Fh
        ret 4

    general_one:
        mov ecx, [ecx + 4]
        lea eax, [ecx - 1]
        cmp eax, 0Bh
        ja invalid
        jmp dword ptr [eax * 4 + CInputTypeMouseButtonEvent_0066f060_Table1]

    return_4:
        mov eax, 4
        ret 4
    return_7:
        mov eax, 7
        ret 4
    return_10:
        mov eax, 0Ah
        ret 4
    return_22:
        mov eax, 16h
        ret 4
    return_23:
        mov eax, 17h
        ret 4
    return_24:
        mov eax, 18h
        ret 4
    return_25:
        mov eax, 19h
        ret 4
    return_26:
        mov eax, 1Ah
        ret 4

    invalid:
        xor eax, eax
        ret 4
    }
}
