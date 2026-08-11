#include <stdio.h>

extern "C" void __stdcall PersistHelper_580371(char* name, long* value, long* out);

struct CPersistContext
{
    void Transfer_long_(char* name, long* value)
    {
        long temp = 0;
        PersistHelper_580371(name, value, &temp);
    }
};

// Local stand-in for the helper so the test links standalone.
static char* g_name;
static long* g_value;
static long* g_out;
static int g_calls;

extern "C" void __stdcall PersistHelper_580371(char* name, long* value, long* out)
{
    g_name = name;
    g_value = value;
    g_out = out;
    g_calls++;
    *out = *value + 1;
    *value = *out;
}

int main()
{
    char nm[] = "Health";
    long v = 41;
    CPersistContext ctx;

    ctx.Transfer_long_(nm, &v);

    bool ok = (g_calls == 1)
           && (g_name == nm)
           && (g_value == &v)
           && (g_out != 0)
           && (v == 42);

    if (ok)
        printf("TRANSFER_LONG_OK v=%ld\n", v);
    else
        printf("TRANSFER_LONG_FAIL v=%ld calls=%d\n", v, g_calls);
    return ok ? 0 : 1;
}