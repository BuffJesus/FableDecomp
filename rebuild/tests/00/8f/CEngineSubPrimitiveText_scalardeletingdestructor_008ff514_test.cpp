#include <cstdio>
static int g_AutoTinyMemberDestructorCalls = 0;
static int g_AutoTinyCompositeDestructorCalls = 0;
static int g_AutoTinyCompositeDeleteCalls = 0;
static void* g_AutoTinyObservedMember = 0;
void __fastcall AutoTinyMemberDestructor(void* member)
{
    ++g_AutoTinyMemberDestructorCalls;
    g_AutoTinyObservedMember = member;
}
void __fastcall AutoTinyCompositeDestructor(void*)
{
    ++g_AutoTinyCompositeDestructorCalls;
}
void __cdecl AutoTinyCompositeDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor(static_cast<unsigned char*>(self) + 20);
    AutoTinyCompositeDestructor(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}
int main()
{
    unsigned char object[256] = {0};
    if (CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyObservedMember != object + 20 ||
        g_AutoTinyMemberDestructorCalls != 1 ||
        g_AutoTinyCompositeDestructorCalls != 1 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinyMemberDestructorCalls != 2 ||
        g_AutoTinyCompositeDestructorCalls != 2 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_008ff514_TEST PASS\n");
    return 0;
}