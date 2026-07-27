#include <cstdio>
struct AutoTinyCountedObject
{
    void** vtable;
    long references;
};
struct AutoTinyCountedHandle
{
    AutoTinyCountedObject* object;
    void Assign(AutoTinyCountedHandle* source);
};
static int g_AutoTinyCountedReleaseCalls = 0;
static AutoTinyCountedObject* g_AutoTinyCountedReleasedObject = 0;
void __fastcall AutoTinyCountedRelease(AutoTinyCountedObject* object)
{
    ++g_AutoTinyCountedReleaseCalls;
    g_AutoTinyCountedReleasedObject = object;
}
void AutoTinyCountedHandle::Assign(AutoTinyCountedHandle* source)
{
    AutoTinyCountedObject* current = object;
    if (current != source->object)
    {
        if (current != 0)
        {
            if (--current->references == 0)
                ((void (__fastcall*)(AutoTinyCountedObject*))current->vtable[1])(current);
            object = 0;
        }
        AutoTinyCountedObject* incoming = source->object;
        object = incoming;
        if (incoming != 0)
            ++incoming->references;
    }
}
int main()
{
    void* vtable[2] = {0, (void*)AutoTinyCountedRelease};
    AutoTinyCountedObject last = {vtable, 1};
    AutoTinyCountedObject incoming = {vtable, 2};
    AutoTinyCountedHandle destination = {&last};
    AutoTinyCountedHandle source = {&incoming};
    destination.Assign(&source);
    if (destination.object != &incoming || last.references != 0 || incoming.references != 3 ||
        g_AutoTinyCountedReleaseCalls != 1 || g_AutoTinyCountedReleasedObject != &last)
        return 1;
    destination.Assign(&destination);
    if (incoming.references != 3 || g_AutoTinyCountedReleaseCalls != 1)
        return 1;
    AutoTinyCountedObject retained = {vtable, 2};
    AutoTinyCountedHandle retainedHandle = {&retained};
    AutoTinyCountedHandle empty = {0};
    retainedHandle.Assign(&empty);
    if (retainedHandle.object != 0 || retained.references != 1 ||
        g_AutoTinyCountedReleaseCalls != 1)
        return 1;
    std::printf("AUTO_TINY_007be73a_TEST PASS\n");
    return 0;
}