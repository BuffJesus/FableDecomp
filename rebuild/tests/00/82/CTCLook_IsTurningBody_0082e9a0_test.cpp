#include <cstdio>

struct Vec8 { float a; float b; };

struct SubObj {
    bool Query(Vec8* out, Vec8* in);
};

struct CTCLook {
    char pad[0x2c];
    SubObj sub;
    bool IsTurningBody(Vec8 arg);
};

static Vec8 g_seen_in;
static SubObj* g_seen_this;

bool SubObj::Query(Vec8* out, Vec8* in)
{
    g_seen_this = this;
    g_seen_in = *in;
    out->a = 1.0f;
    out->b = 2.0f;
    return (in->a > 0.5f);
}

int main()
{
    CTCLook obj;
    Vec8 arg; arg.a = 3.5f; arg.b = 9.0f;

    bool r = obj.IsTurningBody(arg);

    bool ok = true;
    if (r != true) ok = false;
    if (g_seen_in.a != 3.5f || g_seen_in.b != 9.0f) ok = false;
    if ((char*)g_seen_this != (char*)&obj + 0x2c) ok = false;

    Vec8 arg2; arg2.a = 0.1f; arg2.b = 0.0f;
    bool r2 = obj.IsTurningBody(arg2);
    if (r2 != false) ok = false;

    printf(ok ? "TURNBODY_OK\n" : "TURNBODY_FAIL\n");
    return ok ? 0 : 1;
}