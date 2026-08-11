extern "C" void free(void* p);   // cdecl free -> 0xbfea14

struct CActiveFile;
void __fastcall base_OnReadFinished(CActiveFile* self);   // 0x5356a0, thiscall base

struct CActiveFile {
    void* buffer;   // +0
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_OnReadFinished(self);
    void* p = self->buffer;
    if (p) {
        free(p);
    }
}