extern "C" __declspec(naked) bool IsInputTypeWithMouseButton(void *, int) {
    __asm { xor al, al }
    __asm { ret 4 }
}
