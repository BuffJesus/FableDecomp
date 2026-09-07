// CTCCoopSpirit::SwapToHero @ 0x0066ff20
// Exact byte-led coordinator with neutral names for still-opaque engine seams.

extern "C" void CoopSpirit_GetPlayerManager_00686d40();
extern "C" void CoopSpirit_GetDrawEnvironment_00449700();
extern "C" void CoopSpirit_GetThingPosition_004c73d0();
extern "C" void CoopSpirit_SwapPlayerSlotToHero_00449cb0();
extern "C" void CoopSpirit_CreateSwapChild_00449730();
extern "C" void CoopSpirit_AddSwapChild_00449b20();
extern "C" void CoopSpirit_TemporaryDtor_0099eae0();
extern "C" void CoopSpirit_GetPlayer_004498c0();
extern "C" void CoopSpirit_ResolvePlayer_00487dc0();

extern "C" __declspec(naked) void __fastcall
CTCCoopSpirit_SwapToHero_0066ff20(void* self)
{
    __asm {
        sub esp, 10h
        push esi
        push edi
        mov esi, ecx
        call CoopSpirit_GetPlayerManager_00686d40
        mov ecx, eax
        call CoopSpirit_GetDrawEnvironment_00449700
        mov ecx, [esi+4]
        mov edi, eax
        call CoopSpirit_GetThingPosition_004c73d0
        mov ecx, [eax]
        mov [esp+0ch], ecx
        mov edx, [eax+4]
        mov [esp+10h], edx
        mov eax, [eax+8]
        mov [esp+14h], eax
        mov eax, [esi+4]
        movsx ecx, byte ptr [eax+90h]
        push ecx
        mov ecx, esi
        call CoopSpirit_GetPlayerManager_00686d40
        mov ecx, eax
        call CoopSpirit_SwapPlayerSlotToHero_00449cb0
        push edi
        lea edx, [esp+0ch]
        push edx
        mov ecx, esi
        call CoopSpirit_GetPlayerManager_00686d40
        mov ecx, eax
        call CoopSpirit_CreateSwapChild_00449730
        push eax
        push edi
        mov ecx, esi
        call CoopSpirit_GetPlayerManager_00686d40
        mov ecx, eax
        call CoopSpirit_AddSwapChild_00449b20
        lea ecx, [esp+8]
        call CoopSpirit_TemporaryDtor_0099eae0
        push edi
        mov ecx, esi
        call CoopSpirit_GetPlayerManager_00686d40
        mov ecx, eax
        call CoopSpirit_GetPlayer_004498c0
        mov ecx, eax
        call CoopSpirit_ResolvePlayer_00487dc0
        mov ecx, [eax+60h]
        mov eax, [ecx]
        lea edx, [esp+0ch]
        push edx
        call dword ptr [eax+7ch]
        mov esi, [esi+4]
        mov eax, [esi+0b0h]
        pop edi
        mov [esi+0b4h], eax
        pop esi
        add esp, 10h
        ret
    }
}