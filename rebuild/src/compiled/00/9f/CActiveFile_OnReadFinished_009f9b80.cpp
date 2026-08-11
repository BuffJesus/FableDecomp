// CActiveFile::OnReadFinished @ 009f9b80  __fastcall void(CActiveFile*)
extern "C" void __cdecl free_impl(void* p);  // 0xbfea14

struct CActiveFile {
    void* buffer;    // +0x00
    void Helper();   // 0x9f9b30
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->buffer;
    if (p)
        free_impl(p);
}