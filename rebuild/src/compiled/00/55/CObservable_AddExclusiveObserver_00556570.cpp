// NUISystem::CObservable::AddExclusiveObserver @ 0x00556570
// mov eax,[esp+4]; mov [ecx+0x18c],eax; ret 4
// __fastcall: store observer pointer into field at +0x18C.
struct CObservable {
    char pad[0x18c];
    void* m_exclusiveObserver;
};

void __fastcall AddExclusiveObserver(CObservable* self, void* /*edx*/, void* observer)
{
    self->m_exclusiveObserver = observer;
}