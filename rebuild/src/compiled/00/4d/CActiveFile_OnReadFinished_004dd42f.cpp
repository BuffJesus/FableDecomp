#pragma optimize("s",on)
struct CActiveFile { void* field0; };

void __fastcall Helper_4dbad3(CActiveFile* self);
void Free_bfea14(void* p);

void __fastcall OnReadFinished_10(CActiveFile* self)
{
    Helper_4dbad3(self);
    void* p = self->field0;
    if (p)
        Free_bfea14(p);
}