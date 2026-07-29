struct CObserver;

struct CObservable_0066fa50
{
    unsigned char prefix[0x24];
    CObserver* exclusive_observer;
};

extern "C" void __fastcall
CObservable_AddExclusiveObserver_0066fa50(
    CObservable_0066fa50* self, void*, CObserver* observer)
{
    self->exclusive_observer = observer;
}
