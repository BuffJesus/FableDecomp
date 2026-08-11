// CActiveFile::OnReadFinished  @ 00b83950
// void __fastcall(CActiveFile* this)

struct CActiveFile
{
    void* field0;     // [this+0]
    void ProcessRead();   // 0xb836e0, __fastcall no-arg
    void OnReadFinished();
};

extern "C" void __cdecl ReleaseBuffer(void* p);  // 0xbfea14, cdecl free-like

void CActiveFile::OnReadFinished()
{
    this->ProcessRead();
    void* p = this->field0;
    if (p)
        ReleaseBuffer(p);
}