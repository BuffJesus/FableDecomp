#include <stdio.h>

struct IValidatable
{
    void* vtbl[4];
};

typedef void (__fastcall *ValidateFn)(void* self);

struct CMovableResourceMemoryPool
{
    void* vtbl0;

    void ValidateResource(void* p);
};

void CMovableResourceMemoryPool::ValidateResource(void* p)
{
    ValidateFn fn = (ValidateFn)(*(void***)p)[3];
    fn(p);
}

static int g_called = 0;
static void* g_self_seen = 0;

static void __fastcall FakeValidate(void* self)
{
    g_called = 1;
    g_self_seen = self;
}

int main()
{
    void* fakeVtbl[4];
    fakeVtbl[3] = (void*)&FakeValidate;

    struct Obj { void* vtbl; } obj;
    obj.vtbl = fakeVtbl;

    CMovableResourceMemoryPool pool;
    pool.vtbl0 = 0;

    pool.ValidateResource(&obj);

    if (g_called == 1 && g_self_seen == (void*)&obj)
    {
        printf("PASS_VALIDATE_RESOURCE_FORWARD\n");
    }
    else
    {
        printf("FAIL\n");
    }

    return 0;
}