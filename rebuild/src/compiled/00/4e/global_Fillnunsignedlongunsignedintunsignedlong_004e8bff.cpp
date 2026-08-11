#pragma optimize("s",on)
// std::_Fill_n<unsigned long*, unsigned int, unsigned long>
// __fastcall: ecx = dest, edx = count, [esp+4] = val (pointer to fill value)
// Returns the one-past-the-end output iterator in eax (walked dest pointer).
unsigned long * __fastcall Fill_n_ulong(unsigned long *dest, unsigned int count, unsigned long *val)
{
    for (; 0 < count; --count, ++dest)
    {
        if (dest != 0)
            *dest = *val;
    }
    return dest;
}