#include <stdio.h>
#include <string.h>

struct CPersistContext {
    void Transfer_long_(char* name, long* value);
};

// captured args from the forwarded call
static char* g_name;
static long* g_value;
static long   g_def;
static int    g_called;

void __stdcall CPersistContext_TransferHelper(char* name, long* value, long* def)
{
    g_name = name;
    g_value = value;
    g_def = *def;      // observe the zero-inited default
    g_called = 1;
}

void CPersistContext::Transfer_long_(char* name, long* value)
{
    long def = 0;
    CPersistContext_TransferHelper(name, value, &def);
}

int main()
{
    CPersistContext ctx;
    char nm[] = "MyField";
    long v = 12345;

    ctx.Transfer_long_(nm, &v);

    int ok = g_called == 1
          && g_name == nm
          && g_value == &v
          && g_def == 0;

    if (ok)
        printf("TRANSFER_LONG_OK\n");
    else
        printf("TRANSFER_LONG_FAIL c=%d def=%ld\n", g_called, g_def);
    return ok ? 0 : 1;
}