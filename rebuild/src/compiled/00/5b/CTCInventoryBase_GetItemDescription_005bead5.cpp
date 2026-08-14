#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryBase::GetItemDescription @ 0x005bead5
// __fastcall void GetItemDescription(CTCInventoryBase*, CWideString* out, long a, bool b)

struct CWideString {
    void* GetSomething();               // 0x5d97b0 (thiscall) -> value
};

struct Tgt5be839 {
    void Finish(CWideString* out, long a, bool b);  // 0x5be839 (thiscall)
};

struct Sub20;
// 0x5c08c0: __fastcall(ecx=self, edx=f24, stack=arg) -> Tgt5be839*
extern Tgt5be839* __fastcall Sub_5c08c0(Sub20* self, long edxArg, void* stackArg);

struct CTCInventoryBase {
    char   pad[0x20];
    Sub20* f20;    // +0x20
    long   f24;    // +0x24
    void GetItemDescription(CWideString* out, long a, bool b);
};

void CTCInventoryBase::GetItemDescription(CWideString* out, long a, bool b)
{
    void* r = out->GetSomething();
    Tgt5be839* v = Sub_5c08c0(this->f20, this->f24, r);
    v->Finish(out, a, b);
}