// Byte-exact reconstruction of
// CPersistContext_TransferableVector<CHeroMarriageInfo>::Resize @ 0x00c361f0

struct FieldA { int a; };

struct Sub {
    int s;
    // real thiscall members (this in ecx, arg on stack) -> direct call, self-cleanup
    void MethodA(FieldA* arg);   // 0xc457e0
    void MethodB(long n);        // 0xc458d0
};

struct Ctx {
    char   pad[0x44];
    FieldA fieldA;   // +0x44
    char   pad2[0x4c - 0x44 - sizeof(FieldA)];
    Sub    sub;      // +0x4c
    void Resize(long n);
};

void Ctx::Resize(long n)
{
    this->sub.MethodA(&this->fieldA);
    this->sub.MethodB(n);
}