#pragma optimize("t", on)
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