// NUISystem::CObservable::AddExclusiveObserver  @ 0x005d8e40
// mov eax,[esp+4]; mov [ecx+0x10],eax; ret 4
// __fastcall setter: this->exclusiveObserver (offset 0x10) = observer

struct CObserver;

struct CObservable {
    char pad[0x10];
    CObserver* exclusiveObserver; // +0x10
};

void __fastcall AddExclusiveObserver(CObservable* self, void* /*edx*/, CObserver* observer)
{
    self->exclusiveObserver = observer;
}