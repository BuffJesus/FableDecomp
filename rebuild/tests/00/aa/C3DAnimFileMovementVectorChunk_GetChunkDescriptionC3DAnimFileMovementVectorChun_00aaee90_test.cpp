#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, int n) {
        buf = s;
        len = (n < 0) ? (int)strlen(s) : n;
    }
    const char* buf;
    int len;
};

struct C3DAnimFileMovementVectorChunk {
    virtual ~C3DAnimFileMovementVectorChunk() {}
    virtual CCharString GetChunkDescription() const;
    int dummy;
};

CCharString C3DAnimFileMovementVectorChunk::GetChunkDescription() const {
    return CCharString("MovementVector", -1);
}

int main() {
    C3DAnimFileMovementVectorChunk c;
    c.dummy = 0;
    CCharString r = c.GetChunkDescription();
    if (strcmp(r.buf, "MovementVector") == 0 && r.len == 14) {
        printf("PASS_MOVEVEC_DESC\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}