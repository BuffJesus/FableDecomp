// CTCKraken::EndAttackBeam @ 0045c5f6
// push esi; mov esi,ecx; lea ecx,[esi+0x70]; mov [ecx],vtbl(0x1230ba0);
// call 0x9fc550(ecx); mov ecx,esi; pop esi; jmp 0x454b4c(this)

struct Sub {
    void** vtbl;
};

// vtbl address 0x1230ba0 — model as an extern global we take the address of
extern void* Sub_vtbl_1230ba0;

// helper at 0x9fc550, __fastcall on the subobject (ecx = sub)
void __fastcall helper_9fc550(Sub* sub);

struct CTCKraken {
    char pad[0x70];
    Sub sub;   // at +0x70

    void EndAttackBeam();
    void Base454b4c();  // base method tail-called at 0x454b4c
};

void CTCKraken::EndAttackBeam()
{
    this->sub.vtbl = (void**)&Sub_vtbl_1230ba0;
    helper_9fc550(&this->sub);
    this->Base454b4c();
}