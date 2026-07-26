#include <cstdio>

struct CObservable {
    unsigned char _pad_0x0[0x13c];
    void* exclusiveObserver;
};

/* Model the observable member function semantics for the behavior test. */
void __fastcall CObservable_AddExclusiveObserver(CObservable* self, void* observer) {
    self->exclusiveObserver = observer;
}

int main() {
    CObservable o;
    o.exclusiveObserver = 0;
    CObservable_AddExclusiveObserver(&o, (void*)0x1234);
    if (o.exclusiveObserver != (void*)0x1234) { std::printf("FAIL set\n"); return 1; }
    std::printf("CObservable_0041ca52_TEST PASS\n");
    return 0;
}