// Lift target #1: CTCPhysicsBase::GetAirResistance @0x00662030 (retail Fable.exe).
// Retail decomp (BSim sim=1.0 vs ego_r):
//   float __thiscall CTCPhysicsBase::GetAirResistance(void) const
//   { return *(float*)(*(int*)(this + 0xe0) + 0x78); }
// Retail bytes (10): 8b 81 e0 00 00 00  d9 40 78  c3
//   mov eax,[ecx+0xe0]   ; load the member pointer at this+0xe0
//   fld dword [eax+0x78] ; push *(float*)(member+0x78) onto the x87 stack (return)
//   ret
//
// Lifted as a standalone __thiscall free function so it compiles with no engine
// headers; the class layout is expressed via raw offset arithmetic (matching the
// decomp). Compile 32-bit with VC7.1 (cl 13.10) and diff .text against the bytes
// above. This is the bar-#2 harness proof: pseudo-C -> compilable C++ -> object.

typedef unsigned char* thisptr;

// __thiscall puts `this` in ECX, matching the retail calling convention.
float __fastcall GetAirResistance(thisptr self, void* /*edx unused*/) {
    // *(float*)(*(int*)(self + 0xe0) + 0x78)
    int member = *reinterpret_cast<int*>(self + 0xe0);
    return *reinterpret_cast<float*>(member + 0x78);
}
