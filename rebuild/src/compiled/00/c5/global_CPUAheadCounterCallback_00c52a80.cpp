extern "C" unsigned __int64 __cdecl CPUAheadCounterCallback(void)
{
    unsigned __int64 t;
    __asm
    {
        rdtsc
        mov dword ptr t, eax
        mov dword ptr t+4, edx
    }
    return t;
}