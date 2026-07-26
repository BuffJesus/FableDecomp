#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall AutoTinyScalarDestructor(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl AutoTinyScalarDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor(self);
    if (flags & 1)
        AutoTinyScalarDelete(self);
    return self;
}
int main()
{
    int object = 0;
    if (CLandscapeBackgroundPatch_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CLandscapeBackgroundPatch_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_009031fa_TEST PASS\n");
    return 0;
}