// CStringParser::CStringParser(CCharString const&, void(*)(CCharString*), CCharString const&, long) @ 0x00404720
//
// Builds the CBase sub-object, overwrites its vtable with CStringParser's own,
// constructs two embedded CCharString members (+0xc and +0x10), allocates and
// self-links a circular list sentinel node stored at +0x14, assigns the
// caller's input string over the +0x10 member, resolves a non-null "name"
// pointer from that string (falling back to a global empty-string data
// pointer on null), and forwards everything to Init().

struct CCharString
{
    void* rep; // +0x0: pointer to shared string-rep whose first field is the char* data

    CCharString& operator=(const CCharString& src);
};

typedef void (*StringParserErrorCB)(CCharString*);

extern "C" void __fastcall FableStringParserBaseConstruct_0099A2D0(void* self);
extern "C" void __fastcall FableCharStringConstruct_0099E4B0(void* self);
extern "C" void* __cdecl FableAllocate_00BFEA0E(unsigned int size);

struct CStringParser
{
    void* vtbl;              // +0x0
    unsigned char pad[8];    // +0x4 .. +0xb
    CCharString member0c;    // +0xc
    CCharString member10;    // +0x10
    void* sentinel;          // +0x14

    void Init(char* name, StringParserErrorCB cb, CCharString* ctx, long mode);
};

extern "C" void* __fastcall CStringParser_ctor(
    void* self, void*, const CCharString* src, StringParserErrorCB cb, CCharString* ctx, long mode)
{
    CStringParser* self_ = (CStringParser*)self;

    FableStringParserBaseConstruct_0099A2D0(self);
    self_->vtbl = (void*)0x0122d874;

    FableCharStringConstruct_0099E4B0(&self_->member0c);
    FableCharStringConstruct_0099E4B0(&self_->member10);

    self_->sentinel = 0;
    unsigned int* sentinel = (unsigned int*)FableAllocate_00BFEA0E(0x10);
    sentinel[0] = (unsigned int)sentinel;
    sentinel[1] = (unsigned int)sentinel;
    self_->sentinel = sentinel;
    self_->member10 = *src;

    void* srcRep = src->rep;
    char* name;
    if (srcRep == 0)
        goto useDefault;
    name = *(char**)srcRep;
    goto haveName;
useDefault:
    name = (char*)0x0129aaf4;
haveName:
    ;

    self_->Init(name, cb, ctx, mode);
    return self;
}