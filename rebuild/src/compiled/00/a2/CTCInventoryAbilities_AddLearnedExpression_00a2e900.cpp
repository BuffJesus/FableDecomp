struct Sub { void M(int* pv); };
struct S { char pad[56]; void Add(int v); };
void S::Add(int v){ ((Sub*)((char*)this+56))->M(&v); }