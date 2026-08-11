// CActiveFile::OnReadFinished  0x00cdb530
// push esi; mov esi,ecx; call 0xcd4df0 (thiscall this); mov esi,[esi];
// test esi,esi; je end; push esi; call 0xbfea14 (cdecl free); add esp,4; end.

void __cdecl FreeThing(void* p);   // 0xbfea14

struct CActiveFile
{
    void* field0;      // +0x00
    void Helper();     // 0xcd4df0 (thiscall)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    this->Helper();
    void* p = this->field0;
    if (p)
        FreeThing(p);
}