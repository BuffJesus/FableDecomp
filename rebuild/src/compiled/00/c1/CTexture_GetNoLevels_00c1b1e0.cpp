struct CTexture {
    virtual long slot0(int v);
};

long __fastcall GetNoLevels(CTexture* self, int /*edx*/, void* p) {
    int* ip = (int*)p;
    long r = self->slot0(*ip);
    *ip = 0;
    return r;
}