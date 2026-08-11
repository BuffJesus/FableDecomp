#include <cstdio>

struct CDiskFileWin32_0099abd0
{
    char pad0[8];
    long position; // +0x08
    char pad2[4];  // +0x0C
    void* handle;  // +0x10
};

extern "C" long __fastcall GetPosition_0099abd0(CDiskFileWin32_0099abd0* self);

static int g_calls;
static void* g_handle;
static long g_move;
static long* g_high;
static unsigned long g_method;

extern "C" unsigned long __stdcall TestSetFilePointer_0099abd0(
    void* hFile,
    long lDistanceToMove,
    long* lpDistanceToMoveHigh,
    unsigned long dwMoveMethod)
{
    ++g_calls;
    g_handle = hFile;
    g_move = lDistanceToMove;
    g_high = lpDistanceToMoveHigh;
    g_method = dwMoveMethod;
    return 0xDEADBEEF; // discarded by GetPosition
}

extern "C" unsigned long (__stdcall *CDiskFileWin32_SetFilePointerIAT_0099abd0)(
    void*, long, long*, unsigned long) = &TestSetFilePointer_0099abd0;

int main()
{
    CDiskFileWin32_0099abd0 obj;
    obj.position = 0x12345678;
    obj.handle = (void*)0xABCD1234;

    typedef long (__fastcall *GetPosFn)(CDiskFileWin32_0099abd0*);
    GetPosFn volatile fn = &GetPosition_0099abd0;

    long r = fn(&obj);

    int fail = 0;
    if (r != 0x12345678) ++fail;
    if (g_calls != 1) ++fail;
    if (g_handle != (void*)0xABCD1234) ++fail;
    if (g_move != 0) ++fail;
    if (g_high != 0) ++fail;
    if (g_method != 1) ++fail;

    if (fail == 0)
        std::printf("0099abd0_TEST PASS\n");
    else
        std::printf("0099abd0_TEST FAIL fail=%d r=%ld calls=%d method=%lu\n",
                    fail, r, g_calls, g_method);
    return 0;
}