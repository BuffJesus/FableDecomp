#pragma optimize("s", on)
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