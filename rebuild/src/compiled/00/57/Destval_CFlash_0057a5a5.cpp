extern int g_glob;
struct Sub { void M(int* p); };
struct S { char pad[0x170]; void Dtor(); };
void S::Dtor(){ ((Sub*)((char*)this+0x170))->M(&g_glob); }