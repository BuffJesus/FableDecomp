// CActiveFile::OnReadFinished @ 0x00aec9d0
// push esi; mov esi,ecx; call 0xaec840 (thiscall method, ecx=this);
// mov esi,[esi]; test esi,esi; je end; push esi; call 0xbfea14 (cdecl free); add esp,4; pop esi; ret

struct CActiveFile {
    void* m_buffer;   // +0x00
    void Finalise();  // the method at 0xaec840
    void OnReadFinished();
};

// external cdecl deallocator (call 0xbfea14 with add esp,4 cleanup)
extern "C" void FreeBuffer(void* p);

void CActiveFile::OnReadFinished()
{
    this->Finalise();
    if (this->m_buffer)
        FreeBuffer(this->m_buffer);
}