long __thiscall CGameEvent::CompressIntoBuffer(unsigned char* buffer) const
{
    unsigned short type = static_cast<unsigned short>(Type);
    if (Replacement) {
        type = static_cast<unsigned short>(type | 0x8000);
    }

    *reinterpret_cast<unsigned short*>(buffer) = type;
    buffer[2] = static_cast<unsigned char>(Player);

    const unsigned char endPos = EndPos;
    buffer[3] = endPos;

    const unsigned char* src = &Data[0];
    unsigned char* dst = buffer + 4;

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

    return static_cast<long>(4u + static_cast<unsigned int>(endPos));
}