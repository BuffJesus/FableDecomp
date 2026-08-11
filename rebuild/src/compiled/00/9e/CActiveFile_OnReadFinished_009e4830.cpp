// CActiveFile::OnReadFinished @ 009e4830

extern "C" void __cdecl freefn_bfea14(void* p);

struct CActiveFile {
    void* buffer;   // +0
    void helper();  // 0x9e4710
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    this->helper();
    void* p = this->buffer;
    if (p != 0)
    {
        freefn_bfea14(p);
    }
}