extern void __cdecl orf_notify(void*);
struct S { void* p; };
void __fastcall OnReadFinished(S* self)
{
    if (self->p)
        orf_notify(self->p);
}