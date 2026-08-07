struct S;
extern int __fastcall isf_count(S*);
struct S { char pad[8]; int v; };
int __fastcall IsFinished(S* self)
{
    return isf_count(self) + self->v;
}