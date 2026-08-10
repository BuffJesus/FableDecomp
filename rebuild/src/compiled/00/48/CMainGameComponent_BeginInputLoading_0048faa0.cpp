struct CMainGameComponent;
struct CWideString;

extern "C" void __stdcall LoadingTarget_48e0a2(void* ws, int mode);

__declspec(naked) void BeginInputLoading()
{
    __asm {
        mov edx, dword ptr [esp+8]
        mov ecx, dword ptr [esp+4]
        push 47h
        push dword ptr [esp+10h]
        call LoadingTarget_48e0a2
        ret 0Ch
    }
}