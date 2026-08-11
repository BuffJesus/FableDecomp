#include <cstdio>

struct SubBuffer {
    char pad[0x400];
    void Clear();
};

struct CMemoryDataOutputStream {
    char     pad0[0x3404];
    unsigned WritePos;
    char     pad1[0x340c - 0x3408];
    SubBuffer sub;
    void Clear();
};

static int g_subCleared = 0;
void SubBuffer::Clear() { g_subCleared++; }

void CMemoryDataOutputStream::Clear()
{
    this->sub.Clear();
    this->WritePos = 0;
}

int main()
{
    CMemoryDataOutputStream* s = new CMemoryDataOutputStream();
    s->WritePos = 12345;
    g_subCleared = 0;
    s->Clear();
    bool ok = (s->WritePos == 0) && (g_subCleared == 1);
    if (ok) printf("CLEAR_OK\n");
    else    printf("CLEAR_FAIL wp=%u sc=%d\n", s->WritePos, g_subCleared);
    delete s;
    return ok ? 0 : 1;
}