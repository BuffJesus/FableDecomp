// CActiveFile::OnReadFinished  0x00a17570

extern "C" void __cdecl ReleaseBuf(void* p);  // 0xbfea14

struct CActiveFile {
    void* m_ptr;      // [this+0x00]
    void Notify();    // 0xa17390 (__fastcall on this)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    Notify();
    void* p = m_ptr;
    if (p)
        ReleaseBuf(p);
}