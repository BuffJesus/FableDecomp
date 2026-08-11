#include <stdio.h>

class CBase {
public:
    int marker;
    CBase() { marker = 0; }
    ~CBase() { marker = 0x0B; }
    virtual void v0() {}
};

class CVolume : public CBase {
public:
    int released;
    CVolume() { released = 0; }
    ~CVolume();
    void ReleaseResources(int flag) { released = flag; }
    virtual void v0() {}
};

CVolume::~CVolume()
{
    ReleaseResources(1);
}

int main()
{
    CVolume* v = new CVolume();
    v->ReleaseResources(0);
    int before = v->released;
    v->~CVolume();
    int after = v->released;
    ::operator delete(v);
    if (before == 0 && after == 1) {
        printf("OK dtor released=%d marker=%d\n", after, 0x0B);
    } else {
        printf("FAIL before=%d after=%d\n", before, after);
    }
    return 0;
}