struct S { void* f0; bool op(void* t); };
bool S::op(void* t){ return this->f0 != t; }