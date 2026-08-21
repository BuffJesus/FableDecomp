// Store the SECOND stack argument at this+0 and return `this`; the first is
// unused. __fastcall this=ecx (ret 8).
struct T {
    int field;
    T* Init(int unused, int value);
};
T* T::Init(int unused, int value) { this->field = value; return this; }