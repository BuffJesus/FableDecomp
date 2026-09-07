// CGameScriptInterface::GetAllCreaturesInAreaWithScriptName @ 0x008a86c0
// Exact byte-led rough-area/search/vector coordinator.

extern "C" void Script_FloatToLong_00bfea70();
extern "C" void Script_StringCopyCtor_0099ec30();
extern "C" void Script_StringDtor_0099eae0();
extern "C" void Script_StringEquals_004115a0();
extern "C" void Script_SearchNamedCreatures_008ad950();
extern "C" void Script_SearchPotentialCustomers_008ad710();
extern "C" void ScriptThingVector_Reserve_008acd30();
extern "C" void ScriptThing_FromCreature_004c7e50();
extern "C" void ScriptThingVector_Insert_008ad5f0();
extern "C" void ScriptThing_Reset_0099a2e0();
extern "C" void ScriptThingRef_Free_00bfe9bc();
extern "C" void CreaturePointerArray_Free_00bfea14();
extern "C" unsigned long ScriptThing_Vftable_01238c8c;
extern "C" char Script_EmptyString_0122d70e;

extern "C" __declspec(naked) long __fastcall
CGameScriptInterface_GetAllCreaturesInAreaWithScriptName_008a86c0(
    void* self, void*, void* scriptName, void* position, float radius, void* out)
{
    __asm {
        sub esp, 1ch
        push ebx
        push ebp
        push esi
        push edi
        mov edi, [esp+34h]
        fld dword ptr [edi]
        mov ebp, ecx
        call Script_FloatToLong_00bfea70
        fld dword ptr [edi+4]
        mov esi, eax
        call Script_FloatToLong_00bfea70
        fld dword ptr [esp+38h]
        mov edi, eax
        call Script_FloatToLong_00bfea70
        mov ebx, [esp+30h]
        mov ecx, esi
        sub ecx, eax
        mov edx, edi
        sub edx, eax
        add esi, eax
        add edi, eax
        xor eax, eax
        mov [esp+1ch], ecx
        mov ecx, [ebx]
        cmp ecx, eax
        mov [esp+20h], edx
        mov [esp+24h], esi
        mov [esp+28h], edi
        mov [esp+10h], eax
        mov [esp+14h], eax
        mov [esp+18h], eax
        jne string_object
        mov edi, offset Script_EmptyString_0122d70e
        mov esi, edi
        mov ecx, 1
        xor eax, eax
        repe cmps byte ptr [esi], byte ptr [edi]
        je empty_name_search
named_search:
        push ebx
        lea ecx, [esp+38h]
        call Script_StringCopyCtor_0099ec30
        lea edx, [esp+10h]
        push edx
        mov edx, [ebp+4]
        lea eax, [esp+38h]
        push eax
        lea ecx, [esp+24h]
        push ecx
        mov ecx, [edx+20h]
        call Script_SearchNamedCreatures_008ad950
        lea ecx, [esp+34h]
        call Script_StringDtor_0099eae0
search_complete:
        mov ebx, [esp+10h]
        mov ebp, [esp+14h]
        cmp ebx, ebp
        jne have_results
        test ebx, ebx
        je empty_return
        push ebx
        call CreaturePointerArray_Free_00bfea14
        add esp, 4
empty_return:
        pop edi
        pop esi
        pop ebp
        xor eax, eax
        pop ebx
        add esp, 1ch
        ret 10h
string_object:
        push offset Script_EmptyString_0122d70e
        call Script_StringEquals_004115a0
        test al, al
        je named_search
empty_name_search:
        lea ecx, [esp+10h]
        push ecx
        mov ecx, [ebp+4]
        mov ecx, [ecx+20h]
        lea edx, [esp+38h]
        push edx
        lea eax, [esp+24h]
        push eax
        call Script_SearchPotentialCustomers_008ad710
        jmp search_complete
have_results:
        mov esi, [esp+3ch]
        mov eax, ebp
        sub eax, ebx
        sar eax, 2
        push eax
        mov ecx, esi
        call ScriptThingVector_Reserve_008acd30
        mov edi, ebx
loop_body:
        mov ecx, [edi]
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
        lea edx, [esp+3ch]
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
        call CreaturePointerArray_Free_00bfea14
        add esp, 4
free_done:
        pop edi
        mov eax, esi
        pop esi
        pop ebp
        pop ebx
        add esp, 1ch
        ret 10h
    }
}