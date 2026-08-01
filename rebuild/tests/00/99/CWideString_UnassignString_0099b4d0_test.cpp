#include <cstdio>

struct CWideStringData
{
    void* buffer;
    int unused1;
    int unused2;
    int refCount;
};

struct CWideString
{
    CWideStringData* data;
};

static int g_freeBufferCalls = 0;
static int g_freeDataCalls = 0;
static void* g_lastBuffer = 0;
static CWideStringData* g_lastData = 0;

void __cdecl FreeStringBuffer(void* buffer)
{
    g_freeBufferCalls++;
    g_lastBuffer = buffer;
}

void __cdecl FreeStringData(CWideStringData* data)
{
    g_freeDataCalls++;
    g_lastData = data;
}

void __fastcall CWideString_UnassignString(CWideString* self)
{
    CWideStringData* data = self->data;
    if (data)
    {
        int rc = data->refCount - 1;
        data->refCount = rc;
        if (rc <= 0)
        {
            CWideStringData* p = self->data;
            if (p)
            {
                void* buf = p->buffer;
                if (buf)
                    FreeStringBuffer(buf);
                FreeStringData(p);
            }
        }
        self->data = 0;
    }
}

int main()
{
    // Case 1: refcount drops to 0, buffer present -> frees both, zeroes self->data
    char bufferMem[4] = {1,2,3,4};
    CWideStringData d1;
    d1.buffer = bufferMem;
    d1.unused1 = 0;
    d1.unused2 = 0;
    d1.refCount = 1;
    CWideString s1;
    s1.data = &d1;
    CWideString_UnassignString(&s1);
    bool case1ok = (g_freeBufferCalls == 1 && g_lastBuffer == bufferMem &&
                    g_freeDataCalls == 1 && g_lastData == &d1 &&
                    s1.data == 0);

    // Case 2: refcount still positive -> no frees, but self->data zeroed
    g_freeBufferCalls = 0; g_freeDataCalls = 0;
    CWideStringData d2;
    d2.buffer = bufferMem;
    d2.refCount = 5;
    CWideString s2;
    s2.data = &d2;
    CWideString_UnassignString(&s2);
    bool case2ok = (g_freeBufferCalls == 0 && g_freeDataCalls == 0 &&
                    d2.refCount == 4 && s2.data == 0);

    // Case 3: data is null -> no-op, no crash, data stays null
    CWideString s3;
    s3.data = 0;
    CWideString_UnassignString(&s3);
    bool case3ok = (s3.data == 0);

    // Case 4: refcount drops to 0, buffer null -> only FreeStringData called
    g_freeBufferCalls = 0; g_freeDataCalls = 0;
    CWideStringData d4;
    d4.buffer = 0;
    d4.refCount = 1;
    CWideString s4;
    s4.data = &d4;
    CWideString_UnassignString(&s4);
    bool case4ok = (g_freeBufferCalls == 0 && g_freeDataCalls == 1 && s4.data == 0);

    if (case1ok && case2ok && case3ok && case4ok)
        printf("UNASSIGN_STRING_OK\n");
    else
        printf("UNASSIGN_STRING_FAIL\n");
    return 0;
}