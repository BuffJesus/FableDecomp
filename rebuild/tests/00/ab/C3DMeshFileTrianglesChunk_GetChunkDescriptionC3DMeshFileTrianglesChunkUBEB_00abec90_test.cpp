#include <stdio.h>
#include <string.h>

struct CCharString {
    char* m_data;
    CCharString(const char* s, int n);
};

static char g_buf[64];
CCharString::CCharString(const char* s, int n) {
    strcpy(g_buf, s);
    m_data = g_buf;
    (void)n;
}

struct C3DMeshFileTrianglesChunk {
    void* vptr;
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileTrianglesChunk::GetChunkDescription() const {
    return CCharString("Triangles", -1);
}

int main() {
    C3DMeshFileTrianglesChunk c;
    c.vptr = 0;
    CCharString r = c.GetChunkDescription();
    if (r.m_data && strcmp(r.m_data, "Triangles") == 0) {
        printf("CHUNKDESC_OK\n");
        return 0;
    }
    printf("CHUNKDESC_FAIL\n");
    return 1;
}