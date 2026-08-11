#include <cstdio>

struct CCharString {
    char* m_data;
    CCharString(const char* s, int len);
};

__declspec(noinline) CCharString::CCharString(const char* s, int len) {
    m_data = (char*)s;
    if (len) m_data = (char*)s + 1;
}

struct C3DMeshFileXVertexDataChunk {
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXVertexDataChunk::GetChunkDescription() const {
    return CCharString((const char*)0x40, -1);
}

int main() {
    C3DMeshFileXVertexDataChunk c;
    CCharString r = c.GetChunkDescription();
    // len=-1 (nonzero) -> s+1 == 0x41
    if (r.m_data == (char*)0x41) {
        printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    printf("FAIL %p\n", r.m_data);
    return 1;
}