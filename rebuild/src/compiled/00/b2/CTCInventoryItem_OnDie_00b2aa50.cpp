// CTCInventoryItem::OnDie @ 0x00b2aa50
// void __fastcall OnDie(CTCInventoryItem* this)
//   eax = this->field_0x208;
//   if (eax) { cdecl_free(eax); }   // add esp,4 cleanup
//   BaseOnDie(this);                // this-call tail jmp

struct CTCInventoryItem {
    char pad[0x208];
    void* p208;   // +0x208
};

// cdecl callee at 0xbfea14 (operator delete / free style, single ptr arg, caller cleans)
extern "C" void __cdecl EngineFree(void* p);

// this-call base OnDie at 0xb59730, value model as __fastcall(self)
extern void __fastcall BaseOnDie(CTCInventoryItem* self);

void __fastcall OnDie(CTCInventoryItem* self)
{
    void* p = self->p208;
    if (p) {
        EngineFree(p);
    }
    BaseOnDie(self);
}