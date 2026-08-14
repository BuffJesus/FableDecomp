// Byte-exact reconstruction of CDataOutputStream::WriteCBYTE @ 0x00993ec0
// Retail:
//   mov eax,[ecx]        ; vtable
//   push 8
//   lea edx,[esp+8]      ; &value (arg1 on stack; this in ecx via thiscall)
//   push edx
//   call [eax+0x10]      ; vtable slot 4  -> thiscall Write(this, void* p, int n)
//   ret 8
//
// slot4 is the 5th virtual (offset 0x10). Model the base with 4 preceding
// virtuals then WriteBytes so the compiler emits a call [eax+0x10] with
// this in ecx and the two args pushed (thiscall callee).

struct IDataOutputStream {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void WriteBytes(const void* p, int n);   // slot4 @ +0x10
};

struct CDataOutputStream : public IDataOutputStream {
    void WriteCBYTE(char value, int trailing);
};

void CDataOutputStream::WriteCBYTE(char value, int /*trailing*/)
{
    this->WriteBytes(&value, 8);
}