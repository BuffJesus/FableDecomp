#include <stdio.h>

static int g_freeCalls = 0;
static void* g_lastFreed = 0;

extern "C" void __cdecl FreeStreamBuffer(void* p)
{
    g_freeCalls++;
    g_lastFreed = p;
}

class IOwnedFile
{
public:
    virtual void   Slot00() = 0;
    virtual void   Slot04() = 0;
    virtual void   Slot08() = 0;
    virtual void   Slot0C() = 0;
    virtual void   Slot10() = 0;
    virtual void   ReleaseHandle(int id) = 0;
    virtual void   Slot18() = 0;
    virtual void   Slot1C() = 0;
    virtual void   Slot20() = 0;
    virtual void   Slot24() = 0;
    virtual bool   ShouldRelease() = 0;
};

class CFileDataInputStream
{
public:
    virtual void Unused00() = 0;
    virtual void Unused04() = 0;
    virtual int  GetHandleId() = 0;

    void* field04;
    void* field08;
    void* field0C;
    void* field10;
    void* field14;
    IOwnedFile* ownedFile;
    void* buffer;
    int   bufferCount;

    void Close();
};

void CFileDataInputStream::Close()
{
    if (buffer != 0)
    {
        FreeStreamBuffer(buffer);
        buffer = 0;
        bufferCount = 0;
    }

    if (ownedFile != 0)
    {
        if (ownedFile->ShouldRelease())
        {
            ownedFile->ReleaseHandle(GetHandleId());
        }
    }

    ownedFile = 0;
    field04 = 0;
    field08 = 0;
    field10 = 0;
    field14 = 0;
    field0C = 0;
}

static int g_getIdCalls = 0;
static int g_shouldReleaseCalls = 0;
static int g_releaseCalls = 0;
static int g_releaseId = 0;

class FakeOwnedFile : public IOwnedFile
{
public:
    virtual void Slot00() {}
    virtual void Slot04() {}
    virtual void Slot08() {}
    virtual void Slot0C() {}
    virtual void Slot10() {}
    virtual void Slot18() {}
    virtual void Slot1C() {}
    virtual void Slot20() {}
    virtual void Slot24() {}
    virtual bool ShouldRelease()
    {
        g_shouldReleaseCalls++;
        return true;
    }
    virtual void ReleaseHandle(int id)
    {
        g_releaseCalls++;
        g_releaseId = id;
    }
};

class FakeStream : public CFileDataInputStream
{
public:
    virtual void Unused00() {}
    virtual void Unused04() {}
    virtual int GetHandleId()
    {
        g_getIdCalls++;
        return 0x1234;
    }
};

int main()
{
    int dummyBuffer = 0;

    FakeOwnedFile ownedFileObj;
    FakeStream stream;

    stream.field04 = (void*)1;
    stream.field08 = (void*)2;
    stream.field0C = (void*)3;
    stream.field10 = (void*)4;
    stream.field14 = (void*)5;
    stream.ownedFile = &ownedFileObj;
    stream.buffer = &dummyBuffer;
    stream.bufferCount = 42;

    stream.Close();

    bool ok = true;
    if (g_freeCalls != 1) ok = false;
    if (g_lastFreed != &dummyBuffer) ok = false;
    if (stream.buffer != 0) ok = false;
    if (stream.bufferCount != 0) ok = false;
    if (g_shouldReleaseCalls != 1) ok = false;
    if (g_getIdCalls != 1) ok = false;
    if (g_releaseCalls != 1) ok = false;
    if (g_releaseId != 0x1234) ok = false;
    if (stream.ownedFile != 0) ok = false;
    if (stream.field04 != 0) ok = false;
    if (stream.field08 != 0) ok = false;
    if (stream.field0C != 0) ok = false;
    if (stream.field10 != 0) ok = false;
    if (stream.field14 != 0) ok = false;

    if (ok)
        printf("CLOSE_TEST_OK\n");
    else
        printf("CLOSE_TEST_FAIL\n");

    return 0;
}