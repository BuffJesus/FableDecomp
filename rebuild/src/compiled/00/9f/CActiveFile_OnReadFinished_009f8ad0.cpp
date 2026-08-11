// CActiveFile::OnReadFinished  @ 009f8ad0  (__fastcall, void)
// Body:
//   push esi; mov esi,ecx
//   call 0x9f89b0            (this->Helper(), __fastcall, ecx=this)
//   mov esi,[esi]            (this->m_ptr)
//   test esi,esi; je end
//   push esi; call 0xbfea14; add esp,4   (FreeIt(this->m_ptr), cdecl)
//   pop esi; ret

extern "C" void __cdecl FreeIt(void* p);

struct CActiveFile
{
    void* m_ptr;         // [ecx+0]
    void Helper();       // external __fastcall, defined elsewhere -> real call
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    Helper();
    void* p = m_ptr;
    if (p)
        FreeIt(p);
}