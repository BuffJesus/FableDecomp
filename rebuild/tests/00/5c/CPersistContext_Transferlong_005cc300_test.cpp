#include <stdio.h>

struct CPersistContext
{
    void TransferImpl(char* name, long* value, long* defaultValue);
    void Transfer_long_(char* name, long* value);
};

// Capture what the wrapper forwards.
static CPersistContext* g_self;
static char* g_name;
static long* g_value;
static long  g_default = 0x7BADF00D;

void CPersistContext::TransferImpl(char* name, long* value, long* defaultValue)
{
    g_self = this;
    g_name = name;
    g_value = value;
    g_default = *defaultValue; // must be the zeroed local
}

void CPersistContext::Transfer_long_(char* name, long* value)
{
    long defaultValue = 0;
    TransferImpl(name, value, &defaultValue);
}

int main()
{
    CPersistContext ctx;
    char nm[] = "FieldName";
    long v = 12345;

    ctx.Transfer_long_(nm, &v);

    bool ok = (g_self == &ctx)
           && (g_name == nm)
           && (g_value == &v)
           && (g_default == 0);

    if (ok)
        printf("TRANSFER_LONG_OK\n");
    else
        printf("TRANSFER_LONG_FAIL self=%p name=%p value=%p def=%ld\n",
               g_self, g_name, g_value, g_default);
    return ok ? 0 : 1;
}