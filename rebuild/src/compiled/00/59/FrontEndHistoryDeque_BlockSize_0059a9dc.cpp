// Front-end history deque fixed block capacity @ 0x0059A9DC.
//
// Each deque block stores 32 component pointers.  On the 32-bit retail ABI
// that is the 0x80-byte ownership unit allocated at 0x0059AA43.

extern "C" __declspec(naked)
unsigned long __cdecl FrontEndHistoryDeque_BlockSize_0059a9dc()
{
    __asm
    {
        push 20h
        pop eax
        ret
    }
}
