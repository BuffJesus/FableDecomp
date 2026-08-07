struct Sub; extern int __fastcall cat_probe(Sub*);
struct S { char pad[0x74]; Sub* sub; };
bool __fastcall CanAnimationTransitionOut(S* self)
{
    return (~cat_probe(self->sub)) & 1;
}