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
void __fastcall CEngine_Release(AutoTinyVirtualObject* object)
{
    delete object;
}
int main()
{
    CEngine_Release(0);
    if (g_AutoTinyVirtualDestructorCalls != 0)
        return 1;
    AutoTinyVirtualObject* object =
        new AutoTinyVirtualObject;
    CEngine_Release(object);
    if (g_AutoTinyVirtualDestructorCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00b23990_TEST PASS\n");
    return 0;
}