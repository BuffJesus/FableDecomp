#include <cstdio>
typedef int EWeaponClass;
struct Iter8 { EWeaponClass* a; EWeaponClass* b; };

static Iter8* g_last_tmp;
static int g_last_first;
static void* g_last_self;

EWeaponClass* callee_a98540(void* self, int first, Iter8* tmp)
{
    g_last_self = self;
    g_last_first = first;
    g_last_tmp = tmp;
    return tmp->a;
}

EWeaponClass* f_model(void* self, int first)
{
    Iter8 tmp;
    tmp.a = 0;
    tmp.b = 0;
    return callee_a98540(self, first, &tmp);
}

int main()
{
    EWeaponClass* r = f_model((void*)0x1234, 0x5678);
    if (g_last_first == 0x5678 && g_last_self == (void*)0x1234 &&
        g_last_tmp->a == 0 && g_last_tmp->b == 0 && r == 0) {
        printf("PARITY_OK_a98600\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}