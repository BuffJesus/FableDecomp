// Byte-exact reconstruction of 0x0046b81d
// std::_Fill_n specialization for a 16-byte C3DClothQuad value.
// __fastcall: ecx = dest, edx = count, [esp+4] = pointer to value.
// Returns advanced dest pointer.

#pragma optimize("s", on)

struct C3DClothQuad {
    unsigned int a, b, c, d; // 16 bytes
};

extern "C" C3DClothQuad* __fastcall fill_C3DClothQuad(C3DClothQuad* dest, unsigned int count, const C3DClothQuad* val)
{
    for (; 0 < count; --count) {
        if (dest) { *dest = *val; }
        ++dest;
    }
    return dest;
}