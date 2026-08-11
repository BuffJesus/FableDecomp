// CMemoryDataOutputStream::Clear  @ 009e42c0

struct SubBuffer {
    void Clear();  // @ 0x9e4710, __fastcall on this=+0xd0c
    int dummy;
};

struct CMemoryDataOutputStream {
    char pad[0xd0c];
    SubBuffer sub;      // at +0xd0c
    unsigned int count; // at +0xd10
    void Clear();
};

void CMemoryDataOutputStream::Clear()
{
    this->sub.Clear();
    this->count = 0;
}