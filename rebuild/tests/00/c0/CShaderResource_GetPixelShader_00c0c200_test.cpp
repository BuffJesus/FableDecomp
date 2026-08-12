#include <stdio.h>

struct CCharString;
struct CPixelShader;

int __stdcall helper_c0bcc0(CCharString* a, CCharString* b);

struct CShaderResource {
    int marker;
    int consume_c0c060(int v);
    CPixelShader* GetPixelShader(CCharString* name);
};

static int g_helper_calls = 0;
static int g_consume_arg = -1;
static CCharString* g_a = 0;
static CCharString* g_b = 0;

int __stdcall helper_c0bcc0(CCharString* a, CCharString* b)
{
    g_helper_calls++;
    g_a = a; g_b = b;
    return 777;
}

int CShaderResource::consume_c0c060(int v)
{
    g_consume_arg = v;
    return 0;
}

int main()
{
    CShaderResource r;
    r.marker = 0x1234;
    CCharString* nm = reinterpret_cast<CCharString*>(0xDEAD);

    CPixelShader* out = r.GetPixelShader(nm);

    int ok = 1;
    if (reinterpret_cast<void*>(out) != reinterpret_cast<void*>(&r)) ok = 0; // returns this
    if (g_helper_calls != 1) ok = 0;
    if (g_a != nm || g_b != nm) ok = 0;        // both args are the same name ptr
    if (g_consume_arg != 777) ok = 0;          // helper result fed to consume

    printf(ok ? "PASS_GETPIXELSHADER\n" : "FAIL\n");
    return ok ? 0 : 1;
}