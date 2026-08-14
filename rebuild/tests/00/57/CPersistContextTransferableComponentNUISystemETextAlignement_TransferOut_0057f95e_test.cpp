#include <cstdio>

typedef int ETextAlignement;

struct Self {
    void* vtbl;
    ETextAlignement* pValue;
};

static ETextAlignement g_captured;
static void* g_ctx;
static void* g_name;

static void Transfer_stub(void* ctx, void* name, ETextAlignement* pval)
{
    g_ctx = ctx;
    g_name = name;
    g_captured = *pval;
}

static void TransferOut_model(Self* self, void* ctx, void* name)
{
    ETextAlignement v = *(self->pValue);
    Transfer_stub(ctx, name, &v);
}

int main()
{
    ETextAlignement value = 7;
    Self s;
    s.vtbl = 0;
    s.pValue = &value;

    int ctxObj = 111;
    int nameObj = 222;

    TransferOut_model(&s, &ctxObj, &nameObj);

    bool ok = (g_captured == 7) &&
              (g_ctx == (void*)&ctxObj) &&
              (g_name == (void*)&nameObj);

    if (ok) printf("TRANSFEROUT_OK\n");
    else    printf("FAIL cap=%d\n", g_captured);
    return 0;
}