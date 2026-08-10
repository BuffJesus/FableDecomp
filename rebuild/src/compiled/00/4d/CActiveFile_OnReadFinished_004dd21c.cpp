#pragma optimize("s",on)
struct CActiveFile;
void __fastcall helper_4db88a(CActiveFile* self);
void __cdecl free_bfea14(void* p);

struct CActiveFile {
    void* field0;
};

void __fastcall OnReadFinished_5(CActiveFile* self)
{
    helper_4db88a(self);
    void* p = self->field0;
    if (p)
        free_bfea14(p);
}