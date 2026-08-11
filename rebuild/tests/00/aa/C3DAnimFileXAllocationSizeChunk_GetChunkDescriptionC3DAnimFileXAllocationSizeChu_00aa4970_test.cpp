#include <stdio.h>
#include <string.h>

struct CCharString {
    char* m_data;
    CCharString(const char* s, int n);
};

static char g_buf[64];
CCharString::CCharString(const char* s, int n) {
    // emulate: length -1 means strlen
    strcpy(g_buf, s);
    m_data = g_buf;
    (void)n;
}

struct C3DAnimFileXAllocationSizeChunk {
    void* vptr;
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DAnimFileXAllocationSizeChunk::GetChunkDescription() const {
    return CCharString("AllocationSize", -1);
}

int main() {
    C3DAnimFileXAllocationSizeChunk c;
    c.vptr = 0;
    CCharString r = c.GetChunkDescription();
    if (r.m_data && strcmp(r.m_data, "AllocationSize") == 0) {
        printf("CHUNKDESC_OK\n");
        return 0;
    }
    printf("CHUNKDESC_FAIL\n");
    return 1;
}