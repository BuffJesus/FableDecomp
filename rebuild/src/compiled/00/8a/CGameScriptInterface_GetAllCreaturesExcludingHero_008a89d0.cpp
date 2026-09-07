// CGameScriptInterface::GetAllCreaturesExcludingHero @ 0x008a89d0
// Exact byte-led vector bridge; the specialized search policy remains opaque.

extern "C" void Script_GetCreaturesExcludingHero_0052b270();
extern "C" void ScriptThingVector_Reserve_008acd30();
extern "C" void ScriptThing_FromCreature_004c7e50();
extern "C" void ScriptThingVector_Insert_008ad5f0();
extern "C" void ScriptThing_Reset_0099a2e0();
extern "C" void ScriptThingRef_Free_00bfe9bc();
extern "C" void CreaturePointerArray_Free_00bfea14();
extern "C" unsigned long ScriptThing_Vftable_01238c8c;

extern "C" __declspec(naked) long __fastcall
CGameScriptInterface_GetAllCreaturesExcludingHero_008a89d0(void* self, void*, void* out)
{
    __asm {
        sub esp, 0ch
        push ebx
        push ebp
        push esi
        push edi
        lea eax, [esp+10h]
        push eax
        mov eax, [ecx+4]
        mov ecx, [eax+20h]
        xor ebx, ebx
        lea edx, [esp+24h]
        push edx
        mov [esp+18h], ebx
        mov [esp+1ch], ebx
        mov [esp+20h], ebx
        call Script_GetCreaturesExcludingHero_0052b270
        mov edi, [esp+14h]
        mov ebp, [esp+10h]
        mov esi, [esp+20h]
        sub edi, ebp
        sar edi, 2
        push edi
        mov ecx, esi
        call ScriptThingVector_Reserve_008acd30
        test edi, edi
        jbe loop_done
        jmp loop_body
        // Retail's three-byte loop-alignment NOP: lea ecx,[ecx+0].
        _emit 08dh
        _emit 049h
        _emit 000h
loop_body:
        mov ecx, [ebp+ebx*4]
        lea edx, [esp+10h]
        push edx
        call ScriptThing_FromCreature_004c7e50
        mov ecx, [esi+4]
        cmp ecx, [esi+8]
        je grow_output
        test ecx, ecx
        je append_done
        mov dword ptr [ecx], offset ScriptThing_Vftable_01238c8c
        mov edx, [eax+4]
        mov [ecx+4], edx
        mov eax, [eax+8]
        test eax, eax
        mov [ecx+8], eax
        je append_done
        inc dword ptr [eax]
append_done:
        add dword ptr [esi+4], 0ch
        jmp release_temporary
grow_output:
        push 1
        push 1
        lea edx, [esp+28h]
        push edx
        push eax
        push ecx
        mov ecx, esi
        call ScriptThingVector_Insert_008ad5f0
release_temporary:
        mov eax, [esp+18h]
        test eax, eax
        mov dword ptr [esp+10h], offset ScriptThing_Vftable_01238c8c
        je reset_temporary
        dec dword ptr [eax]
        mov eax, [esp+18h]
        cmp dword ptr [eax], 0
        jne reset_temporary
        mov ecx, [eax+8]
        call dword ptr [eax+4]
        mov eax, [esp+18h]
        push eax
        call ScriptThingRef_Free_00bfe9bc
        add esp, 4
reset_temporary:
        xor eax, eax
        lea ecx, [esp+10h]
        mov [esp+14h], eax
        mov [esp+18h], eax
        call ScriptThing_Reset_0099a2e0
        inc ebx
        cmp ebx, edi
        jb loop_body
loop_done:
        mov eax, [esi]
        mov ecx, [esi+4]
        sub ecx, eax
        mov eax, 2aaaaaabh
        imul ecx
        sar edx, 1
        mov esi, edx
        shr esi, 1fh
        add esi, edx
        test ebp, ebp
        je free_done
        push ebp
        call CreaturePointerArray_Free_00bfea14
        add esp, 4
free_done:
        pop edi
        mov eax, esi
        pop esi
        pop ebp
        pop ebx
        add esp, 0ch
        ret 4
    }
}