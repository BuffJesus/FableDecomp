// CNavNavigableLeafNode::GetMemoryUsage  retail 0x00a76500
// __fastcall ulong (this, ulong& a, ulong& b, ulong& c, ulong& d)

struct CNavNavigableLeafNode {
    char pad[0x18];
    signed char m_kind; // offset 0x18

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