// CMemoryDataOutputStream::Clear  @ 00a66c00
// push esi; mov esi,ecx; lea ecx,[esi+0x340c]; call 0x9e4710
// mov [esi+0x3404],0; pop esi; ret

struct SubBuffer {
    char pad[0x400];
    void Clear();   // 0x9e4710, __fastcall
};

struct CMemoryDataOutputStream {
    char     pad0[0x3404];   // 0..0x3403
    unsigned WritePos;       // 0x3404
    char     pad1[0x340c - 0x3408];
    SubBuffer sub;           // 0x340c
    void Clear();
};

void CMemoryDataOutputStream::Clear()
{
    this->sub.Clear();
    this->WritePos = 0;
}