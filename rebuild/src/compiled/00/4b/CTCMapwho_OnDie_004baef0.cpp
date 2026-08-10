struct CTCMapwho;
extern "C" void __fastcall Sub_4b9b90(CTCMapwho* self);
extern "C" void FreeFn_bfe9bc(CTCMapwho* self);
void __fastcall OnDie_3(CTCMapwho* self)
{
    if (self)
    {
        Sub_4b9b90(self);
        FreeFn_bfe9bc(self);
    }
}