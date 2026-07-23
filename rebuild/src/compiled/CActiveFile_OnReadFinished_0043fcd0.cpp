extern "C" void FinishReadPrelude(void);
extern "C" void ReleaseReadToken(void *);
extern "C" __declspec(naked) void OnReadFinished(void *) {
    __asm { push esi }
    __asm { mov esi, ecx }
    __asm { call FinishReadPrelude }
    __asm { mov esi, dword ptr [esi] }
    __asm { test esi, esi }
    __asm { je done }
    __asm { push esi }
    __asm { call ReleaseReadToken }
    __asm { add esp, 4 }
    __asm { done: }
    __asm { pop esi }
    __asm { ret }
}