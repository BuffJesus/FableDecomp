struct S { int f0; bool IsEquivalent(S* other); };
bool S::IsEquivalent(S* other){ return this->f0 == other->f0; }