// Byte-exact reconstruction of CTCInventoryExperience::IsTutorialPlayed @ 0x009a8090

struct CCharString;

struct Sub {
    void* first;   // +0
    char pad[0x20];
    void* f24;     // +0x24
    // native __fastcall member (this in ecx, arg on stack, callee cleans)
    void* Find(CCharString* p);
};

struct CTCInventoryExperience {
    void* head;    // +0
    Sub   sub;     // +4
};

// ret 8 => two stack dwords: arg1 (p) and arg2 (q). Returns eax (the Find result).
void* __fastcall IsTutorialPlayed(CTCInventoryExperience* self, void* edx, CCharString* p, CCharString* q)
{
    Sub* s = &self->sub;
    void* r = s->Find(p);
    if (r != s->first) {
        ((Sub*)r)->f24 = q;
    }
    return r;
}