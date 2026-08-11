// CActiveFile::OnReadFinished @ 0062e8e0
struct CActiveFile;

void __fastcall base_0x62e2b0(CActiveFile* self);   // 0x62e2b0
void free_0xbfea14(void* p);                          // 0xbfea14 cdecl

struct CActiveFile {
    void* field0;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    base_0x62e2b0(self);
    void* p = *(void**)self;   // esi = [esi] ; reload from this
    if (p) {
        free_0xbfea14(p);
    }
}