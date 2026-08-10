struct CTCMapwho;

extern "C" void helper_9f9020(void);
extern "C" void free_bfe9bc(CTCMapwho* p);

void __fastcall OnDie(CTCMapwho* self)
{
    if (self)
    {
        helper_9f9020();
        free_bfe9bc(self);
    }
}