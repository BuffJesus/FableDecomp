// CNewFrontendGameComponent::InitialiseEngine @ 0x0042E204
//
// Exact retail engine/frontend handoff. The named seams keep the recovered
// primitive, environment-definition, profile-path, and engine-init contracts
// readable while their native ownership is still being promoted.

extern "C" const char FableInitialiseEngineEnvironmentName_01230c84[];
extern "C" unsigned char FableInitialiseEngineProfileFlag_01375459;
extern "C" unsigned char FableInitialiseEngineCacheFlag_013b8642;
extern "C" unsigned long FableInitialiseEngineDefaults_0137549c[10];

extern "C" void* FableInitialiseEngineCreatePrimitive_00b26340();
extern "C" void FableInitialiseEngineReset_0042fd04();
extern "C" void FableInitialiseEngineCharStringCtor_0099ebf0();
extern "C" void* FableInitialiseEngineGetPreMainMemory_0043368d();
extern "C" void FableInitialiseEngineGetEnvironmentDef_00430096();
extern "C" void FableInitialiseEngineStringCons_0099eae0();
extern "C" void FableInitialiseEngineCtor_0042d830();
extern "C" void* FableInitialiseEngineGetSystemManager_009a4ec0();
extern "C" void* FableInitialiseEngineGetAutoSaveName_0041a100();
extern "C" void FableInitialiseEngineWideAssign_0099b7d0();
extern "C" void FableInitialiseEngineWideDestroy_0099b510();
extern "C" void* FableInitialiseEngineGetAutoSaveName_0041a1c0();
extern "C" void* FableInitialiseEngineGetAutoSaveName_0041a1e0();
extern "C" void* FableInitialiseEngineGetAutoSaveName_0041a200();
extern "C" void* FableInitialiseEngineGetCacheDir_0041a220();
extern "C" void FableInitialiseEngineDestructor_0042f7c1();

extern "C" __declspec(naked) void __fastcall
CNewFrontendGameComponent_InitialiseEngine_0042e204(void*)
{
    __asm
    {
        push ebp
        lea ebp, [esp - 078h]
        sub esp, 0a8h
        push ebx
        push esi
        mov esi, ecx
        push edi
        lea ebx, [esi + 058h]

        call FableInitialiseEngineCreatePrimitive_00b26340
        push eax
        mov ecx, ebx
        call FableInitialiseEngineReset_0042fd04
        and dword ptr [ebp + 070h], 0
        push -1
        push offset FableInitialiseEngineEnvironmentName_01230c84
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineCharStringCtor_0099ebf0
        lea eax, [ebp + 070h]
        push eax
        push 1
        lea eax, [ebp + 074h]
        push eax
        call FableInitialiseEngineGetPreMainMemory_0043368d
        mov ecx, eax
        call FableInitialiseEngineGetEnvironmentDef_00430096
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineStringCons_0099eae0
        lea ecx, [ebp - 030h]
        call FableInitialiseEngineCtor_0042d830
        call FableInitialiseEngineGetSystemManager_009a4ec0
        mov eax, dword ptr [eax + 060h]
        mov eax, dword ptr [eax + 08h]
        and dword ptr [ebp - 014h], 0
        mov dword ptr [ebp - 028h], eax
        mov eax, dword ptr [esi + 010h]
        mov dword ptr [ebp - 020h], eax
        mov eax, dword ptr [esi + 040h]
        lea ecx, [ebp + 074h]
        mov dword ptr [ebp - 01ch], eax
        call FableInitialiseEngineGetAutoSaveName_0041a100
        push eax
        lea ecx, [ebp - 00ch]
        call FableInitialiseEngineWideAssign_0099b7d0
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineWideDestroy_0099b510
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineGetAutoSaveName_0041a1c0
        push eax
        lea ecx, [ebp - 010h]
        call FableInitialiseEngineWideAssign_0099b7d0
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineWideDestroy_0099b510
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineGetAutoSaveName_0041a1e0
        push eax
        lea ecx, [ebp - 008h]
        call FableInitialiseEngineWideAssign_0099b7d0
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineWideDestroy_0099b510
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineGetAutoSaveName_0041a200
        push eax
        lea ecx, [ebp - 004h]
        call FableInitialiseEngineWideAssign_0099b7d0
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineWideDestroy_0099b510
        call FableInitialiseEngineGetPreMainMemory_0043368d
        lea ecx, [ebp + 074h]
        mov dword ptr [ebp - 030h], eax
        call FableInitialiseEngineGetCacheDir_0041a220
        push eax
        lea ecx, [ebp + 000h]
        call FableInitialiseEngineWideAssign_0099b7d0
        lea ecx, [ebp + 074h]
        call FableInitialiseEngineWideDestroy_0099b510
        mov eax, dword ptr [ebp + 070h]
        mov dword ptr [ebp - 02ch], eax
        mov al, byte ptr [FableInitialiseEngineProfileFlag_01375459]
        push 0ah
        pop ecx
        mov esi, offset FableInitialiseEngineDefaults_0137549c
        lea edi, [ebp + 004h]
        rep movsd
        neg al
        sbb eax, eax
        and eax, 4
        or dword ptr [ebp + 030h], eax
        cmp byte ptr [FableInitialiseEngineCacheFlag_013b8642], 0
        je init_engine_flags_ready
        or dword ptr [ebp + 030h], 020h
init_engine_flags_ready:
        mov ecx, dword ptr [ebx]
        mov eax, dword ptr [ecx]
        lea edx, [ebp - 030h]
        push edx
        call dword ptr [eax + 08h]
        mov ecx, dword ptr [ebx]
        mov eax, dword ptr [ecx]
        push 0
        call dword ptr [eax + 0130h]
        fld1
        fstp dword ptr [ebp + 054h]
        mov ebx, dword ptr [ebx]
        fld1
        lea ecx, [ebp + 054h]
        fstp dword ptr [ebp + 058h]
        push ecx
        fld1
        lea ecx, [ebp + 034h]
        push ecx
        fstp dword ptr [ebp + 05ch]
        lea ecx, [ebp + 044h]
        fld1
        push ecx
        lea ecx, [ebp + 064h]
        fstp dword ptr [ebp + 060h]
        push ecx
        fld1
        mov ecx, ebx
        fstp dword ptr [ebp + 034h]
        fld1
        fstp dword ptr [ebp + 038h]
        fld1
        fstp dword ptr [ebp + 03ch]
        fld1
        fstp dword ptr [ebp + 040h]
        fld1
        fstp dword ptr [ebp + 044h]
        fld1
        fstp dword ptr [ebp + 048h]
        fld1
        fstp dword ptr [ebp + 04ch]
        fld1
        fstp dword ptr [ebp + 050h]
        fld1
        fstp dword ptr [ebp + 064h]
        fld1
        fstp dword ptr [ebp + 068h]
        fld1
        fstp dword ptr [ebp + 06ch]
        mov eax, dword ptr [ebx]
        call dword ptr [eax + 010ch]
        lea ecx, [ebp - 030h]
        call FableInitialiseEngineDestructor_0042f7c1
        mov ecx, dword ptr [ebp + 070h]
        test ecx, ecx
        pop edi
        pop esi
        pop ebx
        je init_engine_done
        dec dword ptr [ecx + 04h]
        jne init_engine_done
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 04h]
init_engine_done:
        add ebp, 078h
        leave
        ret
    }
}
