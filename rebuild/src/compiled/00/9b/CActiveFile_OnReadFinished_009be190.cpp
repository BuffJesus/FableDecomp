// CActiveFile::OnReadFinished @ 009be190
void some_free(void* p);   // cdecl free @ 0xbfea14

struct CActiveFile {
    void* buffer;   // +0x00
    void Helper();  // __fastcall @ 0x9be060, non-inlined
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->buffer;
    if (p)
        some_free(p);
}