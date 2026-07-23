int __cdecl MemCmp_Unsigned16(
    const unsigned short* lhs, const unsigned short* rhs, unsigned long count)
{
    unsigned long index = 0;
    if (count != 0)
    {
        const unsigned short* current = rhs;
        do
        {
            if (lhs[index] != *current)
                return (-(static_cast<unsigned long>(lhs[index] < rhs[index])) & 0xFFFFFFFEUL) + 1;
            ++index;
            ++current;
        } while (index < count);
    }
    return 0;
}
