#include <cstdio>

class CObserver;

class CObservable {
public:
    char pad0[0x30];
    CObserver* m_exclusiveObserver; // +0x30
    void AddExclusiveObserver(CObserver* obs);
};

void CObservable::AddExclusiveObserver(CObserver* obs) {
    m_exclusiveObserver = obs;
}

int main() {
    CObservable o;
    o.m_exclusiveObserver = 0;
    CObserver* fake = reinterpret_cast<CObserver*>(0xDEADBEEF);
    o.AddExclusiveObserver(fake);
    bool ok = (o.m_exclusiveObserver == fake);
    // verify offset 0x30
    char* base = reinterpret_cast<char*>(&o);
    void* atOff = *reinterpret_cast<void**>(base + 0x30);
    ok = ok && (atOff == fake);
    if (ok) printf("EXCLUSIVE_OBS_OK\n");
    else printf("FAIL\n");
    return ok ? 0 : 1;
}