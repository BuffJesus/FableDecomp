#include <stdio.h>
#include "candidates/build_number_compiled.h"

enum BuildEvent
{
    EVENT_ASSIGN_WIDE,
    EVENT_PATH_EXISTS,
    EVENT_DESTROY,
    EVENT_DISK_CONSTRUCT,
    EVENT_FILE_CONSTRUCT,
    EVENT_ASSIGN_STRING,
    EVENT_DISK_CLOSE,
    EVENT_DISK_BASE_CLEANUP
};

struct BuildCall
{
    BuildEvent Event;
    const void* Self;
    const void* Argument;
    int ValueA;
    int ValueB;
};

CCharString FableFullBuildVersion;
void* FableDiskFileWin32Vftable[1];
static BuildCall gCalls[20];
static int gCallCount;
static bool gPathExists;

static void Record(BuildEvent event, const void* self, const void* argument, int valueA, int valueB)
{
    BuildCall& call = gCalls[gCallCount++];
    call.Event = event;
    call.Self = self;
    call.Argument = argument;
    call.ValueA = valueA;
    call.ValueB = valueB;
}

void CCharString::AssignFromWide(const wchar_t* text)
{
    Record(EVENT_ASSIGN_WIDE, this, text, 0, 0);
}

bool CWideString::PathExists() const
{
    Record(EVENT_PATH_EXISTS, this, 0, 0, 0);
    return gPathExists;
}

void CCharString::Destroy()
{
    Record(EVENT_DESTROY, this, 0, 0, 0);
}

void CDiskFileWin32::Construct(const CWideString& path, int accessMode, int shareMode)
{
    Record(EVENT_DISK_CONSTRUCT, this, &path, accessMode, shareMode);
}

void CCharString::ConstructFromFile(CAFile* file, int length)
{
    Record(EVENT_FILE_CONSTRUCT, this, file, length, 0);
}

CCharString* CCharString::Assign(const CCharString& other)
{
    Record(EVENT_ASSIGN_STRING, this, &other, 0, 0);
    return this;
}

void CDiskFileWin32::Close()
{
    Record(EVENT_DISK_CLOSE, this, Vftable, 0, 0);
}

void CDiskFileWin32::BaseCleanup()
{
    Record(EVENT_DISK_BASE_CLEANUP, this, 0, 0, 0);
}

int main()
{
    gPathExists = false;
    GFGetBuildNumber2();
    if (gCallCount != 3) return 1;
    if (gCalls[0].Event != EVENT_ASSIGN_WIDE) return 2;
    if (gCalls[1].Event != EVENT_PATH_EXISTS) return 3;
    if (gCalls[2].Event != EVENT_DESTROY) return 4;

    gCallCount = 0;
    gPathExists = true;
    GFGetBuildNumber2();
    if (gCallCount != 12) return 5;
    if (gCalls[0].Event != EVENT_ASSIGN_WIDE ||
        gCalls[1].Event != EVENT_PATH_EXISTS ||
        gCalls[2].Event != EVENT_DESTROY) return 6;
    if (gCalls[3].Event != EVENT_ASSIGN_WIDE) return 7;
    if (gCalls[4].Event != EVENT_DISK_CONSTRUCT ||
        gCalls[4].ValueA != 1 || gCalls[4].ValueB != 2) return 8;
    if (gCalls[5].Event != EVENT_DESTROY) return 9;
    if (gCalls[6].Event != EVENT_FILE_CONSTRUCT || gCalls[6].ValueA != -1) return 10;
    if (gCalls[7].Event != EVENT_ASSIGN_STRING ||
        gCalls[7].Self != &FableFullBuildVersion ||
        gCalls[7].Argument != gCalls[6].Self) return 11;
    if (gCalls[8].Event != EVENT_DESTROY || gCalls[8].Self != gCalls[6].Self) return 12;
    if (gCalls[9].Event != EVENT_DISK_CLOSE ||
        gCalls[9].Argument != FableDiskFileWin32Vftable) return 13;
    if (gCalls[10].Event != EVENT_DESTROY) return 14;
    if (gCalls[11].Event != EVENT_DISK_BASE_CLEANUP ||
        gCalls[11].Self != gCalls[9].Self) return 15;

    printf("GF_GET_BUILD_NUMBER_2_TEST PASS\n");
    return 0;
}
