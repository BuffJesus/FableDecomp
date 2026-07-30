// CThing::IsForceNoSerialise @ 0x004c7bd0
// __fastcall bool(this=ecx). Loads [ecx+0x70]; if null -> false;
// else test byte [that+0x41] & 0x40 -> true if set else false.

struct Inner {
    char pad[0x41];
    unsigned char flags; // +0x41
};

struct CThing {
    char pad[0x70];
    Inner* m_inner; // +0x70
    bool IsForceNoSerialise();
};

bool CThing::IsForceNoSerialise() {
    Inner* p = m_inner;
    if (p != 0 && (p->flags & 0x40) != 0)
        return true;
    return false;
}