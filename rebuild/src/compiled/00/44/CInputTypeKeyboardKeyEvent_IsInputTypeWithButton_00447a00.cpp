extern "C" __declspec(naked) bool IsInputTypeWithButton(void *, int) {
    __asm { xor al, al }
    __asm { ret 4 }
}