struct CActiveFile {
    void* field0;
};

// member at 0x742290, this-in-ecx, no other args
void __fastcall CActiveFile_Base(CActiveFile* self);
// cdecl free function at 0xbfea14
void CActiveFile_FreeHelper(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base(self);
    void* p = self->field0;
    if (p)
        CActiveFile_FreeHelper(p);
}