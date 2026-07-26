#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
struct AutoTinyVectorObject
{
    void* vftable;
};
void* const g_AutoTinyVectorVftable = (void*)0x12345678;
void __fastcall AutoTinyVectorDestructor(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl AutoTinyVectorDelete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall vector_deleting_destructor(AutoTinyVectorObject* self, int, unsigned int flags)
{
    self->vftable = (void*)&g_AutoTinyVectorVftable;
    AutoTinyVectorDestructor(self);
    if (flags & 1)
        AutoTinyVectorDelete(self);
    return self;
}
int main()
{
    AutoTinyVectorObject object = {0};
    if (vector_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_AutoTinyVectorVftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    vector_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004eafa7_TEST PASS\n");
    return 0;
}