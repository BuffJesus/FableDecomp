#include <cstdio>

struct CPhysicsDef { int a; int b; };

static int g_calls = 0;
static CPhysicsDef* g_lastSelf = 0;
static const CPhysicsDef* g_lastOther = 0;

extern "C" void __fastcall CPhysicsDef_CopyImpl(CPhysicsDef* self, void* /*edx*/, const CPhysicsDef* other)
{
    g_calls++;
    g_lastSelf = self;
    g_lastOther = other;
    self->a = other->a;
    self->b = other->b;
}

void __fastcall CPhysicsDef_Copy(CPhysicsDef* self, void* edx, const CPhysicsDef* other)
{
    CPhysicsDef_CopyImpl(self, edx, other);
}

int main()
{
    CPhysicsDef dst; dst.a = 0; dst.b = 0;
    CPhysicsDef src; src.a = 7; src.b = 42;
    CPhysicsDef_Copy(&dst, 0, &src);
    if (g_calls == 1 && g_lastSelf == &dst && g_lastOther == &src && dst.a == 7 && dst.b == 42) {
        std::printf("CPhysicsDef_004537a0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d a=%d b=%d\n", g_calls, dst.a, dst.b);
    return 1;
}