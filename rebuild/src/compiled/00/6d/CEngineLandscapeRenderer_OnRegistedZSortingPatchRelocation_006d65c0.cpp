struct S { char pad[0x44]; int* f44; void M(int a0, int a1); };
void S::M(int a0, int a1){ this->f44[a0] = a1; }