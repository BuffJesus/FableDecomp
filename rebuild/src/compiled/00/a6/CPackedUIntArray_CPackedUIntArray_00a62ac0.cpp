// Retail 0x00a62ac0  CPackedUIntArray::~CPackedUIntArray  __fastcall, void
// Layout reconstructed from disasm:
//   [esi+0x0]  = heap buffer pointer (freed)
//   [esi+0x4]  = dword (count/capacity) -> 0
//   [esi+0x8]  = byte flag -> 0
//   [esi+0xc]  = dword -> 0

// cdecl free helper at 0xbfeb1c
void FreePackedUIntArrayBuffer(void* p);

struct CPackedUIntArray
{
    void* buffer;   // +0x0
    int   f4;       // +0x4
    char  f8;       // +0x8
    char  pad9;
    char  padA;
    char  padB;
    int   fC;       // +0xc

    ~CPackedUIntArray();
};

CPackedUIntArray::~CPackedUIntArray()
{
    FreePackedUIntArrayBuffer(this->buffer);
    this->buffer = 0;
    this->f4 = 0;
    this->f8 = 0;
    this->fC = 0;
}