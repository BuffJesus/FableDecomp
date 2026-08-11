#include <cstdio>

struct CNavNavigableLeafNode {
    char pad[0x18];
    signed char m_kind;

    unsigned long GetMemoryUsage(unsigned long& a,
                                 unsigned long& b,
                                 unsigned long& c,
                                 unsigned long& d);
};

unsigned long CNavNavigableLeafNode::GetMemoryUsage(unsigned long& a,
                                                    unsigned long& b,
                                                    unsigned long& c,
                                                    unsigned long& d)
{
    ++a;
    ++c;
    return (unsigned long)((int)m_kind * 4 + 0x20);
}

int main()
{
    CNavNavigableLeafNode n;
    for (int i = 0; i < 0x18; ++i) n.pad[i] = 0;

    n.m_kind = 5;
    unsigned long a = 10, b = 20, c = 30, d = 40;
    unsigned long r = n.GetMemoryUsage(a, b, c, d);

    bool ok = true;
    if (r != (unsigned long)(5 * 4 + 0x20)) ok = false; // 0x34 = 52
    if (a != 11) ok = false;   // incremented
    if (b != 20) ok = false;   // untouched
    if (c != 31) ok = false;   // incremented
    if (d != 40) ok = false;   // untouched

    // negative kind (signed byte)
    n.m_kind = (signed char)-2;
    unsigned long a2 = 0, b2 = 0, c2 = 0, d2 = 0;
    unsigned long r2 = n.GetMemoryUsage(a2, b2, c2, d2);
    if (r2 != (unsigned long)(-2 * 4 + 0x20)) ok = false; // 0x18 = 24
    if (a2 != 1 || c2 != 1 || b2 != 0 || d2 != 0) ok = false;

    if (ok) printf("NAVLEAF_MEMUSAGE_OK r=%lu r2=%lu\n", r, r2);
    else    printf("NAVLEAF_MEMUSAGE_FAIL r=%lu r2=%lu a=%lu c=%lu\n", r, r2, a, c);
    return ok ? 0 : 1;
}