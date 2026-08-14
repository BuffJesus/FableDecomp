#include <stdio.h>

// Behaviour model of operator_delete forwarder: it calls inner_free(block, 0, 0).

static void *g_seen_block;
static void *g_seen_heap;
static int   g_seen_flags;
static int   g_call_count;

static void __stdcall inner_free(void *block, void *heap, int flags)
{
    g_seen_block = block;
    g_seen_heap  = heap;
    g_seen_flags = flags;
    g_call_count++;
}

static void __stdcall operator_delete(void *block)
{
    inner_free(block, 0, 0);
}

int main(void)
{
    int dummy = 0;
    void *p = &dummy;

    operator_delete(p);

    if (g_call_count == 1 &&
        g_seen_block == p &&
        g_seen_heap == 0 &&
        g_seen_flags == 0)
    {
        printf("DELETE_FORWARD_OK\n");
    }
    else
    {
        printf("FAIL\n");
    }
    return 0;
}