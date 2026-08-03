struct MI { int a; int b; int c; int d; int e; char f; };
// retail is __fastcall: this=ecx, src=[esp+4], ret 4, returns this in eax.
// harness rewrites __fastcall->__fastcall; give an edx dummy so src lands on the stack.
struct MI* __fastcall CHeroMarriageInfo(struct MI* self, int /*edx*/, const struct MI* src)
{
    self->a = src->a;
    self->b = src->b;
    self->c = src->c;
    self->d = src->d;
    self->e = src->e;
    self->f = src->f;
    return self;
}