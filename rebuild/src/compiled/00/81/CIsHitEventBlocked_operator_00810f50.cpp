struct Y { char pad[0x26]; unsigned char flag; };
struct X { char pad[8]; Y* y; };
unsigned char __fastcall IsHitBlockedCall(void* self, void* edx, X* arg)
{
    (void)self; (void)edx;
    return arg->y->flag;
}