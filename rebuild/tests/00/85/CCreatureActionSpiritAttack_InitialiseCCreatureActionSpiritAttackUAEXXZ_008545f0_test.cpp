#include <cstdio>

static int g_int = 7;
static float g_mul = 2.5f;
static int helper_called = 0;

int SpiritHelper() { helper_called = 1; return 99; }
int FloatToIntConvert(float f) { return (int)f; }

struct Obj { char pad[0x10]; int field10; };

void Initialise_model(Obj* self)
{
    SpiritHelper();
    self->field10 = FloatToIntConvert((float)g_int * g_mul);
}

int main()
{
    Obj o;
    o.field10 = -1;
    Initialise_model(&o);
    // 7 * 2.5 = 17.5 -> 17
    if (helper_called == 1 && o.field10 == 17) {
        printf("BEHAVIOUR_OK\n");
    } else {
        printf("BEHAVIOUR_FAIL %d %d\n", helper_called, o.field10);
    }
    return 0;
}