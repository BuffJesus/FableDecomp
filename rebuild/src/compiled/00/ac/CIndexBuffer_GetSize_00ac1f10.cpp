// CIndexBuffer::GetSize @ 00ac1f10
// eax=this->f4; ecx2=this->f4->vtbl; sub esp,0x14; call vtbl[0xd](this->f4, &local20);
// returns a dword from the 0x14-byte descriptor filled by the vtbl call.

struct Desc {          // 0x14 bytes
    long a;            // +0x00
    long b;            // +0x04
    long size;         // +0x08
    long d;            // +0x0c
    long e;            // +0x10
};

struct IBufImpl;
struct IBufImplVtbl {
    void* slot0;   // +0x00
    void* slot1;   // +0x04
    void* slot2;   // +0x08
    void* slot3;   // +0x0c
    void* slot4;   // +0x10
    void* slot5;   // +0x14
    void* slot6;   // +0x18
    void* slot7;   // +0x1c
    void* slot8;   // +0x20
    void* slot9;   // +0x24
    void* slot10;  // +0x28
    void* slot11;  // +0x2c
    void* slot12;  // +0x30
    void (__stdcall *GetDesc)(IBufImpl* self, Desc* out); // +0x34
};
struct IBufImpl {
    IBufImplVtbl* vtbl;
};

struct CIndexBuffer {
    void* f0;         // +0x00
    IBufImpl* f4;     // +0x04
};

long __fastcall GetSize(CIndexBuffer* self)
{
    Desc d;
    self->f4->vtbl->GetDesc(self->f4, &d);
    return d.e;
}