// Empty member function from an unoptimised TU: retail keeps the frame and
// the `this` spill. Needs `#pragma optimize("",off)`, which the verifier's
// pragma sweep supplies. __fastcall this=ecx.
#pragma optimize("",off)
struct T { void Hook(); };
void T::Hook() {}