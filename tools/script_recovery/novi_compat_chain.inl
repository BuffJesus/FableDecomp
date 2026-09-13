// Included by the isolated sidecar's dllmain.cpp, never by upstream FSE.
// Load only before resuming the newly created game's primary thread.
static DWORD g_noviPreviousRegistrationHook = 0;
static bool g_noviStarted = false;

static __declspec(naked) void NoviChainedRegistrationHook() {
    __asm {
        pushfd
        pushad
        sub esp, 230h
        lea eax, [esp+0fh]
        and eax, 0fffffff0h
        fxsave [eax]
        call InjectCustomScripts
        lea eax, [esp+0fh]
        and eax, 0fffffff0h
        fxrstor [eax]
        add esp, 230h
        popad
        popfd
        jmp dword ptr [g_noviPreviousRegistrationHook]
    }
}

extern "C" __declspec(dllexport) DWORD WINAPI NoviCompatibilityStart(void*) {
    if (g_noviStarted) return 2;
    HMODULE original = GetModuleHandleA("FableScriptExtender.dll");
    if (!original) return 10;
    InitializeFableAPI();
    BYTE* site = reinterpret_cast<BYTE*>(ASLR<DWORD>(0xCDB355));
    if (site[0] != 0xE9) return 11;
    DWORD previous = reinterpret_cast<DWORD>(site) + 5 + *reinterpret_cast<DWORD*>(site+1);
    MEMORY_BASIC_INFORMATION info = {};
    if (!VirtualQuery(reinterpret_cast<void*>(previous), &info, sizeof(info)) ||
        info.AllocationBase != original) return 12;
    // Upstream MyHook starts pushad/sub esp,28h/fnstenv [esp].
    const BYTE prefix[] = {0x60,0x83,0xec,0x28,0x9b,0xd9,0x34,0x24};
    if (memcmp(reinterpret_cast<void*>(previous), prefix, sizeof(prefix))) return 13;
    const BYTE addScriptPrefix[] = {0x8b,0x44,0x24,0x08,0x83,0xec,0x18};
    if (memcmp(reinterpret_cast<void*>(ASLR<DWORD>(0xCB5C90)), addScriptPrefix, 7)) return 14;
    DWORD oldProtection;
    if (!VirtualProtect(site, 5, PAGE_EXECUTE_READWRITE, &oldProtection)) return 15;
    if (!InstallRetailOverrideHook()) {
        DWORD ignored;
        VirtualProtect(site, 5, oldProtection, &ignored);
        return 16;
    }
    g_noviPreviousRegistrationHook = previous;
    const DWORD displacement = reinterpret_cast<DWORD>(&NoviChainedRegistrationHook) -
                               reinterpret_cast<DWORD>(site) - 5;
    memcpy(site+1, &displacement, 4);
    DWORD ignored;
    VirtualProtect(site, 5, oldProtection, &ignored);
    FlushInstructionCache(GetCurrentProcess(), site, 5);
    g_noviStarted = true;
    LogToFile("[NoviCompatibility] Original FSE callback chained; independent New Oakvale hosts enabled.");
    return 1;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        InitializeFSEPaths(module);
        DisableThreadLibraryCalls(module);
    }
    return TRUE;
}
