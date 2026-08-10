struct CTCMapwho;

// thiscall method on self (ecx=this, no other args)
void __fastcall CTCMapwho_inner(CTCMapwho *self);
// cdecl free function taking self on the stack
void CTCMapwho_free(CTCMapwho *p);

void __fastcall CTCMapwho_OnDie(CTCMapwho *self)
{
    if (self)
    {
        CTCMapwho_inner(self);
        CTCMapwho_free(self);
    }
}