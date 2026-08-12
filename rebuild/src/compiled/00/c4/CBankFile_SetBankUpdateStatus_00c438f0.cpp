// CBankFile::SetBankUpdateStatus  @ 0x00c438f0
// void __fastcall(CBankFile* this, CCharString* status)
//   this->field_f0 = status;
//   if (this->sub_50) this->sub_50->vtbl[14](status);

class CCharString;

struct Sub
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void Notify(CCharString* status);   // slot 14 -> vtbl+0x38
};

struct CBankFile
{
    char pad0[0x50];
    Sub* sub;             // +0x50
    char pad1[0xf0 - 0x54];
    CCharString* status;  // +0xf0

    void SetBankUpdateStatus(CCharString* s);
};

void CBankFile::SetBankUpdateStatus(CCharString* s)
{
    this->status = s;
    Sub* p = this->sub;
    if (p)
        p->Notify(s);
}