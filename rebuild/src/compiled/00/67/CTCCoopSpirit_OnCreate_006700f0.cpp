// CTCCoopSpirit::OnCreate @ 0x006700f0
//
// Byte-led reconstruction.  The generated lift attached unrelated morph and
// drunkenness template identities to this body.  Retail instead initializes
// the particle-emitter intelligent pointer, calls the base OnCreate, clears
// spirit state, resolves the owning thing's definition index, extracts the
// coop-spirit subdefinition, and releases the temporary definition reference.

extern "C" void CoopSpirit_IntelligentPointerCtor_00a01b10();
extern "C" void CoopSpirit_IntelligentPointerSet_00a01b90();
extern "C" void CoopSpirit_IntelligentPointerGet_00a01b50();
extern "C" void CoopSpirit_IntelligentPointerDtor_00a01c10();
extern "C" void CoopSpirit_BaseOnCreate_00686850();
extern "C" void CoopSpirit_GetDefinitionManager_006869d0();
extern "C" void CoopSpirit_DefinitionManagerGetDef_0042af3c();
extern "C" void CoopSpirit_DefinitionGetSubDef_00670b90();
extern "C" unsigned long CoopSpirit_IntelligentPointerVftable_01238c6c;

extern "C" __declspec(naked) void __fastcall
CTCCoopSpirit_OnCreate_006700f0(void* self)
{
    __asm {
        sub esp, 8
        push esi
        mov esi, ecx
        push edi
        lea ecx, [esp+8]
        call CoopSpirit_IntelligentPointerCtor_00a01b10
        xor edi, edi
        push edi
        lea ecx, [esp+0ch]
        mov dword ptr [esp+0ch], offset CoopSpirit_IntelligentPointerVftable_01238c6c
        call CoopSpirit_IntelligentPointerSet_00a01b90
        lea ecx, [esp+8]
        call CoopSpirit_IntelligentPointerGet_00a01b50
        push eax
        lea ecx, [esi+2ch]
        call CoopSpirit_IntelligentPointerSet_00a01b90
        lea ecx, [esp+8]
        call CoopSpirit_IntelligentPointerDtor_00a01c10
        mov ecx, esi
        call CoopSpirit_BaseOnCreate_00686850
        mov [esi+20h], edi
        mov [esi+1ch], edi
        mov [esi+18h], edi
        mov ecx, [esi+4]
        mov [esi+0ch], edi
        mov [esi+24h], edi
        mov [esi+14h], edi
        mov [esi+10h], edi
        movzx edx, word ptr [ecx+8ch]
        lea eax, [esp+8]
        push eax
        push edx
        mov ecx, esi
        mov [esp+10h], edi
        call CoopSpirit_GetDefinitionManager_006869d0
        mov ecx, eax
        call CoopSpirit_DefinitionManagerGetDef_0042af3c
        add esi, 28h
        push esi
        mov esi, [esp+0ch]
        mov ecx, esi
        call CoopSpirit_DefinitionGetSubDef_00670b90
        cmp esi, edi
        je release_done
        dec dword ptr [esi+4]
        jne release_done
        mov eax, [esi]
        mov ecx, esi
        call dword ptr [eax+4]
release_done:
        pop edi
        pop esi
        add esp, 8
        ret
    }
}