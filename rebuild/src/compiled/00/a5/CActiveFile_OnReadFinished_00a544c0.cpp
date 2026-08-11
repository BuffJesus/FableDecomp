// CActiveFile::OnReadFinished  @ 0x00a544c0
// push esi; mov esi,ecx; call helper(this); esi=[this]; if(esi){ free(esi); }

struct CActiveFile {
    void* m_buffer;   // +0x00
    void PreFinish();
    void OnReadFinished();
};

// cdecl free-like deallocator at 0xbfea14
extern "C" void __cdecl ReleaseBuffer(void* p);

void CActiveFile::OnReadFinished()
{
    this->PreFinish();
    void* p = this->m_buffer;
    if (p != 0) {
        ReleaseBuffer(p);
    }
}