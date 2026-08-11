#include <cstdio>

struct CParticleProgram {
    char pad[0xc0];
    char* data;   // +0xc0
    void RelocateData(long a, void* oldBase, void* newBase, long d);
};

void CParticleProgram::RelocateData(long a, void* oldBase, void* newBase, long d)
{
    if (this->data != 0) {
        this->data = this->data - (long)oldBase + (long)newBase;
    }
}

int main()
{
    char oldBuf[16];
    char newBuf[16];
    CParticleProgram p;

    // non-null: relocate
    p.data = oldBuf + 7;
    p.RelocateData(1, oldBuf, newBuf, 2);
    bool ok1 = (p.data == newBuf + 7);

    // null: unchanged
    p.data = 0;
    p.RelocateData(1, oldBuf, newBuf, 2);
    bool ok2 = (p.data == 0);

    if (ok1 && ok2) {
        printf("RELOC_OK\n");
        return 0;
    }
    printf("RELOC_FAIL\n");
    return 1;
}