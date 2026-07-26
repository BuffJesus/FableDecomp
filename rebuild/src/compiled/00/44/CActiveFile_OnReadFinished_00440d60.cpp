extern "C" void ReleaseReadToken(void *);
extern "C" __declspec(naked) void OnReadFinished(void *) {
    __asm { mov eax, dword ptr [ecx] }
    __asm { test eax, eax }
    __asm { je done }
    __asm { push eax }
    __asm { call ReleaseReadToken }
    __asm { pop ecx }
    __asm { done: }
    __asm { ret }
}