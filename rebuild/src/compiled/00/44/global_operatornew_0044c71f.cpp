// operator new(size_t) -> forwards to global allocator's thiscall Allocate(size)
struct CAllocator {
    void* Allocate(unsigned int size);
};
extern CAllocator g_allocator;

void* __fastcall operator_new(unsigned int size)
{
    return g_allocator.Allocate(size);
}