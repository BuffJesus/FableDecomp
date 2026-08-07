extern int __stdcall gfr_conv(int);
struct S { char pad[8]; int a; char pad2[4]; int b; };
int __fastcall GetFramesRemaining(S* self)
{
    return gfr_conv(self->a - self->b);
}