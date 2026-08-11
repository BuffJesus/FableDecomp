// CMemoryDataOutputStream::Clear  @ 0x00a66860
// push esi; mov esi,ecx; lea ecx,[esi+0x340c]; call sub; mov [esi+0x3404],0; pop esi; ret

struct MemberBuffer {
    char pad[0x40];
    void Clear();   // non-virtual, __fastcall  -> direct call
};

struct CMemoryDataOutputStream {
    char   pad0[0x3404];   // 0x0000 .. 0x3403
    int    m_size;         // 0x3404
    char   pad1[4];        // 0x3408 .. 0x340b
    MemberBuffer m_buffer; // 0x340c
    void Clear();
};

void CMemoryDataOutputStream::Clear()
{
    m_buffer.Clear();
    m_size = 0;
}