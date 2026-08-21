// Assign the stack argument to the member at this+0 and return *this.
// __fastcall this=ecx (ret 4).
struct T {
    int field;
    T& Assign(int value);
};
T& T::Assign(int value) { this->field = value; return *this; }