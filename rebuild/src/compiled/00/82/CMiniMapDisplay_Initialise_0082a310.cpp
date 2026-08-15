// Byte-exact reconstruction of CMiniMapDisplay::Initialise @ 0x0082a310
// __fastcall(CMiniMapDisplay* self, float) -> void
// add ecx,0x20 ; call 0x440730 (subobject method) ; mov byte [g]=0 ; ret 4
//
// Model this-call as a leading pointer param (ecx under __fastcall).

struct Sub;  // subobject living at CMiniMapDisplay+0x20

// callee 0x440730 : __fastcall on Sub (no extra args) -> ecx=Sub*
extern void __fastcall Sub_Reset(Sub* self);

struct CMiniMapDisplay {
    char pad[0x20];
    Sub* subAt20;   // &(this->pad[0x20]) == this+0x20
};

// global flag at 0x13b86c4
extern unsigned char g_miniMapFlag;

// Author Initialise as a __fastcall free fn: arg1=this in ecx, arg2(edx dummy) then stack float.
// Actually the retail sig is __fastcall(self,float): ecx=this, float on stack, ret 4.
void __fastcall Initialise(CMiniMapDisplay* self, float /*unused*/)
{
    Sub* p = reinterpret_cast<Sub*>(reinterpret_cast<char*>(self) + 0x20);
    Sub_Reset(p);           // add ecx,0x20 ; call 0x440730
    g_miniMapFlag = 0;      // mov byte [0x13b86c4],0
}                           // ret 4