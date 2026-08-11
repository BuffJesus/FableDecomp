// CActiveFile::OnReadFinished  @ 0x00a0a880  (__fastcall, void)

extern "C" void __cdecl free_buf(void *p);   // 0xbfea14 (cdecl free/operator delete)

struct CActiveFile
{
    void *m_buffer;          // +0x00
    void PreFinish();        // 0xa0a720 (__fastcall member)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    PreFinish();
    void *p = m_buffer;
    if (p)
        free_buf(p);
}