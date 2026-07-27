#include <cstdio>
static int g_AutoTinySequence[6] = {0};
static int g_AutoTinySequenceCount = 0;
static void* g_AutoTinyFirstMember = 0;
static void* g_AutoTinySecondMember = 0;
static int g_AutoTinyCompositeDeleteCalls = 0;
void __fastcall AutoTinyFirstMemberDestructor(void* member)
{
    g_AutoTinyFirstMember = member;
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 1;
}
void __fastcall AutoTinySecondMemberDestructor(void* member)
{
    g_AutoTinySecondMember = member;
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 2;
}
void __fastcall AutoTinyCompositeDestructor(void*)
{
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 3;
}
void __cdecl AutoTinyCompositeDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor(static_cast<unsigned char*>(self) + 72);
    AutoTinySecondMemberDestructor(static_cast<unsigned char*>(self) + 52);
    AutoTinyCompositeDestructor(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}
int main()
{
    unsigned char object[256] = {0};
    if (CThingSwitch_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyFirstMember != object + 72 ||
        g_AutoTinySecondMember != object + 52 ||
        g_AutoTinySequenceCount != 3 ||
        g_AutoTinySequence[0] != 1 ||
        g_AutoTinySequence[1] != 2 ||
        g_AutoTinySequence[2] != 3 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CThingSwitch_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinySequenceCount != 6 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00df9b50_TEST PASS\n");
    return 0;
}