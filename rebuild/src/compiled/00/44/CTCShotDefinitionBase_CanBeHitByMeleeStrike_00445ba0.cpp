extern "C" __declspec(naked) bool CanBeHitByMeleeStrike(void *) {
    __asm { mov eax, dword ptr [ecx+7A0h] }
    __asm { mov al, byte ptr [eax+4Ah] }
    __asm { ret }
}