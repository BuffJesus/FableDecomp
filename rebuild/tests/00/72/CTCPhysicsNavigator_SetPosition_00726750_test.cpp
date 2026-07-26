#include <cstdio>
struct Vec3 { float x, y, z; };
static int g_called = 0;
int __fastcall CTCPhysicsNavigator_Notify(void* self, int edxpass, const Vec3* v)
{
    (void)self; (void)edxpass; (void)v; g_called = 1; return 0;
}
void __fastcall CTCPhysicsNavigator_SetPosition(void* self, int edxpass, const Vec3* v)
{
    CTCPhysicsNavigator_Notify(self, edxpass, v);
    *(Vec3*)((char*)self + 0x89) = *v;
}
int main()
{
    char buf[0x100];
    Vec3 v; v.x = 1.5f; v.y = -2.0f; v.z = 3.25f;
    CTCPhysicsNavigator_SetPosition(buf, 0, &v);
    Vec3* p = (Vec3*)(buf + 0x89);
    if (g_called == 1 && p->x == 1.5f && p->y == -2.0f && p->z == 3.25f) {
        std::printf("CTCPhysicsNavigator_00726750_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}