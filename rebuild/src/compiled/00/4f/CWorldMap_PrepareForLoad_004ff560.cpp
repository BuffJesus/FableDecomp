extern void __stdcall pfl_helper(void* p, int z);
struct S { char pad[0x9c]; void* f9c; };
void __fastcall PrepareForLoad(S* self){ pfl_helper(self->f9c, 0); }