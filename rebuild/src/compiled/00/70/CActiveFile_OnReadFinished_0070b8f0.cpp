struct CActiveFile {
    void* field0;
};

// __fastcall base method at 0x70b8a0 (same module)
void __fastcall CActiveFile_base(CActiveFile* self);

// cdecl free function at 0xbfea14
extern "C" void free_func(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->field0;
    if (p) {
        free_func(p);
    }
}