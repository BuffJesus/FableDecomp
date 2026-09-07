struct Unsigned16ArrayRange
{
    unsigned long begin;
    unsigned long end;
};

int __cdecl MemCmp_Unsigned16(unsigned long lhs, unsigned long rhs, int count);

unsigned long __cdecl Compare_Unsigned16Array(
    const Unsigned16ArrayRange* lhs,
    const Unsigned16ArrayRange* rhs)
{
    int count = ((int)lhs->end - (int)lhs->begin) >> 1;
    if (count == (((int)rhs->end - (int)rhs->begin) >> 1)) {
        count = MemCmp_Unsigned16(lhs->begin, rhs->begin, count);
        if (count == 0) {
            return 1;
        }
    }
    return 0;
}