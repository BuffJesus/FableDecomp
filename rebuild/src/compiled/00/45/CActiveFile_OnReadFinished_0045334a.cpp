#pragma optimize("s",on)
struct CInner;
extern void __fastcall Inner_Process(CInner* self, int arg, char* out);
extern void __cdecl Free_Buf(void* p);

struct CActiveFile {
    CInner* p;   /* +0x0 */
    int     arg; /* +0x4 */
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Inner_Process(self->p, self->arg, &local);
    if (self->p != 0)
        Free_Buf(self->p);
}