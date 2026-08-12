#include <cstdio>
#include <cstring>

class CCharString;

static CCharString* g_notified = 0;
static int g_calls = 0;

struct Sub
{
    virtual void v0(){} virtual void v1(){} virtual void v2(){}
    virtual void v3(){} virtual void v4(){} virtual void v5(){}
    virtual void v6(){} virtual void v7(){} virtual void v8(){}
    virtual void v9(){} virtual void v10(){} virtual void v11(){}
    virtual void v12(){} virtual void v13(){}
    virtual void Notify(CCharString* status) { g_notified = status; g_calls++; }
};

struct CBankFile
{
    char pad0[0x50];
    Sub* sub;
    char pad1[0xf0 - 0x54];
    CCharString* status;

    void SetBankUpdateStatus(CCharString* s);
};

void CBankFile::SetBankUpdateStatus(CCharString* s)
{
    this->status = s;
    Sub* p = this->sub;
    if (p)
        p->Notify(s);
}

int main()
{
    CCharString* marker = (CCharString*)0xDEADBEEF;

    // Case 1: sub present -> stored AND notified
    CBankFile b1;
    memset(&b1, 0, sizeof(b1));
    Sub s;
    b1.sub = &s;
    g_notified = 0; g_calls = 0;
    b1.SetBankUpdateStatus(marker);
    bool ok1 = (b1.status == marker) && (g_notified == marker) && (g_calls == 1);

    // Case 2: sub null -> stored, no notify
    CBankFile b2;
    memset(&b2, 0, sizeof(b2));
    b2.sub = 0;
    g_notified = 0; g_calls = 0;
    b2.SetBankUpdateStatus(marker);
    bool ok2 = (b2.status == marker) && (g_calls == 0);

    if (ok1 && ok2) { printf("SETBANKUPDATE_OK\n"); return 0; }
    printf("FAIL ok1=%d ok2=%d\n", (int)ok1, (int)ok2);
    return 1;
}