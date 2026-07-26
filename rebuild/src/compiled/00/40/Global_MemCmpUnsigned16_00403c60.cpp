int __cdecl MemCmp_Unsigned16(
    const unsigned short* lhs, const unsigned short* rhs, unsigned long count)
{
    unsigned long index;
    for (index = 0; index < count; ++index)
    {
        if (lhs[index] != rhs[index])
            return lhs[index] < rhs[index] ? -1 : 1;
    }
    return 0;
}
