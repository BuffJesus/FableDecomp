// CActiveFile::OnReadFinished  @ 009c6ec0
// void __fastcall(CActiveFile*)

void __cdecl freemem(void* p);   // 0xbfea14 - cdecl deallocator

struct CActiveFile {
    void* buffer;   // +0x00
    void Helper();  // 0x9c6cc0 - __fastcall on this
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    Helper();
    void* p = buffer;
    if (p)
        freemem(p);
}