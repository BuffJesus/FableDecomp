#include <cstdio>
#include <cstring>
#include <new>

struct Inner {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual bool v9(void* payload) = 0;   // slot 9
};

struct CIsThingAlive {
    char pad[0x74];
    unsigned short f74;
    bool operator()(void* arg);
};

bool CIsThingAlive::operator()(void* arg)
{
    this->f74 = 1;
    Inner* sub = (Inner*)((char*)arg + 0x2c);
    return sub->v9(this);
}

static void* g_seen_payload = 0;
static Inner* g_seen_self = 0;

// Concrete Inner whose v9 records what it was called with.
struct RealInner : Inner {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual void v6() {}
    virtual void v7() {}
    virtual void v8() {}
    virtual bool v9(void* payload) {
        g_seen_self = this;
        g_seen_payload = payload;
        return true;
    }
};

// Outer object: an Inner subobject embedded at +0x2c
struct Outer {
    char head[0x2c];
    RealInner inner;
};

int main()
{
    Outer o;
    memset(o.head, 0, sizeof(o.head));
    // placement-construct inner so its vptr is valid
    new (&o.inner) RealInner();

    CIsThingAlive obj;
    memset(&obj, 0, sizeof(obj));
    obj.f74 = 0;

    bool r = obj(&o);

    Inner* expected_self = (Inner*)((char*)&o + 0x2c);

    bool ok = true;
    if (obj.f74 != 1) ok = false;                      // word store this->f74 = 1
    if (!r) ok = false;                                // returned v9's result
    if ((void*)g_seen_self != (void*)expected_self) ok = false; // self = arg+0x2c
    if (g_seen_payload != (void*)&obj) ok = false;     // payload = this

    if (ok) printf("00c26560_TEST PASS\n");
    else printf("FAIL f74=%d r=%d self=%p exp=%p pay=%p exp=%p\n",
               (int)obj.f74, (int)r, (void*)g_seen_self, (void*)expected_self,
               g_seen_payload, (void*)&obj);
    return 0;
}