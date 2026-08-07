// NUISystem::CTable::Initialise @ 0x00550C60
//
// The body is transcribed from the retail executable.  The four calls which
// leave this compilation unit are named seams for the already recovered
// changing-state base initializer, tree destruction, free, and operator
// delete routines; the component/vtable traversal remains the retail body.

extern "C" void __fastcall FableCTableInitialiseBase(void* self);
extern "C" void __fastcall FableCTableInitialiseDestroyTree(
    void* treeOwner,
    void* treeRoot);
extern "C" void __cdecl FableCTableInitialiseFree(void* memory);
extern "C" void __cdecl FableCTableInitialiseDelete(void* object);

extern "C" __declspec(naked) void __fastcall
CTable_Initialise_00550c60(void*)
{
    __asm
    {
        push ebx
        push esi
        mov esi,ecx
        mov eax,dword ptr [esi+168h]
        xor ebx,ebx
        cmp eax,ebx
        je done
        cmp dword ptr [esi+16ch],ebx
        je done
        push edi
        call FableCTableInitialiseBase
        mov eax,dword ptr [esi]
        mov ecx,esi
        call dword ptr [eax+238h]
        mov eax,dword ptr [esi+160h]
        cmp eax,ebx
        lea edi,[esi+15ch]
        je tree_done
        mov ecx,dword ptr [edi]
        mov edx,dword ptr [ecx+4]
        push edx
        mov ecx,edi
        call FableCTableInitialiseDestroyTree
        mov eax,dword ptr [edi]
        mov dword ptr [eax+8],eax
        mov eax,dword ptr [edi]
        mov dword ptr [eax+4],ebx
        mov eax,dword ptr [edi]
        mov dword ptr [eax+0ch],eax
        mov dword ptr [edi+4],ebx
tree_done:
        mov edi,dword ptr [esi+168h]
        cmp edi,ebx
        je second_vector
        mov eax,dword ptr [edi]
        cmp eax,ebx
        je first_vector_delete
        push eax
        call FableCTableInitialiseFree
        add esp,4
first_vector_delete:
        push edi
        call FableCTableInitialiseDelete
        add esp,4
second_vector:
        mov edi,dword ptr [esi+16ch]
        cmp edi,ebx
        mov dword ptr [esi+168h],ebx
        je second_vector_done
        mov eax,dword ptr [edi]
        cmp eax,ebx
        je second_vector_delete
        push eax
        call FableCTableInitialiseFree
        add esp,4
second_vector_delete:
        push edi
        call FableCTableInitialiseDelete
        add esp,4
second_vector_done:
        mov dword ptr [esi+16ch],ebx
        mov ecx,dword ptr [esi+0b4h]
        sub ecx,dword ptr [esi+0b0h]
        sar ecx,3
        xor edi,edi
        test ecx,ecx
        jbe children_done
children_loop:
        mov edx,dword ptr [esi+0b0h]
        mov ecx,dword ptr [edx+edi*8]
        mov eax,dword ptr [ecx]
        push esi
        call dword ptr [eax+0cch]
        mov ecx,dword ptr [esi+0b4h]
        sub ecx,dword ptr [esi+0b0h]
        inc edi
        sar ecx,3
        cmp edi,ecx
        jb children_loop
children_done:
        pop edi
done:
        pop esi
        pop ebx
        ret
    }
}
