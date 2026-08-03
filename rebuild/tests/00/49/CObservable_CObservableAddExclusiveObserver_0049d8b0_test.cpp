#include <cstdio>

struct CObservable { char pad[0xA0]; void* exclusive; };

void __fastcall CObservable_AddExclusiveObserver(CObservable* self, int edx, void* observer)
{
    self->exclusive = observer;
}

int main()
{
    CObservable o;
    int marker = 42;
    o.exclusive = 0;
    CObservable_AddExclusiveObserver(&o, 0, &marker);
    if (o.exclusive != &marker) { std::printf("FAIL: exclusive not set\n"); return 1; }
    std::printf("OBSERVER_SET_OK\n");
    return 0;
}