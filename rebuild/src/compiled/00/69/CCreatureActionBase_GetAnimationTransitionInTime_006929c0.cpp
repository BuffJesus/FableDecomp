struct Foo { long Method(long arg); };
struct Sub { };
__declspec(noinline) long __fastcall Sub_Helper(Sub* s);

struct CCreatureActionBase {
    char pad0[8];
    Sub  sub;
    char pad1[0x74 - 8 - sizeof(Sub)];
    Foo* f74;
};

long __fastcall GetAnimationTransitionInTime(CCreatureActionBase* self)
{
    Foo* f = self->f74;
    long r = Sub_Helper(&self->sub);
    return f->Method(r);
}