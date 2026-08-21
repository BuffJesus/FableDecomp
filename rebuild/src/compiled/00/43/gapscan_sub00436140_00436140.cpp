// Returns `this` unchanged. __fastcall this=ecx.
struct T { T* Self(); };
T* T::Self() { return this; }