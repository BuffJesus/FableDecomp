// Front-end history deque map-slot deallocator @ 0x0059A9F9.
//
// This is the ownership companion to Allocate at 0x0059A9E0.  It frees the
// map allocation itself; the number of slots is part of the allocator
// interface but is not needed by the retail heap.

extern "C" void __cdecl
FrontEndHistoryDequeMapDelete_0059a9f9(void* allocation);

class FrontEndHistoryDequeMapAllocator_0059a9f9
{
public:
    void Deallocate(void** allocation, unsigned long count);
};

__declspec(naked)
void FrontEndHistoryDequeMapAllocator_0059a9f9::Deallocate(
    void**,
    unsigned long)
{
    __asm
    {
        cmp dword ptr [esp + 4], 0
        je done
        push dword ptr [esp + 4]
        call FrontEndHistoryDequeMapDelete_0059a9f9
        pop ecx

    done:
        ret 8
    }
}
