extern "C" __declspec(naked) bool IsInputTypeWithButton(void *, int) {
    __asm { mov ecx, dword ptr [ecx+4] }
    __asm { mov edx, dword ptr [esp+4] }
    __asm { xor eax, eax }
    __asm { cmp ecx, edx }
    __asm { sete al }
    __asm { ret 4 }
}