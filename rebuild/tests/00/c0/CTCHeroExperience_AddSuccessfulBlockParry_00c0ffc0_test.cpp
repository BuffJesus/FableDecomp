#include <cstdio>

extern int (*g_TimerFnPtr)();

static int g_helperCalls = 0;

struct HelperArg
{
    int tag;
    void MethodC1C830();
};

void HelperArg::MethodC1C830() { g_helperCalls++; this->tag += 100; }

struct CTCHeroExperience
{
    HelperArg* field0;
    int field4;
    CTCHeroExperience* AddSuccessfulBlockParry(HelperArg* arg);
};

CTCHeroExperience* CTCHeroExperience::AddSuccessfulBlockParry(HelperArg* arg)
{
    this->field0 = arg;
    arg->MethodC1C830();
    this->field4 = g_TimerFnPtr();
    return this;
}

static int FakeTimer() { return 0x1234ABCD; }
int (*g_TimerFnPtr)() = FakeTimer;

int main()
{
    CTCHeroExperience h;
    h.field0 = 0;
    h.field4 = -1;

    HelperArg a;
    a.tag = 5;

    CTCHeroExperience* r = h.AddSuccessfulBlockParry(&a);

    bool ok = true;
    if (r != &h) ok = false;
    if (h.field0 != &a) ok = false;
    if (h.field4 != 0x1234ABCD) ok = false;
    if (g_helperCalls != 1) ok = false;
    if (a.tag != 105) ok = false;

    if (ok) {
        printf("PARRY_OK f4=%08X tag=%d\n", h.field4, a.tag);
        return 0;
    }
    printf("PARRY_FAIL\n");
    return 1;
}