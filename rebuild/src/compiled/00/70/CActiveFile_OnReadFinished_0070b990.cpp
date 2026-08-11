// CActiveFile::OnReadFinished @ 0070b990
// base_70b8a0(this); p = *this; if(p) free_bfea14(p);

struct CActiveFile;

extern "C" void __fastcall base_70b8a0(CActiveFile* self);
extern "C" void free_bfea14(void* p);

struct CActiveFile {
    void* ptr; // +0
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_70b8a0(self);
    void* p = *(void**)self;
    if (p)
        free_bfea14(p);
}