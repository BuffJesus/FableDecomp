#pragma optimize("s",on)
struct CActiveFile;
extern void __fastcall CActiveFile_helper_004dba39(CActiveFile* self);
extern void __cdecl CActiveFile_free_00bfea14(void* p);
struct CActiveFile { void* m_ptr; };
void __fastcall OnReadFinished_8(CActiveFile* self)
{
    CActiveFile_helper_004dba39(self);
    void* p = self->m_ptr;
    if (p)
        CActiveFile_free_00bfea14(p);
}