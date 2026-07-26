struct CIME { char pad[8]; char enabled; };

extern void __fastcall CIME_helper(void* p, int q);

void __fastcall CIME_FinaliseString(CIME* self, int edx_ignore, void* a, int b)
{
    if (self->enabled)
        CIME_helper(a, b);
}