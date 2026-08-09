struct Self { char pad[0x10]; unsigned short r; };
__declspec(noinline) unsigned short __stdcall TreeInsert_A_helper(void* arg);
void __fastcall TreeInsert_A(Self* self, int edx, void* arg)
{
    self->r = TreeInsert_A_helper(arg);
}