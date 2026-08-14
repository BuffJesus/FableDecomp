// Byte-exact reconstruction of CMemoryFile::Open @ 0x009e90a0

struct Sub {
    // member subobject at CMemoryFile+0x20.
    // Real C++ member => native thiscall (ecx=this, arg on stack, edx untouched).
    void init(const char* name);
};

struct CMemoryFile {
    char pad0[8];       // +0x00
    int  field8;        // +0x08
    int  fieldC;        // +0x0c
    char pad10[0x0c];   // +0x10 .. 0x1b
    char field1C;       // +0x1c
    char pad1D[3];      // +0x1d
    Sub  sub;           // +0x20
};

void __fastcall Open(CMemoryFile* self)
{
    self->sub.init((const char*)0x122d70c);
    self->field8 = 0;
    self->fieldC = 0;
    self->field1C = 1;
}