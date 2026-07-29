#include <stdio.h>
#include <string.h>

struct CObserver
{
    unsigned long marker;
};

struct CObservable_0066fa50
{
    unsigned char prefix[0x24];
    CObserver* exclusive_observer;
    unsigned long suffix;
};

extern "C" void __fastcall
CObservable_AddExclusiveObserver_0066fa50(
    CObservable_0066fa50* self, void*, CObserver* observer);

static int Check(int condition, const char* message)
{
    if (!condition)
    {
        printf("FAIL: %s\n", message);
        return 0;
    }
    return 1;
}

int main()
{
    int ok = 1;
    CObservable_0066fa50 observable;
    CObserver first;
    CObserver second;

    memset(&observable, 0x5a, sizeof(observable));
    first.marker = 1;
    second.marker = 2;

    CObservable_AddExclusiveObserver_0066fa50(
        &observable, 0, &first);
    ok &= Check(observable.exclusive_observer == &first,
                "setter must store the observer at offset 0x24");
    ok &= Check(observable.prefix[0] == 0x5a &&
                observable.prefix[0x23] == 0x5a &&
                observable.suffix == 0x5a5a5a5a,
                "setter must preserve adjacent object state");

    CObservable_AddExclusiveObserver_0066fa50(
        &observable, 0, &second);
    ok &= Check(observable.exclusive_observer == &second,
                "setter must replace an existing observer");

    CObservable_AddExclusiveObserver_0066fa50(
        &observable, 0, 0);
    ok &= Check(observable.exclusive_observer == 0,
                "setter must accept a null observer");

    if (!ok)
        return 1;

    puts("PASS CObservable::AddExclusiveObserver 0x0066FA50");
    return 0;
}
