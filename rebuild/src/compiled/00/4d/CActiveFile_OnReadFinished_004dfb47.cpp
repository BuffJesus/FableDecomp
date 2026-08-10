#pragma optimize("s",on)
struct CActiveFile;

// helper called with this in ecx (thiscall modelled as fastcall + dummy edx-less: no stack args)
extern "C" void __fastcall helper_4dd641(CActiveFile* self);
// cdecl free-style function taking one pointer arg (cleaned by caller)
void free_thing_bfea14(void* p);

struct CActiveFile {
    void* payload;   // offset 0
};

void __fastcall OnReadFinished_15(CActiveFile* self)
{
    helper_4dd641(self);
    void* p = self->payload;
    if (p) {
        free_thing_bfea14(p);
    }
}