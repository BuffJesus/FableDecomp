#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryBase::GetItemDescription @ 0x005bf30d

struct CWideString;

struct CTCInventoryBase {
    char pad20[0x20];
    void* member20;    // [esi+0x20] -> ecx (this) of 0x5c08c0
    void* member24;    // [esi+0x24] -> edx of 0x5c08c0
};

// call 0x5d97b0 : __fastcall taking ecx = the CWideString* out, returns a token in eax
extern void* __fastcall sub_5D97B0(CWideString* out);

// call 0x5c08c0 : __fastcall, ecx=member20, edx=member24, one stack arg (pushed eax);
// returns a this-ptr for the final call in eax
extern void* __fastcall sub_5C08C0(void* self, void* edxArg, void* stackArg);

// final tail target 0x5bf1d7 : __fastcall(CWideString* out, long a, bool b) -> void.
// Modelled as a real member so only ecx=this is set (no edx marshalling) and the
// incoming stack args (out,itemId,flag) pass straight through the tail jmp.
struct CFinal {
    void Emit(CWideString* out, long a, bool b);
};

void __fastcall GetItemDescription(CTCInventoryBase* self, void* edxDummy,
                                   CWideString* out, long itemId, bool flag)
{
    (void)edxDummy;
    void* t = sub_5D97B0(out);
    CFinal* inner = (CFinal*)sub_5C08C0(self->member20, self->member24, t);
    inner->Emit(out, itemId, flag);
}