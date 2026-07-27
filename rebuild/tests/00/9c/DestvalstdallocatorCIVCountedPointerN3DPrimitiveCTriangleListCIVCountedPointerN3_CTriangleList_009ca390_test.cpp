#include <cstdio>
struct AutoTinyCountedObject
{
    void** vtable;
    long references;
};
struct AutoTinyCountedHandle
{
    AutoTinyCountedObject* object;
    void Reset();
};
static int g_AutoTinyCountedReleaseCalls = 0;
static AutoTinyCountedObject* g_AutoTinyCountedReleasedObject = 0;
void __fastcall AutoTinyCountedRelease(AutoTinyCountedObject* object)
{
    ++g_AutoTinyCountedReleaseCalls;
    g_AutoTinyCountedReleasedObject = object;
}
void AutoTinyCountedHandle::Reset()
{
    AutoTinyCountedObject* current = object;
    if (current != 0)
    {
        if (--current->references == 0)
            ((void (__fastcall*)(AutoTinyCountedObject*))current->vtable[1])(current);
        object = 0;
    }
}
int main()
{
    void* vtable[2] = {0, (void*)AutoTinyCountedRelease};
    AutoTinyCountedObject last = {vtable, 1};
    AutoTinyCountedHandle lastHandle = {&last};
    lastHandle.Reset();
    if (lastHandle.object != 0 || last.references != 0 ||
        g_AutoTinyCountedReleaseCalls != 1 || g_AutoTinyCountedReleasedObject != &last)
        return 1;
    AutoTinyCountedObject retained = {vtable, 2};
    AutoTinyCountedHandle retainedHandle = {&retained};
    retainedHandle.Reset();
    if (retainedHandle.object != 0 || retained.references != 1 ||
        g_AutoTinyCountedReleaseCalls != 1)
        return 1;
    AutoTinyCountedHandle empty = {0};
    empty.Reset();
    if (g_AutoTinyCountedReleaseCalls != 1)
        return 1;
    std::printf("AUTO_TINY_009ca390_TEST PASS\n");
    return 0;
}