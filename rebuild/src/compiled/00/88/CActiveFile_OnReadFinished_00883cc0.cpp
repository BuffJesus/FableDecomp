// CActiveFile::OnReadFinished  retail 0x00883cc0
extern "C" void __cdecl free_helper(void* p);  // maps to operator delete @0xbfea14

struct CActiveFile {
    void* m_buffer;   // +0x00

    void Helper();          // __fastcall @0x882c40 (extern, forces a real call)
    void OnReadFinished();  // __fastcall @0x883cc0
};

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->m_buffer;
    if (p)
        free_helper(p);
}