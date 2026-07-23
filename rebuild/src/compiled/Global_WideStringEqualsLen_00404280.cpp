extern int __cdecl MemCmp_Unsigned16(
    const unsigned short* lhs, const unsigned short* rhs, unsigned long count);

struct WideStringRange
{
    unsigned short* begin;
    unsigned short* end;
};

int __cdecl WideString_EqualsLen(
    const WideStringRange* wideString, const unsigned short* text)
{
    int length = 0;
    unsigned short ch = *text;
    while (ch != 0)
    {
        ++length;
        ch = text[length];
    }
    const long byteSpan = reinterpret_cast<const char*>(wideString->end) -
                          reinterpret_cast<const char*>(wideString->begin);
    if ((byteSpan >> 1) == length &&
        (length = MemCmp_Unsigned16(wideString->begin, text, length)) == 0)
        return 1;
    return 0;
}
