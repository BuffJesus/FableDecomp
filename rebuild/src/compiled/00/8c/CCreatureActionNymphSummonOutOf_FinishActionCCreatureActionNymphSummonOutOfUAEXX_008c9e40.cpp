// Byte-exact reconstruction of
// ?FinishAction@CCreatureAction_NymphSummonOutOf@@UAEXXZ  @ 0x008c9e40

// The invoked object at [Inner+0x60]. Its used method is the 37th virtual (byte
// offset 0x90 = index 36). Declaring 36 leading virtuals + the real one makes VC71
// emit a proper __fastcall virtual call (this in ecx, one arg pushed).
struct Target {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void slot36(int a);   // vtable byte offset 0x90
};

struct Inner {
    char pad[0x60];
    Target* pField60;   // [Inner + 0x60]
};

// callee at 0xa01b50: __fastcall(ecx = self+8) -> Inner*
extern Inner* __fastcall sub_a01b50(void* ecxSub);

struct CCreatureAction_NymphSummonOutOf {
    char pad0[8];
    char sub8[4];   // subobject region begins at +8
};

// base method tail-jumped to: 0x859a20, __fastcall(self)
extern void __fastcall base_FinishAction_859a20(CCreatureAction_NymphSummonOutOf* self);

void __fastcall FinishAction(CCreatureAction_NymphSummonOutOf* self)
{
    Inner* p = sub_a01b50((char*)self + 8);
    Target* h = p->pField60;      // ecx = [eax+0x60]; slot fetched from [ecx]
    h->slot36(0);                 // push 0; call [eax+0x90]
    base_FinishAction_859a20(self);
}