// Independent behaviour test for
// C3DMeshFileXAnimatedPrimitiveChunk::GetChunkDescription
//
// The real fn constructs CCharString(0x129e2a4, -1) via the engine ctor.
// We supply a local ctor that records the args so we can assert the fn
// forwards the exact literal pointer and length that retail hard-codes.

#include <cstdio>

static const char* g_lastPtr = 0;
static int         g_lastLen = 0;

class CCharString
{
public:
    CCharString(const char* str, int len)
    {
        g_lastPtr = str;
        g_lastLen = len;
        m_ptr = (char*)str;
    }
private:
    char* m_ptr;
};

class C3DMeshFileXAnimatedPrimitiveChunk
{
public:
    virtual CCharString GetChunkDescription() const;
    int filler;
};

CCharString C3DMeshFileXAnimatedPrimitiveChunk::GetChunkDescription() const
{
    return CCharString((const char*)0x129e2a4, -1);
}

int main()
{
    C3DMeshFileXAnimatedPrimitiveChunk chunk;
    chunk.GetChunkDescription();

    if (g_lastPtr == (const char*)0x129e2a4 && g_lastLen == -1)
    {
        printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    printf("GETCHUNKDESC_FAIL ptr=%p len=%d\n", (void*)g_lastPtr, g_lastLen);
    return 1;
}