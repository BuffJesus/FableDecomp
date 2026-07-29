extern "C" long __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetCategory(
    void* event, void*);
extern "C" long __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType(
    void* event, void*);

extern "C" __declspec(naked) unsigned char __fastcall
CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0(
    void* input_type, void*, const void* event)
{
    __asm
    {
        push esi
        mov esi, [esp + 8]
        push edi
        mov edi, ecx
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetCategory
        cmp eax, 3
        jne false_event

        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 4
        je button_one
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 5
        je button_one
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 6
        je button_one

        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 7
        je button_three
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 8
        je button_three
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 9
        je button_three

        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 0Ah
        je button_two
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 0Bh
        je button_two
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 0Ch
        je button_two

        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 16h
        jl second_range
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 1Ah
        jg second_range
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        mov edx, [edi + 4]
        sub eax, 0Eh
        xor ecx, ecx
        cmp edx, eax
        sete cl
        pop edi
        mov al, cl
        pop esi
        ret 4

    second_range:
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 1Bh
        jl third_range
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 1Fh
        jg third_range
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        mov ecx, [edi + 4]
        sub eax, 13h
        xor edx, edx
        cmp ecx, eax
        sete dl
        pop edi
        mov al, dl
        pop esi
        ret 4

    third_range:
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 20h
        jl false_event
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        cmp eax, 24h
        jg false_event
        mov ecx, esi
        call CInputTypeMouseButtonEvent_IsEventOfThisType_0066eee0_GetType
        mov edx, [edi + 4]
        sub eax, 18h
        xor ecx, ecx
        cmp edx, eax
        sete cl
        pop edi
        mov al, cl
        pop esi
        ret 4

    button_two:
        mov ecx, [edi + 4]
        xor eax, eax
        cmp ecx, 2
        pop edi
        sete al
        pop esi
        ret 4

    button_three:
        mov ecx, [edi + 4]
        xor eax, eax
        cmp ecx, 3
        pop edi
        sete al
        pop esi
        ret 4

    button_one:
        mov ecx, [edi + 4]
        xor eax, eax
        cmp ecx, 1
        pop edi
        sete al
        pop esi
        ret 4

    false_event:
        pop edi
        xor al, al
        pop esi
        ret 4
    }
}
