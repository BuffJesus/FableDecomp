struct S { char pad[0xAC]; void* o; };
void __fastcall AddExclusiveObserver(S* self, void* edx, void* arg)
{
    (void)edx;
    self->o = arg;
}