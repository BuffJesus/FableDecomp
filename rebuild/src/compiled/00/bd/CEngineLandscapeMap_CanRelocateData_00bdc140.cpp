struct S { char pad[0x5C]; unsigned char f; };
bool __fastcall CanRelocateData(S* self, void* edx, void* arg)
{
    (void)edx; (void)arg;
    return self->f == 0;
}