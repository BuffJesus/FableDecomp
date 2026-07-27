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
void __fastcall DeleteData(AutoTinyVirtualObject* object)
{
    delete object;
}
int main()
{
    DeleteData(0);
    if (g_AutoTinyVirtualDestructorCalls != 0)
        return 1;
    AutoTinyVirtualObject* object =
        new AutoTinyVirtualObject;
    DeleteData(object);
    if (g_AutoTinyVirtualDestructorCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00a110d0_TEST PASS\n");
    return 0;
}