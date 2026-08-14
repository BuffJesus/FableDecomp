// std::_Fill_n specialized for a 12-byte value type (3 unsigned longs).
// __fastcall: ecx = dest pointer, edx = count; [esp+0xc] = pointer to value.
// Returns the advanced dest pointer (left in eax), matching retail's induction
// on eax. optimize("s") makes VC7.1 emit the movsd*3 struct copy the retail
// build uses instead of an unrolled scalar mov sequence.
#pragma optimize("s",on)
struct V12 { unsigned long a, b, c; };

V12* __fastcall Fill_n(V12* dest, unsigned int count, const V12* val)
{
    for (; count > 0; --count, ++dest)
        if (dest)
            *dest = *val;
    return dest;
}