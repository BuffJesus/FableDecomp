#include "rebuild_abi.h"
#include <stdio.h>

struct CActiveFileReader;

struct CActiveFile
{
    CActiveFileReader* m_reader; // +0x00
    fable_i32          m_status; // +0x04
};

extern void FABLE_FASTCALL CActiveFile_OnReadFinished(CActiveFile* thisptr);

// ---- captured effects ----
static int   g_notifyCalls  = 0;
static fable_i32 g_seenStatus = 0;
static void* g_notifyThis   = 0;
static int   g_releaseCalls = 0;
static void* g_releasedPtr  = 0;

void FABLE_FASTCALL CActiveFileReader_Notify(CActiveFileReader* thisptr,
                                             fable_i32 status,
                                             fable_u8* outByte)
{
    g_notifyCalls++;
    g_seenStatus = status;
    g_notifyThis = (void*)thisptr;
    if (outByte)
        *outByte = 0xAB;
}

void FABLE_CDECL CActiveFile_ReleaseReader(CActiveFileReader* reader)
{
    g_releaseCalls++;
    g_releasedPtr = (void*)reader;
}

int main(void)
{
    // Case A: reader non-null -> notify + release both fire.
    CActiveFile a;
    a.m_reader = (CActiveFileReader*)0x12345678;
    a.m_status = 7;
    CActiveFile_OnReadFinished(&a);

    // Case B: reader null -> notify fires, release does NOT.
    CActiveFile b;
    b.m_reader = 0;
    b.m_status = 99;
    CActiveFile_OnReadFinished(&b);

    if (g_notifyCalls == 2 &&
        g_seenStatus == 99 &&
        g_notifyThis == (void*)0 &&
        g_releaseCalls == 1 &&
        g_releasedPtr == (void*)0x12345678)
    {
        printf("PASS_0046f68c\n");
    }
    else
    {
        printf("FAIL_0046f68c calls=%d status=%d rel=%d\n",
               g_notifyCalls, (int)g_seenStatus, g_releaseCalls);
    }
    return 0;
}