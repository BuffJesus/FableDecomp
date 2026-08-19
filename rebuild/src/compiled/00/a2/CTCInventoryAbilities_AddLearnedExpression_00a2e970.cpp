struct Sub { void M(int* pv); };
struct S { char pad[72]; void Add(int v); };
void S::Add(int v){ ((Sub*)((char*)this+72))->M(&v); }