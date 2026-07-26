extern "C" void NotifyBeforeDie(void);
extern "C" void ReleaseMapwho(void *);
extern "C" __declspec(naked) void OnDie(void *) {
    __asm { push esi }
    __asm { mov esi, ecx }
    __asm { test esi, esi }
    __asm { je done }
    __asm { call NotifyBeforeDie }
    __asm { push esi }
    __asm { call ReleaseMapwho }
    __asm { add esp, 4 }
    __asm { done: }
    __asm { pop esi }
    __asm { ret }
}