#include <cstdio>

static int g_baseCalled = 0;
static void* g_subArg = 0;
static int g_subFlag = 0;
static void* g_subThis = 0;

void __fastcall CThingNoiseDef_baseTransfer(void* self)
{
    g_baseCalled = 1;
    (void)self;
}

struct CThingNoiseSub {
    void Transfer(void* a, int flag);
};
void CThingNoiseSub::Transfer(void* a, int flag)
{
    g_subThis = this;
    g_subArg = a;
    g_subFlag = flag;
}

void* __fastcall Transfer(void* self);

int main()
{
    char buf[64] = {0};
    void* self = buf;
    void* r = Transfer(self);
    if (r == self && g_baseCalled == 1 &&
        g_subThis == (void*)((char*)self + 0x14) &&
        g_subArg == self && g_subFlag == 1) {
        printf("00c3a720_TEST PASS\n");
    } else {
        printf("FAIL r=%p self=%p base=%d subThis=%p subArg=%p flag=%d\n",
               r, self, g_baseCalled, g_subThis, g_subArg, g_subFlag);
    }
    return 0;
}