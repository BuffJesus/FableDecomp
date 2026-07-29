struct CScriptThingRecord
{
    void* m_pVTable;
    void* m_pData;
    unsigned long* m_pReference;
};

struct CGameScriptInterface { void** m_pVTable; };

extern "C" unsigned long g_ScriptThingVTable;
extern "C" float g_DefaultPosition[2];
extern "C" void* __fastcall Sub_InitScriptThing(void* self);
extern "C" void __fastcall Sub_DestroyScriptThing(void* self);
extern "C" void __cdecl Sub_Free(void* pMemory);
extern "C" long __cdecl Sub_RoundFloat();
extern "C" void __cdecl Sub_DeleteReference(void* pReference);

__declspec(naked)
CScriptThingRecord* __fastcall
CGameScriptInterface_GetFurthestWithScriptName(
    CGameScriptInterface* self,
    int edxUnused,
    CScriptThingRecord* pResult,
    CScriptThingRecord* pOrigin,
    const void* pScriptName)
{
    __asm
    {
        sub esp, 1Ch
        mov eax, dword ptr [ecx]
        push ebx
        push ebp
        push esi
        push edi
        lea edx, [esp + 14h]
        push edx
        mov edx, dword ptr [esp + 3Ch]
        xor ebp, ebp
        push edx
        mov dword ptr [esp + 1Ch], ebp
        mov dword ptr [esp + 20h], ebp
        mov dword ptr [esp + 24h], ebp
        call dword ptr [eax + 12Ch]
        mov esi, dword ptr [esp + 14h]
        cmp esi, dword ptr [esp + 18h]
        jne have_candidates

        mov edi, dword ptr [esp + 30h]
        mov ecx, edi
        call Sub_InitScriptThing
        mov ebx, dword ptr [esp + 18h]
        mov esi, dword ptr [esp + 14h]
        cmp esi, ebx
        mov dword ptr [edi], offset g_ScriptThingVTable
        mov dword ptr [edi + 4], ebp
        mov dword ptr [edi + 8], ebp
        je empty_destroyed
empty_destroy_loop:
        mov eax, dword ptr [esi]
        push ebp
        mov ecx, esi
        call dword ptr [eax]
        add esi, 0Ch
        cmp esi, ebx
        jne empty_destroy_loop
        mov esi, dword ptr [esp + 14h]
empty_destroyed:
        cmp esi, ebp
        je empty_freed
        push esi
        call Sub_Free
        add esp, 4
empty_freed:
        mov eax, edi
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 1Ch
        ret 0Ch

have_candidates:
        mov dword ptr [esp + 38h], ebp
        mov dword ptr [esp + 10h], esi
candidate_loop:
        mov eax, dword ptr [esi + 8]
        cmp eax, ebp
        mov ecx, dword ptr [esi + 4]
        mov ebx, offset g_ScriptThingVTable
        mov dword ptr [esp + 20h], ebx
        mov dword ptr [esp + 24h], ecx
        mov dword ptr [esp + 28h], eax
        je reference_copied
        inc dword ptr [eax]
        mov ecx, dword ptr [esp + 24h]
reference_copied:
        cmp ecx, ebp
        jne candidate_position
        mov edi, offset g_DefaultPosition
        jmp have_candidate_position
candidate_position:
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 18h]
        mov edi, eax
have_candidate_position:
        mov ecx, dword ptr [esp + 34h]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 18h]
        fld dword ptr [edi]
        fsub dword ptr [eax]
        fld dword ptr [edi + 4]
        fsub dword ptr [eax + 4]
        fld st(0)
        fmul st(0), st(1)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fstp st(2)
        fstp st(0)
        fild dword ptr [esp + 38h]
        fld st(1)
        fcompp
        fnstsw ax
        test ah, 41h
        jne discard_distance
        call Sub_RoundFloat
        mov dword ptr [esp + 38h], eax
        mov dword ptr [esp + 10h], esi
        jmp release_local
discard_distance:
        fstp st(0)

release_local:
        mov eax, dword ptr [esp + 28h]
        cmp eax, ebp
        mov dword ptr [esp + 20h], ebx
        je local_released
        dec dword ptr [eax]
        mov eax, dword ptr [esp + 28h]
        cmp dword ptr [eax], ebp
        jne local_released
        mov ecx, dword ptr [eax + 8]
        call dword ptr [eax + 4]
        mov ecx, dword ptr [esp + 28h]
        push ecx
        call Sub_DeleteReference
        add esp, 4
local_released:
        lea ecx, [esp + 20h]
        mov dword ptr [esp + 24h], ebp
        mov dword ptr [esp + 28h], ebp
        call Sub_DestroyScriptThing
        mov ecx, dword ptr [esp + 18h]
        add esi, 0Ch
        cmp esi, ecx
        jne candidate_loop

        mov ebx, dword ptr [esp + 30h]
        mov eax, dword ptr [esp + 10h]
        mov dword ptr [ebx], offset g_ScriptThingVTable
        mov edx, dword ptr [eax + 4]
        mov dword ptr [ebx + 4], edx
        mov eax, dword ptr [eax + 8]
        cmp eax, ebp
        mov dword ptr [ebx + 8], eax
        je result_copied
        inc dword ptr [eax]
        mov ecx, dword ptr [esp + 18h]
result_copied:
        mov esi, dword ptr [esp + 14h]
        cmp esi, ecx
        mov edi, ecx
        je vector_destroyed
vector_destroy_loop:
        mov eax, dword ptr [esi]
        push ebp
        mov ecx, esi
        call dword ptr [eax]
        add esi, 0Ch
        cmp esi, edi
        jne vector_destroy_loop
        mov esi, dword ptr [esp + 14h]
vector_destroyed:
        cmp esi, ebp
        je vector_freed
        push esi
        call Sub_Free
        add esp, 4
vector_freed:
        pop edi
        pop esi
        pop ebp
        mov eax, ebx
        pop ebx
        add esp, 1Ch
        ret 0Ch
    }
}
