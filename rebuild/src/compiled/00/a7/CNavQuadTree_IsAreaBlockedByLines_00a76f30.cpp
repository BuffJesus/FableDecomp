// CNavQuadTree::IsAreaBlockedByLines @ 0x00a76f30
// Exact x87/tolerance/list traversal reconstructed directly from retail.

extern "C" void NavBox_ContainsPoint_00535cd0();
extern "C" void NavLine_IntersectsBox_00a57bd0();
extern "C" float NavLine_Epsilon_0129ba3c;

extern "C" __declspec(naked) bool __fastcall
CNavQuadTree_IsAreaBlockedByLines_00a76f30(
    void* self, void*, const void* area, const void* lineLists)
{
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0fffffff8h
        sub esp, 1ch
        push ebx
        push esi
        mov esi, [ebp+0ch]
        mov edx, [esi]
        cmp edx, [esi+4]
        push edi
        mov [esp+10h], edx
        je no_block
        mov edi, [ebp+8]
next_list:
        mov ecx, [edx]
        mov eax, [ecx]
        mov ebx, [eax]
        cmp ebx, eax
        mov [esp+14h], ecx
        je list_done
next_line:
        fld dword ptr [ebx+10h]
        lea esi, [ebx+8]
        fstp qword ptr [esp+18h]
        fld dword ptr [esi]
        fst qword ptr [esp+20h]
        fcomp qword ptr [esp+18h]
        fnstsw ax
        test ah, 5
        lea eax, [esp+18h]
        jnp x_max_ready
        lea eax, [esp+20h]
x_max_ready:
        fld dword ptr [NavLine_Epsilon_0129ba3c]
        fchs
        fstp dword ptr [esp+0ch]
        fld qword ptr [eax]
        fsub dword ptr [edi]
        fcomp dword ptr [esp+0ch]
        fnstsw ax
        test ah, 41h
        jnp advance_line
        fld dword ptr [esi+8]
        fst qword ptr [esp+20h]
        fld dword ptr [esi]
        fstp qword ptr [esp+18h]
        fcomp qword ptr [esp+18h]
        fnstsw ax
        test ah, 5
        lea eax, [esp+20h]
        jnp x_min_ready
        lea eax, [esp+18h]
x_min_ready:
        fld qword ptr [eax]
        fsub dword ptr [edi+8]
        fcomp dword ptr [NavLine_Epsilon_0129ba3c]
        fnstsw ax
        test ah, 1
        je advance_line
        fld dword ptr [esi+0ch]
        fstp qword ptr [esp+18h]
        fld dword ptr [esi+4]
        fst qword ptr [esp+20h]
        fcomp qword ptr [esp+18h]
        fnstsw ax
        test ah, 5
        lea eax, [esp+18h]
        jnp y_max_ready
        lea eax, [esp+20h]
y_max_ready:
        fld qword ptr [eax]
        fsub dword ptr [edi+4]
        fcomp dword ptr [esp+0ch]
        fnstsw ax
        test ah, 41h
        jnp advance_line
        fld dword ptr [esi+0ch]
        fst qword ptr [esp+20h]
        fld dword ptr [esi+4]
        fstp qword ptr [esp+18h]
        fcomp qword ptr [esp+18h]
        fnstsw ax
        test ah, 5
        lea eax, [esp+20h]
        jnp y_min_ready
        lea eax, [esp+18h]
y_min_ready:
        fld qword ptr [eax]
        fsub dword ptr [edi+0ch]
        fcomp dword ptr [NavLine_Epsilon_0129ba3c]
        fnstsw ax
        test ah, 1
        je advance_line
        push esi
        mov ecx, edi
        call NavBox_ContainsPoint_00535cd0
        test al, al
        jne blocked
        lea eax, [esi+8]
        push eax
        mov ecx, edi
        call NavBox_ContainsPoint_00535cd0
        test al, al
        jne blocked
        push edi
        mov ecx, esi
        call NavLine_IntersectsBox_00a57bd0
        test al, al
        jne blocked
        mov ecx, [esp+14h]
        mov edx, [esp+10h]
advance_line:
        mov ebx, [ebx]
        cmp ebx, [ecx]
        jne next_line
        mov esi, [ebp+0ch]
list_done:
        mov eax, [esi+4]
        add edx, 4
        cmp edx, eax
        mov [esp+10h], edx
        jne next_list
no_block:
        xor al, al
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret 8
blocked:
        pop edi
        pop esi
        mov al, 1
        pop ebx
        mov esp, ebp
        pop ebp
        ret 8
    }
}