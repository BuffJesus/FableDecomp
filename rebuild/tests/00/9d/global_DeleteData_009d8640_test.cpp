#include <stdio.h>

struct IDataHolder
{
    virtual void Method0() {}
    virtual void Method1() {}
    virtual void Release(int flag) {}
};

void __fastcall DeleteData(IDataHolder* self)
{
    if (self != 0)
        self->Release(1);
}

static int g_releaseCalledWith = -1;

struct FakeHolder : IDataHolder
{
    virtual void Method0() {}
    virtual void Method1() {}
    virtual void Release(int flag) { g_releaseCalledWith = flag; }
};

int main()
{
    FakeHolder holder;
    g_releaseCalledWith = -1;
    DeleteData(&holder);
    if (g_releaseCalledWith != 1)
    {
        printf("FAIL_NONNULL\n");
        return 1;
    }

    g_releaseCalledWith = -1;
    DeleteData(0);
    if (g_releaseCalledWith != -1)
    {
        printf("FAIL_NULLCALL\n");
        return 1;
    }

    printf("DELETEDATA_TEST_OK\n");
    return 0;
}