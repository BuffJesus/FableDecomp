// RemoveFromGlobalTCs @ 00686df0, module CTCBase, __fastcall(CTCBase*)
struct CTCBase;

// helper at 0x4cc150 is a real member (this in ecx, one stack arg, callee-clean).
struct GContainer {
    void Remove(CTCBase** pp);   // __fastcall member
};
extern GContainer g_container;

struct CTCBase {
    char pad[9];
    char byte9;  // +0x9
};

void __fastcall RemoveFromGlobalTCs(CTCBase* self)
{
    self->byte9 = 0;
    CTCBase* local = self;
    g_container.Remove(&local);
}