// CActiveFile::OnReadFinished @ 009be150
// push esi; mov esi,ecx; call 0x9be060 (thiscall method);
// mov esi,[esi]; test/je; push esi; call 0xbfea14 (cdecl free); add esp,4

extern "C" void __cdecl FreeBuffer(void* p); // 0xbfea14

struct CActiveFile {
    void* m_buffer; // +0x00

    void Helper();  // 0x9be060 (thiscall)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* buf = this->m_buffer;
    if (buf)
        FreeBuffer(buf);
}