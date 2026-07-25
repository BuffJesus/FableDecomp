#include "rebuild_abi.h"
#include <stdio.h>

struct CActiveFile
{
    void* field_0;
    int   field_4;
};

extern void FABLE_FASTCALL CActiveFile_call1(void* thisp, int edx, char* p);
extern void FABLE_CDECL CActiveFile_call2(void* p);
void FABLE_FASTCALL CActiveFile_OnReadFinished(CActiveFile* thisptr);

static int g_call1_seen_edx = 0;
static int g_call1_hits = 0;
static int g_call2_hits = 0;

void FABLE_FASTCALL CActiveFile_call1(void* thisp, int edx, char* p)
{
    (void)thisp; (void)p;
    g_call1_hits++;
    g_call1_seen_edx = edx;
}

void FABLE_CDECL CActiveFile_call2(void* p)
{
    (void)p;
    g_call2_hits++;
}

int main(void)
{
    int dummy = 0;

    // field_0 non-null: both calls fire.
    CActiveFile a;
    a.field_0 = &dummy;
    a.field_4 = 0x1234;
    CActiveFile_OnReadFinished(&a);

    // field_0 null: only call1 fires.
    CActiveFile b;
    b.field_0 = 0;
    b.field_4 = 0x5678;
    CActiveFile_OnReadFinished(&b);

    if (g_call1_hits == 2 && g_call2_hits == 1 && g_call1_seen_edx == 0x5678)
        printf("PASS_0046ee0a\n");
    else
        printf("FAIL_0046ee0a c1=%d c2=%d edx=%x\n",
               g_call1_hits, g_call2_hits, g_call1_seen_edx);
    return 0;
}