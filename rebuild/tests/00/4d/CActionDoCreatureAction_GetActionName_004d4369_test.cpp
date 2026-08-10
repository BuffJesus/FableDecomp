#include <cstdio>

struct CCharString { char* p; };

static int g_ctor_calls = 0;
static const char* g_ctor_s = 0;
static int g_ctor_n = 0;
static CCharString* g_ctor_self = 0;

/* model of the ctor at 0x99ebf0 */
static CCharString* ctor_model(CCharString* self, const char* s, int n)
{
    g_ctor_calls++;
    g_ctor_self = self;
    g_ctor_s = s;
    g_ctor_n = n;
    self->p = (char*)s;
    return self;
}

/* model of GetActionName_5: returns retbuf after constructing */
static CCharString* model_GetActionName_5(CCharString* retbuf)
{
    ctor_model(retbuf, (const char*)0x123bb40, -1);
    return retbuf;
}

int main()
{
    CCharString buf;
    buf.p = 0;
    CCharString* r = model_GetActionName_5(&buf);

    int ok = 1;
    if (r != &buf) ok = 0;                       /* returns the buffer ptr */
    if (g_ctor_calls != 1) ok = 0;               /* ctor called exactly once */
    if (g_ctor_self != &buf) ok = 0;             /* this == retbuf */
    if (g_ctor_s != (const char*)0x123bb40) ok = 0; /* literal ptr */
    if (g_ctor_n != -1) ok = 0;                  /* length arg -1 */
    if (buf.p != (char*)0x123bb40) ok = 0;

    if (ok) { printf("OK_004d4369\n"); return 0; }
    printf("FAIL_004d4369\n");
    return 1;
}