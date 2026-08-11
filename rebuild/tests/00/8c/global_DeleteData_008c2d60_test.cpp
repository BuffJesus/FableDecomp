#include <stdio.h>

struct IData {
    virtual void Release(int flag) = 0;
};

void __fastcall DeleteData(IData *self);

static int g_flag = -1;
static int g_calls = 0;

struct Impl : public IData {
    virtual void Release(int flag) { g_flag = flag; g_calls++; }
};

int main()
{
    Impl obj;
    DeleteData(&obj);
    DeleteData(0);
    if (g_calls == 1 && g_flag == 1)
        printf("DELETEDATA_OK\n");
    else
        printf("DELETEDATA_FAIL calls=%d flag=%d\n", g_calls, g_flag);
    return (g_calls == 1 && g_flag == 1) ? 0 : 1;
}