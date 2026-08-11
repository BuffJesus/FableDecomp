#pragma optimize("s",on)
// CActiveFile::OnReadFinished @ 0x00478a02
// __fastcall void OnReadFinished(CActiveFile* this)  (ecx = this)

struct CActiveFile
{
    void* field0; // +0x00
    void* field4; // +0x04
};

// __fastcall helper at 0x478751: ecx = obj, edx = arg
void __fastcall ActiveFile_Dispatch(void* obj, void* arg);

// cdecl free/delete at 0xbfea14
void ActiveFile_Free(void* p);

// this-in-ecx modeled as __fastcall(self, edx-unused)
void __fastcall OnReadFinished(CActiveFile* self)
{
    ActiveFile_Dispatch(self->field0, self->field4);
    if (self->field0)
        ActiveFile_Free(self->field0);
}