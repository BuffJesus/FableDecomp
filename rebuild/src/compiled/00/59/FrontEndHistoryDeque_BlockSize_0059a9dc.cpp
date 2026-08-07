// Front-end history deque fixed block capacity @ 0x0059A9DC.
//
// Each deque block stores 32 component pointers.  VC7.1 size optimization
// emits the retail push/pop constant sequence without assembly.

#pragma optimize("s", on)

extern "C" unsigned long __cdecl
FrontEndHistoryDeque_BlockSize_0059a9dc()
{
    return 0x20;
}

#pragma optimize("s", off)
