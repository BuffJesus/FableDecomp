// CParticleProgram::RelocateData  0x00ae04c0
// void __fastcall(CParticleProgram*, long a, void* oldBase, void* newBase, long d)

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