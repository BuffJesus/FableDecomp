#include <cstdio>
#include <cstring>

struct CObservable {
    char pad[0x18c];
    void* m_exclusiveObserver;
};

void __fastcall AddExclusiveObserver(CObservable* self, void* /*edx*/, void* observer)
{
    self->m_exclusiveObserver = observer;
}

int main()
{
    CObservable obj;
    memset(&obj, 0, sizeof(obj));
    int dummyObserver = 0;
    void* observer = &dummyObserver;

    // Invoke via __asm shim: this=ecx, one stack arg, ret 4.
    void* self = &obj;
    __asm {
        mov  ecx, self
        push observer
        call AddExclusiveObserver
    }

    if (obj.m_exclusiveObserver != observer) {
        printf("FAIL: field=%p expected=%p\n", obj.m_exclusiveObserver, observer);
        return 1;
    }
    printf("OK_0x00556570\n");
    return 0;
}