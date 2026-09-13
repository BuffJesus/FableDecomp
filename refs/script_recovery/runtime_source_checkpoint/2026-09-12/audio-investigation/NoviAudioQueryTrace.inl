// Optional diagnostic for an isolated candidate only. Original FSE stays unchanged.
static DWORD g_noviAudioQuerySuccess=0, g_noviAudioQueryFailure=0;
extern "C" __declspec(dllexport) void __cdecl NoviRecordAudioQuery(DWORD hr, DWORD raw, DWORD object) {
    char text[240];
    sprintf_s(text, "[NoviAudioQuery] tick=%lu object=%08lx HRESULT=%08lx raw=%08lx rawValid=%u",
              GetTickCount(),object,hr,raw,static_cast<LONG>(hr)>=0?1u:0u);
    LogToFile(text);
}
extern "C" __declspec(dllexport) __declspec(naked) void NoviAudioQueryTrace() {
    __asm {
        pushfd
        pushad
        sub esp,230h
        lea eax,[esp+0fh]
        and eax,0fffffff0h
        fxsave [eax]
        // Original EAX, output DWORD at original ESP+4, and original ESI.
        mov eax,[esp+24ch]
        mov ecx,[esp+258h]
        mov edx,[esp+234h]
        push edx
        push ecx
        push eax
        call NoviRecordAudioQuery
        add esp,0ch
        lea eax,[esp+0fh]
        and eax,0fffffff0h
        fxrstor [eax]
        add esp,230h
        popad
        popfd
        // Replay overwritten TEST/JGE and the failure-only MOV exactly.
        test eax,eax
        jl failed
        jmp dword ptr [g_noviAudioQuerySuccess]
    failed:
        mov eax,[esi+8]
        jmp dword ptr [g_noviAudioQueryFailure]
    }
}
static bool InstallNoviAudioQueryTrace() {
    BYTE* site=reinterpret_cast<BYTE*>(ASLR<DWORD>(0xC53352));
    const BYTE expected[]={0x85,0xc0,0x7d,0x1b,0x8b,0x46,0x08};
    if (memcmp(site,expected,sizeof(expected))) return false;
    DWORD old=0;
    if (!VirtualProtect(site,sizeof(expected),PAGE_EXECUTE_READWRITE,&old)) return false;
    g_noviAudioQuerySuccess=ASLR<DWORD>(0xC53371);
    g_noviAudioQueryFailure=ASLR<DWORD>(0xC53359);
    BYTE replacement[]={0xe9,0,0,0,0,0x90,0x90};
    DWORD jump=reinterpret_cast<DWORD>(&NoviAudioQueryTrace)-reinterpret_cast<DWORD>(site)-5;
    memcpy(replacement+1,&jump,4);
    memcpy(site,replacement,sizeof(replacement));
    DWORD ignored=0;
    VirtualProtect(site,sizeof(replacement),old,&ignored);
    FlushInstructionCache(GetCurrentProcess(),site,sizeof(replacement));
    return true;
}
