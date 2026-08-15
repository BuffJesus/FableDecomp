#pragma optimize("s",on)
// CMemoryDataOutputStream::Clear @ 0x00cd2770
// push esi; lea esi,[ecx+8]; mov ecx,esi; call 0x7e70e0; and dword[esi],0; pop esi; ret

struct SubObj {
    unsigned int field0;
};

// external helper called thiscall on the sub-object at +8 (ecx = &sub)
extern "C" void __fastcall SubHelper(SubObj* sub);

struct CMemoryDataOutputStream {
    char pad[8];
    SubObj sub;   // at +8
};

void __fastcall Clear(CMemoryDataOutputStream* self)
{
    SubHelper(&self->sub);
    self->sub.field0 = 0;
}