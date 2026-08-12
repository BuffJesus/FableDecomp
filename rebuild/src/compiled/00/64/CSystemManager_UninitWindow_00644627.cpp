#pragma optimize("s",on)
// CSystemManager::UninitWindow  @ 00644627  (VC7.1 /O2 /Oy)

struct Temp {
    int slot;
};

// callee at 0x43f360 : __fastcall(this=temp, edx=arg, stackarg0, stackarg1)
void __fastcall Temp_helper(Temp* self, int edx, int a0, int a1);

struct CSystemManager {
    char pad0[0x18];
    unsigned char byte18;   // +0x18
    int   field1c;          // +0x1c
    int   field20;          // +0x20
    void UninitWindow();
};

void CSystemManager::UninitWindow()
{
    Temp t;
    if (this->byte18 == 0) {
        Temp_helper(&t, this->field1c, this->field20, 0x41be70);
    }
}