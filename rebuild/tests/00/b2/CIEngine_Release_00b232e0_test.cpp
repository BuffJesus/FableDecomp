#include <cstdio>
struct AutoTinyVirtualObject
{
    virtual ~AutoTinyVirtualObject();
};
static int g_AutoTinyVirtualDestructorCalls = 0;
AutoTinyVirtualObject::~AutoTinyVirtualObject()
{
    ++g_AutoTinyVirtualDestructorCalls;
}
void __fastcall CIEngine_Release(AutoTinyVirtualObject* object)
{
    delete object;
}
int main()
{
    CIEngine_Release(0);
    if (g_AutoTinyVirtualDestructorCalls != 0)
        return 1;
    AutoTinyVirtualObject* object =
        new AutoTinyVirtualObject;
    CIEngine_Release(object);
    if (g_AutoTinyVirtualDestructorCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00b232e0_TEST PASS\n");
    return 0;
}