std::uint32_t __thiscall CompareAndCountAllocations(
    CompareDataOffset* self,
    unsigned long value)
{
    CompareDataOffset* localC = self;
    CompareDataOffset* local8 = self;

    CBankFile::CompareDataOffset::operator()(
        self,
        reinterpret_cast<unsigned long>(&localC),
        value);

    const std::uint32_t result =
        MemoryAllocator_CountAllocatedBlocks(
            &localC,
            &local8,
            reinterpret_cast<std::uint8_t*>(&value) + 3);

    CMemoryAllocatorVariableSize_Clear(localC, local8);
    return result;
}