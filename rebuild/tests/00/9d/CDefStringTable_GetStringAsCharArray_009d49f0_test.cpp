#include <cstdio>
#include <cstring>

char DAT_0122d70e = 0;

class CMemoryDataOutputStream
{
public:
    void *m_base;
    void *PeekData() { return m_base; }
};

class CDefStringTable
{
public:
    char pad0[0xc];
    CMemoryDataOutputStream m_stream;

    char *GetStringAsCharArray(long index);
};

char *CDefStringTable::GetStringAsCharArray(long index)
{
    if (index != -1)
    {
        void *base = m_stream.PeekData();
        return (char *)base + index + 4;
    }

    return &DAT_0122d70e;
}

int main()
{
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    // simulate layout: [4-byte header][actual C-string data...]
    strcpy(buffer + 4, "hello");

    CDefStringTable table;
    table.m_stream.m_base = buffer;

    char *emptyResult = table.GetStringAsCharArray(-1);
    char *helloResult = table.GetStringAsCharArray(0);

    bool ok = (emptyResult == &DAT_0122d70e) && (*emptyResult == 0) &&
              (helloResult == buffer + 4) && (strcmp(helloResult, "hello") == 0);

    if (ok)
        printf("GETSTRINGASCHARARRAY_OK\n");
    else
        printf("GETSTRINGASCHARARRAY_FAIL\n");

    return 0;
}