#pragma optimize("s",on)
struct CActiveFile {
    void* p;     // +0x0
    int field4;  // +0x4
};

extern int __fastcall CActiveFile_cb(void* ecx, int edx, char* arg);
extern void __cdecl CActiveFile_free(void* q);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char buf;
    CActiveFile_cb(self->p, self->field4, &buf);
    void* q = self->p;
    if (q)
        CActiveFile_free(q);
}