#pragma optimize("s", on)
#pragma optimize("y", off)

// NUISystem WaypointData::~WaypointData  @ 0x00472B63  (__fastcall, ret 4)
//
// Retail idiom:
//   push ebp / mov ebp,esp / sub esp,0x40    -> 0x40-byte stack temporary [ebp-0x40]
//   push esi / mov esi,ecx                    -> save this
//   lea ecx,[ebp-0x40] / call <ctor>          -> temp.Construct()   (masked)
//   push &temp / push [ebp+8] / mov ecx,esi
//   mov [ebp-0x40],0x123543c                  -> temp.head = &vtblApply
//   call <apply>                              -> this->Apply(arg, &temp)  (masked, fwd)
//   lea ecx,[ebp-0x40]
//   mov [ebp-0x40],0x1230ba0                  -> temp.head = &vtblDestroy
//   call <dtor>                               -> temp.Destroy()   (masked)
//   pop esi / leave / ret 4
//
// The two immediate stores are the classic MSVC inlined vptr stamps: a local
// object whose vtable pointer is set after construction (derived table) and
// re-set before teardown (base table).  Modelled as explicit head-pointer
// assignments so the compiler emits the two `mov [ebp-0x40], imm32` stores
// exactly where retail schedules them.  All three callees and the two vtable
// addresses are relocation-masked in parity, so they are declared extern.

extern void* const WaypointApplyVtbl_0123543c[];    // 0x0123543c
extern void* const WaypointDestroyVtbl_01230ba0[];  // 0x01230ba0

// 0x40-byte stack temporary.  head = inlined vptr slot; Construct/Destroy are
// this-in-ecx members taking no stack args (masked callees).
struct WaypointTemp {
    void* head;                 // +0x00  inlined vtable pointer slot
    char  pad[0x40 - 4];        // pad out to the full 0x40-byte frame temp
    void Construct();           // lea ecx,[ebp-0x40]; call
    void Destroy();             // lea ecx,[ebp-0x40]; call
};

struct WaypointData {
    // this-in-ecx member taking two stack args (arg, then &temp pushed first):
    //   push &temp; push arg; mov ecx,this; call
    void Apply(void* arg, WaypointTemp* temp);

    void Dtor(void* arg);       // the target: ~WaypointData(arg), ret 4
};

void WaypointData::Dtor(void* arg)
{
    WaypointTemp temp;

    temp.Construct();
    temp.head = (void*)WaypointApplyVtbl_0123543c;
    this->Apply(arg, &temp);
    temp.head = (void*)WaypointDestroyVtbl_01230ba0;
    temp.Destroy();
}