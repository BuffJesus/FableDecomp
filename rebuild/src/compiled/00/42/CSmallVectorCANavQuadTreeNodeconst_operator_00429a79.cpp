// operator[]: `return this->base + index;`. __fastcall this=ecx, index=stack (ret 4).
struct T { int* base; int* At(int index); };
int* T::At(int index) { return this->base + index; }