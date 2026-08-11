#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Standalone behaviour model of GetChunkInfo:
// it returns CCharString("SmoothingGroups", -1). We model CCharString as an
// owning string; len == -1 means "compute via strlen".

class CCharString
{
public:
    CCharString(const char* str, long len)
    {
        long n = (len < 0) ? (long)strlen(str) : len;
        m_pData = (char*)malloc(n + 1);
        memcpy(m_pData, str, n);
        m_pData[n] = 0;
    }
    CCharString(const CCharString& o)
    {
        long n = (long)strlen(o.m_pData);
        m_pData = (char*)malloc(n + 1);
        memcpy(m_pData, o.m_pData, n + 1);
    }
    ~CCharString() { free(m_pData); }
    const char* c_str() const { return m_pData; }
private:
    char* m_pData;
};

class C3DMeshFileSmoothingGroupsChunk
{
public:
    virtual CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileSmoothingGroupsChunk::GetChunkInfo() const
{
    return CCharString("SmoothingGroups", -1);
}

int main()
{
    C3DMeshFileSmoothingGroupsChunk chunk;
    CCharString info = chunk.GetChunkInfo();
    if (strcmp(info.c_str(), "SmoothingGroups") == 0)
        printf("CHUNKINFO_OK\n");
    else
        printf("CHUNKINFO_FAIL:%s\n", info.c_str());
    return (strcmp(info.c_str(), "SmoothingGroups") == 0) ? 0 : 1;
}