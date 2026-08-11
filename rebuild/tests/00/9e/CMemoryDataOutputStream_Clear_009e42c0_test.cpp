#include <cstdio>

static int g_cleared = 0;

struct SubBuffer {
    void Clear();
    int dummy;
};

struct CMemoryDataOutputStream {
    char pad[0xd0c];
    SubBuffer sub;
    unsigned int count;
    void Clear();
};

void SubBuffer::Clear() { g_cleared = 1; }

void CMemoryDataOutputStream::Clear()
{
    this->sub.Clear();
    this->count = 0;
}

int main()
{
    CMemoryDataOutputStream s;
    s.count = 12345;
    g_cleared = 0;
    s.Clear();
    if (g_cleared == 1 && s.count == 0) {
        printf("CLEAR_OK\n");
        return 0;
    }
    printf("CLEAR_FAIL\n");
    return 1;
}