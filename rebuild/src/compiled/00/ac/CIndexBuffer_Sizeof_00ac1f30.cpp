// CIndexBuffer::Sizeof  @ 0x00ac1f30
// Retail:
//   mov eax,[ecx+4]      ; obj = this->m_desc
//   mov ecx,[eax]        ; vtbl = obj->vptr
//   sub esp,0x14         ; 20-byte return buffer for the virtual
//   lea edx,[esp]
//   push edx             ; hidden struct-return ptr
//   push eax             ; this (obj) passed as an explicit stack arg
//   call [ecx+0x34]      ; vtable slot 13, returns 20-byte struct by value
//   mov eax,[esp+0x10]   ; buf.field_10
//   add eax,0x18
//   ret

struct Desc20 {
    int f0;
    int f4;
    int f8;
    int fc;
    int f10;
};

struct IDescObj;
// Virtual is invoked with an explicit 'self' stack arg (not ecx) plus an explicit
// out-buffer pointer as the second arg; it fills *out and returns void.
typedef void (__stdcall *GetInfoFn)(IDescObj* self, Desc20* out);

struct IDescVtbl {
    GetInfoFn slot0;
    GetInfoFn slot1;
    GetInfoFn slot2;
    GetInfoFn slot3;
    GetInfoFn slot4;
    GetInfoFn slot5;
    GetInfoFn slot6;
    GetInfoFn slot7;
    GetInfoFn slot8;
    GetInfoFn slot9;
    GetInfoFn slot10;
    GetInfoFn slot11;
    GetInfoFn slot12;
    GetInfoFn slot13;   // +0x34
};

struct IDescObj {
    IDescVtbl* vptr;
};

struct CIndexBuffer {
    int m_pad0;
    IDescObj* m_desc;   // +4
};

unsigned long __fastcall CIndexBuffer_Sizeof(CIndexBuffer* self)
{
    IDescObj* obj = self->m_desc;
    Desc20 buf;
    obj->vptr->slot13(obj, &buf);
    return (unsigned long)(buf.f10 + 0x18);
}