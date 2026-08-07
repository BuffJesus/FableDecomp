struct Arg { void N(void* x); };
struct S { char pad[0x10]; void* f10; void WriteToFile(Arg* a, int b); };
void S::WriteToFile(Arg* a, int b){ a->N(this->f10); }