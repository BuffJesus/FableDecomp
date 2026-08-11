// CModeChargeForcePush::OnActivate  @ 008d6980  __fastcall void(this)
//
// push esi; mov esi,ecx; call 0x861d00 (base OnActivate, __fastcall this)
// mov ecx,[esi+4]              ; ecx = this->sub
// cmp byte [ecx+0x10],1        ; sub->flag == 1 ?
// pop esi; jne skip
// push 0; push 0; call 0x833a10  ; sub->Helper(0,0)  __fastcall (ecx=sub, stack a,b)
// skip: ret
//
// The pointer at +4 is loaded into ecx because it is BOTH the object whose flag
// is tested AND the `this` for the conditional call 0x833a10.

struct Sub {
    char pad[0x10];
    unsigned char flag;          // +0x10
    void Helper(int a, int b);   // __fastcall member (ecx=this) -> call 0x833a10
};

struct CModeChargeForcePush {
    void* vt;                    // +0x00
    Sub*  sub;                   // +0x04

    void BaseActivate();         // __fastcall base OnActivate (call 0x861d00)
    void OnActivate();
};

void CModeChargeForcePush::OnActivate()
{
    BaseActivate();
    Sub* s = sub;
    if (s->flag == 1)
        s->Helper(0, 0);
}