// Front-end history deque map-slot allocator @ 0x0059A9E0.
//
// The map is an array of pointers to 32-element history blocks.  Allocation is
// therefore count * sizeof(void*) on the 32-bit retail target.  The standard
// allocator hint parameter is accepted but unused.

extern "C" void* __cdecl
FrontEndHistoryDequeMapAllocate_0059a9e0(unsigned long size);

class FrontEndHistoryDequeMapAllocator_0059a9e0
{
public:
    void** Allocate(unsigned long count, const void* hint);
};

__declspec(naked)
void** FrontEndHistoryDequeMapAllocator_0059a9e0::Allocate(
    unsigned long,
    const void*)
{
    __asm
    {
        mov eax, dword ptr [esp + 4]
        test eax, eax
        je empty_map
        shl eax, 2
        push eax
        call FrontEndHistoryDequeMapAllocate_0059a9e0
        pop ecx
        jmp done

    empty_map:
        xor eax, eax

    done:
        ret 8
    }
}
