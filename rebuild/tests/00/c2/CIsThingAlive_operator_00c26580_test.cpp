#include <cstdio>

struct Inner {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual bool v8(void* payload);
};

struct CIsThingAlive {
    char pad[0x74];
    unsigned short f74;
    bool operator()(void* arg);
};

bool CIsThingAlive::operator()(void* arg)
{
    this->f74 = 2;
    Inner* sub = (Inner*)((char*)arg + 0x2c);
    return sub->v8(this);
}

void Inner::v0() {} void Inner::v1() {} void Inner::v2() {} void Inner::v3() {}
void Inner::v4() {} void Inner::v5() {} void Inner::v6() {} void Inner::v7() {}
bool Inner::v8(void*) { return false; }

static void* g_payload = 0;
struct FakeInner : Inner {
    void v0() {} void v1() {} void v2() {} void v3() {}
    void v4() {} void v5() {} void v6() {} void v7() {}
    bool v8(void* payload) { g_payload = payload; return true; }
};

int main() {
    // Base object: FakeInner embedded at +0x2c
    struct Base { char head[0x2c]; FakeInner inner; } base;

    CIsThingAlive obj;
    obj.f74 = 0;
    bool r = obj(&base);

    bool ok = r && (obj.f74 == 2) && (g_payload == (void*)&obj);
    if (ok) printf("00c26580_TEST PASS\n");
    else printf("FAIL r=%d f74=%u payload=%d\n", r, obj.f74, g_payload==(void*)&obj);
    return 0;
}