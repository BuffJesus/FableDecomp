float CMemoryAllocatorFixedSize::GetFragmentation()
{
    return *reinterpret_cast<const float*>(0x0122DEDC);
}