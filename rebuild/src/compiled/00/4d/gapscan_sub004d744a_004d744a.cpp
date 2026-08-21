// Returns `this` unchanged. __fastcall this=ecx.
struct T { T* Self(int a0); };
T* T::Self(int a0) { return this; }