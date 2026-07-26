#include "rebuild_abi.h"
#include <stdio.h>

struct CActiveFileReader;

struct CActiveFile
{
    CActiveFileReader* m_reader; // +0x00
    fable_i32          m_status; // +0x04
};

extern void FABLE_FASTCALL CActiveFileReader_Notify(CActiveFileReader* thisptr,
                                                    fable_i32 status,
                                                    fable_u8* outByte);
extern void FABLE_CDECL CActiveFile_ReleaseReader(CActiveFileReader* reader);
void FABLE_FASTCALL CActiveFile_OnReadFinished_471df4(CActiveFile* thisptr);

static int g_notify = 0;
static int g_release = 0;

void FABLE_FASTCALL CActiveFileReader_Notify(CActiveFileReader* thisptr,
                                             fable_i32 status,
                                             fable_u8* outByte)
{
    (void)thisptr; (void)outByte;
    g_notify = status;
    if (outByte) *outByte = 0;
}

void FABLE_CDECL CActiveFile_ReleaseReader(CActiveFileReader* reader)
{
    (void)reader;
    g_release += 1;
}

int main(void)
{
    // Case 1: reader non-null -> notify + release called.
    CActiveFile a;
    a.m_reader = (CActiveFileReader*)0x1234;
    a.m_status = 7;
    CActiveFile_OnReadFinished_471df4(&a);

    // Case 2: reader null -> notify called (with status), release NOT called.
    CActiveFile b;
    b.m_reader = (CActiveFileReader*)0;
    b.m_status = 42;
    CActiveFile_OnReadFinished_471df4(&b);

    if (g_notify == 42 && g_release == 1)
        printf("OK_471df4\n");
    else
        printf("FAIL_471df4 notify=%d release=%d\n", g_notify, g_release);
    return 0;
}