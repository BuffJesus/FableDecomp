#include <cstdio>

struct CCharString
{
    void* data;
};

extern "C" char g_empty;

char* __fastcall CCharString_operator_char_const_ptr(CCharString* self, void* /*edx_unused*/)
{
    void* data = self->data;
    if (!data)
    {
        return &g_empty;
    }
    return *(char**)data;
}

char g_empty = 0;

int main()
{
    char* payload = "hello";
    CCharString withData;
    withData.data = &payload;

    char* r1 = CCharString_operator_char_const_ptr(&withData, 0);
    if (r1 != payload) { printf("FAIL_NONNULL\n"); return 1; }

    CCharString empty;
    empty.data = 0;

    char* r2 = CCharString_operator_char_const_ptr(&empty, 0);
    if (r2 != &g_empty) { printf("FAIL_NULL\n"); return 1; }

    printf("CCHARSTRING_OP_2_OK\n");
    return 0;
}