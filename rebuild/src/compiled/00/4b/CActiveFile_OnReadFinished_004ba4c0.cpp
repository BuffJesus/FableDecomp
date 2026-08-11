struct CActiveFile {
    void* field0;
};

// base handler (this in ecx) at 0x4b8bb0
void __fastcall CActiveFile_base_handler(CActiveFile* self);
// free cdecl helper at 0xbfea14
void FreeHelper(void* p);

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_handler(self);
    void* p = self->field0;
    if (p)
        FreeHelper(p);
}