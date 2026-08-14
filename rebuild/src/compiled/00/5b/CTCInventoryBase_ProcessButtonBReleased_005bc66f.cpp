// CTCInventoryBase::ProcessButtonBReleased  @ 0x005bc66f
#pragma optimize("s",on)

struct CTCInventoryBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void SlotA();                 // index 32 -> vtbl+0x80
    virtual void f33(); virtual void f34(); virtual void f35(); virtual void f36();
    virtual void f37(); virtual void f38(); virtual void f39(); virtual void f40();
    virtual void f41(); virtual void f42(); virtual void f43(); virtual void f44();
    virtual void f45();
    virtual void SlotB(int);              // index 46 -> vtbl+0xb8
    char  pad[0x1c-4];
    int   state_1c;
};

void __fastcall ProcessButtonBReleased(CTCInventoryBase* self)
{
    switch (self->state_1c) {
    case 0: self->SlotA(); break;
    case 1: self->SlotB(0); break;
    default: break;
    }
}