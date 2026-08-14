#pragma optimize("s",on)
// Byte-exact reconstruction of CTCInventoryItem::OnDie @ 0x004e30c8
// __fastcall (this in ecx) -> modeled as __fastcall free fn (self in ecx)

typedef unsigned int uint;

struct CTCInventoryItem
{
    void* vtbl;        // +0x00
    char  pad[0x54];   // +0x04 .. +0x57
    void* sub;         // +0x58
};

// cdecl free-style callee @ 0xbfea14 (push arg; call; pop ecx cleanup)
extern "C" void __cdecl freeThing(void* p);

// base OnDie (tail-called with this in ecx) @ 0x9fc550
void __fastcall BaseOnDie(CTCInventoryItem* self);

void __fastcall OnDie(CTCInventoryItem* self)
{
    if (self->sub)
        freeThing(self->sub);
    self->vtbl = (void*)0x1230ba0;
    BaseOnDie(self);
}