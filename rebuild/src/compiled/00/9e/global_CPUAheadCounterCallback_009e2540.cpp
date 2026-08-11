// CPUAheadCounterCallback @ 009e2540
// Reads the CPU timestamp counter (rdtsc) and returns it as a 64-bit value.
// VC7.1 has no rdtsc intrinsic. The retail codegen zeroes the __int64 into the
// return registers (xor eax; xor edx), runs rdtsc, then spills/reloads the
// value through an 8-byte stack slot before returning it in edx:eax.

typedef unsigned __int64 uint64;

extern "C" unsigned __int64 __cdecl CPUAheadCounterCallback(unsigned long unused)
{
    uint64 t;
    __asm xor eax, eax;
    __asm xor edx, edx;
    __asm rdtsc;
    __asm mov dword ptr t, eax;
    __asm mov dword ptr t+4, edx;
    return t;
}