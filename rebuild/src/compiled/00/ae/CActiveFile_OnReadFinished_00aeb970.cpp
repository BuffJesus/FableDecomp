// CActiveFile::OnReadFinished @ 0x00aeb970
// push esi; mov esi,ecx; call 0xaec840 (thiscall member Helper);
// mov esi,[esi]; test esi,esi; je end;
// push esi; call 0xbfea14 (cdecl free); add esp,4; ret

struct CActiveFile
{
    void* field0;
    void Helper();          // 0xaec840 thiscall on this (external, not inlined)
    void OnReadFinished();  // 0xaeb970
};

extern "C" void __cdecl FreeMem(void* p); // 0xbfea14

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->field0;
    if (p)
        FreeMem(p);
}