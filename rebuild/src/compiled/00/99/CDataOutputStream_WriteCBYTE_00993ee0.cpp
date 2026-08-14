// CDataOutputStream::WriteCBYTE 0x00993ee0
// mov eax,[ecx]; push 4; lea edx,[esp+8]; push edx; call [eax+0x10]; ret 4
//
// Retail is a __fastcall member forwarding to virtual slot +0x10:
//   this->Write(&value, 4);  where value is a stack (char) parameter.
// Modelling the callee as a real C++ virtual member gives VC7.1's implicit
// __fastcall (this in ecx, args pushed) without writing the __fastcall keyword
// (the harness would rewrite it to __fastcall). WriteCBYTE itself is also a
// member, so 'value' stays a stack param and it emits ret 4.

struct CDataOutputStream {
    // virtual layout: slot +0x10 == index 4 == the 5th virtual.
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void Write(void* buf, int size);   // slot +0x10

    void WriteCBYTE(char value);                // 0x00993ee0
};

void CDataOutputStream::WriteCBYTE(char value)
{
    this->Write(&value, 4);
}