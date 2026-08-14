#include <cstdio>

typedef int LH_RETURN;

static char g_query_ret = 0;
static char query_stub(int) { return g_query_ret; }

struct LHHttp2 { void* conn; };

static LH_RETURN IsDataAvailable_model(LHHttp2* self)
{
    void* c = *(void**)self;
    if (c != 0)
        return query_stub(0) ? 0 : 3;
    return 2;
}

int main()
{
    LHHttp2 a; a.conn = 0;
    LHHttp2 b; b.conn = (void*)0x1234;

    // null conn -> 2
    if (IsDataAvailable_model(&a) != 2) { printf("FAIL null\n"); return 1; }

    // non-null conn, query returns nonzero -> 0
    g_query_ret = 1;
    if (IsDataAvailable_model(&b) != 0) { printf("FAIL nonzero\n"); return 1; }

    // non-null conn, query returns zero -> 3
    g_query_ret = 0;
    if (IsDataAvailable_model(&b) != 3) { printf("FAIL zero\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}