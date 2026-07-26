static_assert(offsetof(CGameEvent, Type) == 0x0);
static_assert(offsetof(CGameEvent, Player) == 0x4);
static_assert(offsetof(CGameEvent, Data) == 0x5);
static_assert(offsetof(CGameEvent, EndPos) == 0x25);
static_assert(offsetof(CGameEvent, Valid) == 0x26);
static_assert(offsetof(CGameEvent, Replacement) == 0x27);
static_assert(sizeof(CGameEvent) == 0x28);

long __thiscall CGameEvent::InitFromCompressedBuffer(const unsigned char* area)
{
    unsigned int type = static_cast<unsigned int>(*reinterpret_cast<const unsigned short*>(area));
    const unsigned char endPos = area[3];
    const bool replacement = (static_cast<unsigned short>(type) & 0x8000u) != 0;

    if (replacement) {
        type = type & 0x7fffu;
    }

    Type = static_cast<long>(type);
    Player = static_cast<signed char>(area[2]);
    Replacement = replacement;
    EndPos = 0;
    Valid = true;

    const unsigned char* src = area + 4;
    unsigned char* dst = &Data[0];

    for (unsigned int count = static_cast<unsigned int>(endPos >> 2); count != 0; --count) {
        *reinterpret_cast<unsigned int*>(dst) = *reinterpret_cast<const unsigned int*>(src);
        src += 4;
        dst += 4;
    }

    for (unsigned int count = static_cast<unsigned int>(endPos & 3); count != 0; --count) {
        *dst = *src;
        ++src;
        ++dst;
    }

    EndPos = static_cast<unsigned char>(EndPos + endPos);
    return static_cast<long>(static_cast<unsigned int>(endPos) + 4u);
}