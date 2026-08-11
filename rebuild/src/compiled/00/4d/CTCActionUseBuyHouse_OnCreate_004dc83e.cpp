#pragma optimize("s",on)
// CTCActionUseBuyHouse::OnCreate  @ 0x004dc83e
// retail shape:
//   push esi; mov esi,ecx
//   lea eax,[esi+0xc]; push eax
//   mov eax,[esi+4]; mov ecx,[eax+0x70]; call sub_4dc858
//   mov ecx,esi; pop esi; jmp 0x686870

struct CMemberArg;          // opaque; only its address (this+0xc) is taken

// The object reached via this->m4 ; its +0x70 slot holds a pointer that is the
// 'this' for the inner thiscall.
struct Registrar
{
    // inner thiscall: this = Registrar*, one pointer arg
    void Register(CMemberArg* p);
};

struct Holder
{
    char        pad[0x70];  // 0x00 .. 0x6f
    Registrar*  slot70;     // +0x70
};

struct CTCActionUseBuyHouse
{
    // vtable/base at +0 (unused here)
    void*   v0;             // +0x00
    Holder* m4;             // +0x04
    char    pad8[4];        // +0x08
    char    member0c;       // +0x0c  (address taken)

    void OnCreate();
    void BaseOnCreate();    // tail-called leaf at 0x686870
};

void CTCActionUseBuyHouse::OnCreate()
{
    this->m4->slot70->Register(reinterpret_cast<CMemberArg*>(&this->member0c));
    this->BaseOnCreate();
}