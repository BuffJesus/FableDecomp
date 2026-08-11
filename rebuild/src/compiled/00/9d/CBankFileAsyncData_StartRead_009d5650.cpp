// CBankFileAsyncData::StartRead  retail 0x009d5650  __fastcall void(this)
// this->f18 = this->f28->m110->Read(&this->f2c);

struct Arg2c { int a, b, c, d; };

struct Reader {
    // __fastcall member; the call target 0x98f350
    int Read(Arg2c* p);
};

struct Inner {
    char pad[0x110];
    Reader* m110;        // +0x110 : object whose method is called
};

struct CBankFileAsyncData {
    char    pad0[0x18];
    int     f18;         // +0x18  result store
    char    pad1c[0x28 - 0x1c];
    Inner*  f28;         // +0x28  pointer member
    Arg2c   f2c;         // +0x2c  address of this member is passed

    void StartRead();
};

void CBankFileAsyncData::StartRead()
{
    Reader* rd = this->f28->m110;
    this->f18 = rd->Read(&this->f2c);
}