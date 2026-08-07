struct Sub { void N(void* a); };
struct S { char pad[0x10]; Sub* f10; void WriteToFile(void* a, int b); };
void S::WriteToFile(void* a, int b){ this->f10->N(a); }