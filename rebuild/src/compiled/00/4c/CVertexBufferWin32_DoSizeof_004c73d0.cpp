struct CVertexBufferWin32 { char pad[0x60]; int baseSize; };
int __fastcall DoSizeof(CVertexBufferWin32* self)
{
    return self->baseSize + 0xc;
}