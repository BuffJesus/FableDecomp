// ?OnReplace@CAIStateGroup_BanditGossip@@MAEXXZ  @ 0x0090e590
// void __fastcall CAIStateGroup_BanditGossip::OnReplace(this)
//
//   push esi
//   mov  esi, ecx                 ; esi = this
//   call 0x90ca60                 ; base OnReplace(this)      (ecx = this)
//   mov  al, [esi+0x24]
//   test al, al
//   je   .ret
//   mov  eax, [esi+4]             ; owner = this->owner
//   mov  ecx, [eax+0x18]          ; ecx = *(owner+0x18)
//   pop  esi
//   jmp  0x664a50                 ; tail call, ecx = *(owner+0x18)
// .ret:
//   pop esi
//   ret

// external targets (become masked relocations in the .obj compare)
extern "C" void __fastcall BaseGroupOnReplace(void* self);     // 0x90ca60
extern "C" void __fastcall SubNotify(void* self);              // 0x664a50

struct CAIStateGroup_BanditGossip {
    void*  vtbl;          // +0x00
    void** owner;         // +0x04  (owner; owner[0x18/4] loaded into ecx)
    char   pad08[0x24 - 0x08];
    char   flag;          // +0x24
    void OnReplace();
};

void CAIStateGroup_BanditGossip::OnReplace()
{
    BaseGroupOnReplace(this);
    if (this->flag) {
        // owner+0x18 dereferenced -> ecx target for tail call
        void* target = this->owner[0x18 / sizeof(void*)];
        SubNotify(target);
    }
}