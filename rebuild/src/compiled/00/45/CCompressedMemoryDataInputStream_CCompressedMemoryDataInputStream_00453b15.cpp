#pragma optimize("s",on)
struct CCompressedMemoryDataInputStream {
    void* vtbl;
    char pad[0xa4];
    void* buf_a8;   // +0xa8
    char pad2[0x8]; // +0xac..+0xb3
    void* buf_b4;   // +0xb4
};

void __cdecl operator delete(void* p);
extern void __fastcall CBase_dtor(void* self);

void __fastcall CCompressedMemoryDataInputStream_dtor(CCompressedMemoryDataInputStream* self)
{
    if (self->buf_b4)
        operator delete(self->buf_b4);
    if (self->buf_a8)
        operator delete(self->buf_a8);
    self->vtbl = (void*)0x1230ba0;
    CBase_dtor(self);
}