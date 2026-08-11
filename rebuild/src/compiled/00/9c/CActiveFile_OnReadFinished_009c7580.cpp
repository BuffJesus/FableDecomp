// CActiveFile::OnReadFinished  @ 009c7580  __fastcall void(CActiveFile*)
// Layout: field at +0 is a heap pointer freed after a member call.

extern "C" void __cdecl free_helper(void* p);   // cdecl -> 0xbfea14

struct CActiveFile
{
    void* m_ptr;          // +0

    void Helper();        // __fastcall -> 0x9c71a0
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    Helper();
    void* p = m_ptr;
    if (p)
        free_helper(p);
}