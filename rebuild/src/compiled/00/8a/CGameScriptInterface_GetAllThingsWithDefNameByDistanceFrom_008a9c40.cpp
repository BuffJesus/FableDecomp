// CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom @ 0x008a9c40
// Exact byte-led search/sort/vector coordinator with neutral specialized seams.

extern "C" void Script_GetDefinitionManagerForName_0044c6b0();
extern "C" void Script_GetDefinitionIndexByName_009ad410();
extern "C" void Script_SearchThingsByDefinition_008adba0();
extern "C" void ScriptThingVector_Reserve_008acd30();
extern "C" void Script_SortThingsByDistance_007a4260();
extern "C" void ScriptThing_FromThing_004c7e50();
extern "C" void ScriptThingVector_Insert_008ad5f0();
extern "C" void ScriptThing_Reset_0099a2e0();
extern "C" void ScriptThingRef_Free_00bfe9bc();
extern "C" void ThingPointerArray_Free_00bfea14();
extern "C" unsigned long ScriptThing_Vftable_01238c8c;

extern "C" __declspec(naked) long __fastcall
CGameScriptInterface_GetAllThingsWithDefNameByDistanceFrom_008a9c40(
    void* self, void*, void* fromThing, void* definitionName, void* out)
{
    __asm {
        sub esp, 0ch
        mov eax, [esp+14h]
        push ebx
        push ebp
        push esi
        push edi
        xor edi, edi
        push eax
        mov esi, ecx
        mov [esp+14h], edi
        mov [esp+18h], edi
        mov [esp+1ch], edi
        call Script_GetDefinitionManagerForName_0044c6b0
        mov ecx, eax
        call Script_GetDefinitionIndexByName_009ad410
        mov [esp+24h], eax
        mov eax, [esi+4]
        lea ecx, [esp+10h]
        push ecx
        mov ecx, [eax+20h]
        lea edx, [esp+28h]
        push edx
        call Script_SearchThingsByDefinition_008adba0
        mov ebx, [esp+10h]
        mov ebp, [esp+14h]
        cmp ebx, ebp
        jne have_results
        cmp ebx, edi
        je empty_return
        push ebx
        call ThingPointerArray_Free_00bfea14
        add esp, 4
empty_return:
        xor eax, eax
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 0ch
        ret 0ch
have_results:
        mov esi, [esp+28h]
        mov ecx, ebp
        sub ecx, ebx
        sar ecx, 2
        push ecx
        mov ecx, esi
        call ScriptThingVector_Reserve_008acd30
        mov ecx, [esp+20h]
        mov edx, [ecx]
        call dword ptr [edx+18h]
        mov edx, [eax]
        sub esp, 0ch
        mov ecx, esp
        mov [ecx], edx
        mov edx, [eax+4]
        mov [ecx+4], edx
        mov eax, [eax+8]
        mov [ecx+8], eax
        mov edx, ebp
        mov ecx, ebx
        call Script_SortThingsByDistance_007a4260
        mov edi, ebx
loop_body:
        mov ecx, [edi]
        lea edx, [esp+10h]
        push edx
        call ScriptThing_FromThing_004c7e50
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
        lea edx, [esp+2ch]
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
        add edi, 4
        cmp edi, ebp
        jne loop_body
        mov eax, [esi]
        mov ecx, [esi+4]
        sub ecx, eax
        mov eax, 2aaaaaabh
        imul ecx
        sar edx, 1
        mov esi, edx
        shr esi, 1fh
        add esi, edx
        test ebx, ebx
        je free_done
        push ebx
        call ThingPointerArray_Free_00bfea14
        add esp, 4
free_done:
        pop edi
        mov eax, esi
        pop esi
        pop ebp
        pop ebx
        add esp, 0ch
        ret 0ch
    }
}