#include <cstdio>

#include "../../../src/compiled/00/52/CObserver_ObserveEvent_0052da20.cpp"

static int g_Sequence;
static int g_FirstSequence;
static int g_SecondSequence;
static int g_CallbackSequence;
static int g_FirstEvent;
static int g_SecondEvent;
static int g_CallbackEvent;
static void* g_FirstResult;
static void* g_SecondResult;
static void* g_FirstCollection;
static void* g_SecondCollection;
static CObserver_0052da20* g_CallbackObserver;

extern "C" void __fastcall
ObserverLookupFirst_0052da20(
    void* collection,
    void*,
    void** result,
    const int* event)
{
    g_FirstSequence = ++g_Sequence;
    g_FirstCollection = collection;
    g_FirstEvent = *event;
    *result = g_FirstResult;
}

extern "C" void __fastcall
ObserverLookupSecond_0052da20(
    void* collection,
    void*,
    void** result,
    const int* event)
{
    g_SecondSequence = ++g_Sequence;
    g_SecondCollection = collection;
    g_SecondEvent = *event;
    *result = g_SecondResult;
}

extern "C" void __fastcall
ObserverVirtualCallback_0052da20(
    CObserver_0052da20* observer,
    void*,
    int event)
{
    g_CallbackSequence = ++g_Sequence;
    g_CallbackObserver = observer;
    g_CallbackEvent = event;
}

static void ResetProbe(void* first_result, void* second_result)
{
    g_Sequence = 0;
    g_FirstSequence = 0;
    g_SecondSequence = 0;
    g_CallbackSequence = 0;
    g_FirstEvent = 0;
    g_SecondEvent = 0;
    g_CallbackEvent = 0;
    g_FirstResult = first_result;
    g_SecondResult = second_result;
    g_FirstCollection = 0;
    g_SecondCollection = 0;
    g_CallbackObserver = 0;
}

int main()
{
    int failures = 0;
    void* vtable[2];
    int sentinel = 1;
    int other = 2;
    CObserver_0052da20 observer;

    vtable[0] = 0;
    vtable[1] = (void*)&ObserverVirtualCallback_0052da20;
    observer.vtable = vtable;
    observer.collection_sentinel = &sentinel;

    ResetProbe(&sentinel, &other);
    observer.ObserveEvent(0x19);
    if (g_FirstSequence != 1 ||
        g_SecondSequence != 2 ||
        g_CallbackSequence != 3 ||
        g_FirstEvent != 0x19 ||
        g_SecondEvent != 0x19 ||
        g_CallbackEvent != 0x19 ||
        g_FirstCollection != &observer.collection_sentinel ||
        g_SecondCollection != &observer.collection_sentinel ||
        g_CallbackObserver != &observer)
    {
        std::printf("matching event lookup/callback order failed\n");
        ++failures;
    }

    ResetProbe(&sentinel, &other);
    observer.ObserveEvent(0x18);
    if (g_FirstSequence != 1 ||
        g_SecondSequence != 2 ||
        g_CallbackSequence != 0)
    {
        std::printf("non-special event filtering failed\n");
        ++failures;
    }

    ResetProbe(&other, &sentinel);
    observer.ObserveEvent(0x19);
    if (g_FirstSequence != 1 ||
        g_SecondSequence != 2 ||
        g_CallbackSequence != 0)
    {
        std::printf("non-sentinel lookup filtering failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "OBSERVER_0052da20_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("OBSERVER_0052da20_TEST PASS\n");
    return 0;
}
