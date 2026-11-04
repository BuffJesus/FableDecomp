// Reconstruction of CVolume::~CVolume (retail 0x009fccf0)
// Pattern: set vtable, call a member cleanup helper with arg 1, tail-call base dtor.

class CBase {
public:
    CBase();
    ~CBase();               // 0x0099a2e0 (base destructor, tail-called)
    virtual void v0();
};

class CVolume : public CBase {
public:
    CVolume();
    ~CVolume();
    void ReleaseResources(int flag);   // 0x00a67460 (member helper, __fastcall + stack arg)
    virtual void v0();
};

CVolume::~CVolume()
{
    ReleaseResources(1);
}