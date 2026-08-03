// CTCInGameMenu::OnCreate @ 0x0047d60d  (__fastcall, void, 59 bytes)
//
// Genuine VC7.1 (cl /c /O2 /Oy /W3) reconstruction -- byte-exact vs retail
// (the two call rel32 operands are relocation-masked). Frame pointers are
// forced on (retail keeps push ebp/leave) via #pragma optimize("y",off);
// size optimization ("s",on) reproduces the `and [ebp-4],0` zero-init idiom.
//
// The body is the classic inlined intrusive-counted-pointer pattern:
//   * a zero-initialised counted-pointer local `h` (holds the object ptr),
//   * a getter that fills it: this->field4->provider70->GetHeld(&h)
//     (ecx = this->field4->[+0x70]; &h pushed),
//   * a member call consuming the held object's +0xa8 field:
//     this->ApplyGuiField(o->fieldA8)  (ecx = this),
//   * the inlined counted-pointer release: --o->refs; if it hit zero, call the
//     object's virtual slot 1 (vtbl[+4]) -> `dec [esi+4]; jne; mov eax,[esi];
//     mov ecx,esi; call [eax+4]`.
// Caching h.obj into the local `o` and using it for BOTH the apply argument and
// the release is what makes cl keep the object live in esi (this stays in edi)
// across the apply call, matching the retail register allocation exactly.

#pragma optimize("y", off)
#pragma optimize("s", on)

struct GuiObject {
    virtual void v0();          // vtbl+0x0
    virtual void v1();          // vtbl+0x4  -- destroy-on-zero-refcount
    int          refs;          // +0x4
    char         pad[0xa8 - 8]; // +0x8 .. +0xa7
    int          fieldA8;       // +0xa8
};

struct GuiHolder { GuiObject* obj; GuiHolder() : obj(0) {} };

struct GuiProvider {
    void GetHeld(GuiHolder* out);   // __fastcall, ecx = this, one stack arg
};

struct Sub4 {
    char         pad[0x70];
    GuiProvider* provider70;        // +0x70
};

struct CTCInGameMenu {
    void* v0field;                  // +0x0
    Sub4* field4;                   // +0x4
    void ApplyGuiField(int v);      // __fastcall member (masked callee)
    void OnCreate();
};

void CTCInGameMenu::OnCreate()
{
    GuiHolder h;
    field4->provider70->GetHeld(&h);
    GuiObject* o = h.obj;
    ApplyGuiField(o->fieldA8);
    if (--o->refs == 0)
        o->v1();
}