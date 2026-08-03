#include <cstdio>

struct CObserver;

struct CObservable {
    char pad[0x10];
    CObserver* exclusiveObserver; // +0x10
};

void __fastcall AddExclusiveObserver(CObservable* self, void* /*edx*/, CObserver* observer)
{
    self->exclusiveObserver = observer;
}

int main()
{
    CObservable obj;
    obj.exclusiveObserver = (CObserver*)0;
    CObserver* obs = (CObserver*)0xDEADBEEF;

    // __fastcall: this=ecx, one stack arg, callee cleans 4 bytes (ret 4)
    void* pself = &obj;
    void* parg = obs;
    __asm {
        mov ecx, pself
        push parg
        call AddExclusiveObserver
    }

    if (obj.exclusiveObserver == (CObserver*)0xDEADBEEF) {
        printf("OK_0x005d8e40\n");
        return 0;
    }
    printf("FAIL got=%p\n", (void*)obj.exclusiveObserver);
    return 1;
}