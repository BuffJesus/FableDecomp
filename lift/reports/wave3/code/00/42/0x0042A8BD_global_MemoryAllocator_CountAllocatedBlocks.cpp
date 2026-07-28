struct CMemoryAllocatorVariableSize_Overlay
{
    CMemoryAllocatorVariableSize* link;
};

static_assert(offsetof(CMemoryAllocatorVariableSize_Overlay, link) == 0x0);

int __cdecl MemoryAllocator_CountAllocatedBlocks(
    unsigned long* param_1,
    CMemoryAllocatorVariableSize* param_2)
{
    CMemoryAllocatorVariableSize* const end =
        reinterpret_cast<CMemoryAllocatorVariableSize_Overlay*>(param_2)->link;
    CMemoryAllocatorVariableSize* current =
        reinterpret_cast<CMemoryAllocatorVariableSize*>(*param_1);
    int count = 0;

    while (true)
    {
        CMemoryAllocatorVariableSize* const previous = current;
        if (previous == end)
        {
            return count;
        }

        current = reinterpret_cast<CMemoryAllocatorVariableSize*>(
            CMemoryAllocatorVariableSize::GetNoAllocatedAreas(param_2));
        count = count + 1;
        param_2 = previous;
    }
}