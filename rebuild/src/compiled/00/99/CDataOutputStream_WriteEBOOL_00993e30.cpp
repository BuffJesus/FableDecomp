// CDataOutputStream::WriteEBOOL — retail 0x00993e30
// __fastcall void WriteEBOOL(CDataOutputStream* this, bool value)

struct CDataOutputStream
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    // slot 4 (0x10): raw write(data, size)
    virtual void Write(void* data, int size);

    void WriteEBOOL(bool value);
};

void CDataOutputStream::WriteEBOOL(bool value)
{
    unsigned char b = value ? 1 : 0;
    this->Write(&b, 1);
}