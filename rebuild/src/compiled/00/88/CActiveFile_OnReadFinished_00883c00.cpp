// CActiveFile::OnReadFinished @ 0x00883c00

void __fastcall sub_882b80(void* self);   // 0x882b80, __fastcall (ecx=self)
void __cdecl sub_bfea14(void* p);         // 0xbfea14, cdecl one-arg (delete/free)

struct CActiveFile {
    void* field0;
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    sub_882b80(this);
    void* p = this->field0;
    if (p) {
        sub_bfea14(p);
    }
}