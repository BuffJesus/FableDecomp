struct CObservable { void* pad[0x50]; void* exclusiveObserver; };

void __fastcall CObservable_AddExclusiveObserver(CObservable* self, void* /*edx*/, void* observer)
{
    *(void**)((char*)self + 0x13c) = observer;
}