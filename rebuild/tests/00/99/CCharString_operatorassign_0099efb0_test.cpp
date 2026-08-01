#include <cstdio>

typedef unsigned char uint8;
typedef int int32;

#pragma pack(push, 1)
struct CCharStringData
{
    uint8 pad[0xd];
    int32 refcount;
};
#pragma pack(pop)

static int g_unassignCalls = 0;
static void* g_lastUnassignThis = 0;

struct CCharString
{
    CCharStringData* data;

    void UnassignString();
};

void CCharString::UnassignString()
{
    ++g_unassignCalls;
    g_lastUnassignThis = this;
}

CCharString* __fastcall CCharString_operator_assign(CCharString* self, void* /*edx*/, CCharString* rhs)
{
    if (rhs != self)
    {
        if (self->data != 0)
            self->UnassignString();

        CCharStringData* p = rhs->data;
        if (p != 0)
        {
            self->data = p;
            p->refcount++;
        }
    }
    return self;
}

int main()
{
    bool ok = true;

    // Case 1: normal assign, non-null rhs data -> refcount increments, old released.
    CCharStringData sharedData;
    sharedData.refcount = 5;

    CCharStringData oldData;
    oldData.refcount = 1;

    CCharString a;
    a.data = &oldData;

    CCharString b;
    b.data = &sharedData;

    g_unassignCalls = 0; g_lastUnassignThis = 0;
    CCharString* ret = CCharString_operator_assign(&a, 0, &b);

    if (ret != &a) ok = false;
    if (a.data != &sharedData) ok = false;
    if (sharedData.refcount != 6) ok = false;
    if (g_unassignCalls != 1) ok = false;
    if (g_lastUnassignThis != &a) ok = false;

    // Case 2: self-assignment guard -> nothing changes, no release call.
    CCharStringData selfData;
    selfData.refcount = 3;
    CCharString c;
    c.data = &selfData;
    g_unassignCalls = 0;
    CCharString* ret2 = CCharString_operator_assign(&c, 0, &c);
    if (ret2 != &c) ok = false;
    if (c.data != &selfData) ok = false;
    if (selfData.refcount != 3) ok = false;
    if (g_unassignCalls != 0) ok = false;

    // Case 3: rhs has null data, self had null data too -> stays null, no release, no crash.
    CCharString d;
    d.data = 0;
    CCharString e;
    e.data = 0;
    g_unassignCalls = 0;
    CCharString* ret3 = CCharString_operator_assign(&d, 0, &e);
    if (ret3 != &d) ok = false;
    if (d.data != 0) ok = false;
    if (g_unassignCalls != 0) ok = false;

    // Case 4: self had non-null data, rhs has null data -> old released via UnassignString,
    // but retail leaves this->data as the (now-stale) old pointer: the null-rhs branch
    // skips both the store and the refcount increment (matches retail machine code).
    CCharStringData oldData2;
    oldData2.refcount = 2;
    CCharString f;
    f.data = &oldData2;
    CCharString g;
    g.data = 0;
    g_unassignCalls = 0;
    CCharString* ret4 = CCharString_operator_assign(&f, 0, &g);
    if (ret4 != &f) ok = false;
    if (f.data != &oldData2) ok = false;
    if (g_unassignCalls != 1) ok = false;

    if (ok)
        std::printf("TEST_OK_ASSIGN_0099EFB0\n");
    else
        std::printf("TEST_FAIL\n");
    return 0;
}