// CActiveFile::OnReadFinished  0x009f8a90
// push esi; mov esi,ecx; call 0x9f89b0(this); mov esi,[esi];
// test esi; je end; push esi; call 0xbfea14; add esp,4; end.

void __fastcall sub_9f89b0(void* self);   // 0x9f89b0  __fastcall member (void)
void __cdecl   sub_bfea14(void* p);       // 0xbfea14  cdecl free/operator delete

struct CActiveFile
{
    void* m_buffer;   // +0x00
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    sub_9f89b0(this);
    if (this->m_buffer)
        sub_bfea14(this->m_buffer);
}