extern "C" void ForwardMouseButtonThunk(void);
extern "C" __declspec(naked) bool IsInputTypeWithMouseButton(void *, int) {
    __asm { xor al, al }
    __asm { ret 4 }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { _emit 0CCh }
    __asm { add ecx, 4 }
    __asm { jmp ForwardMouseButtonThunk }
}