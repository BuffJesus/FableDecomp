extern "C" __declspec(naked) unsigned long GetConstantCount(void *) {
    __asm { mov eax, dword ptr [ecx+7BCh] }
    __asm { sub eax, dword ptr [ecx+7B8h] }
    __asm { sar eax, 3 }
    __asm { ret }
}