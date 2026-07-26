struct CObservable { char pad[0x138]; void* exclusiveObserver; };

void __fastcall CObservable_AddExclusiveObserver(CObservable* self, int edx_unused, void* observer)
{
    self->exclusiveObserver = observer;
}