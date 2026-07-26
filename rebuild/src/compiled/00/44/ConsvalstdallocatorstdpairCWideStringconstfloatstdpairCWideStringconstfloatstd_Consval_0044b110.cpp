extern "C" void ConstructWideFloatPair(void *, void *);
extern "C" __declspec(naked) void _Cons_val(void *, void *) {
    __asm { test ecx, ecx }
    __asm { je done }
    __asm { push edx }
    __asm { call ConstructWideFloatPair }
    __asm { done: }
    __asm { ret }
}