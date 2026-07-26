struct CObservable { char pad[0x134]; void* exclusive; };

void __fastcall CObservable_AddExclusiveObserver(CObservable* self, int edx, void* observer)
{
    self->exclusive = observer;
}