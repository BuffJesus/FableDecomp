struct CObservable {
    char pad[0x134];
    void* exclusiveObserver;
};

void __fastcall CObservable_AddExclusiveObserver(CObservable* self, void* edx_unused, void* observer)
{
    self->exclusiveObserver = observer;
}