// Store both stack arguments at this+0 / this+4 and return `this`.
// __fastcall this=ecx (ret 8).
struct T {
    int first;
    int second;
    T* Init(int a, int b);
};
T* T::Init(int a, int b) {
    this->first = a;
    this->second = b;
    return this;
}