// CFileDataOutputStream::Write forwarder -> sink->vslot4(n, p, 0)
struct ISink {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(const void* p, long n, int zero);
};
struct CFileDataOutputStream {
    void* field0;
    void* field4;
    ISink* sink;   // +8
};

void __fastcall Write(CFileDataOutputStream* self, int /*edx*/, const void* p, long n)
{
    self->sink->v4(p, n, 0);
}