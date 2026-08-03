extern int g_divisor;

struct Obj {
    char pad[0x14];
    int count;
    char flag;
};

char __fastcall Comparison_Float_WithEpsilon(Obj* self, void* edx, float arg)
{
    if (self->flag) return 1;
    return (float)self->count / g_divisor > arg;
}