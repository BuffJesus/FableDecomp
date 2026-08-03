// CVertexBufferWin32::DoSizeof  @ 0x0049c770
// mov eax,[ecx+8]; add eax,0x18; ret
struct CVertexBufferWin32 {
    int m_field0;   // +0x0
    int m_field4;   // +0x4
    int m_dataSize; // +0x8
};

int __fastcall DoSizeof(CVertexBufferWin32 *this_)
{
    return this_->m_dataSize + 0x18;
}