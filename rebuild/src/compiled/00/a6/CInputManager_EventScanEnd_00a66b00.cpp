struct S { char pad[0x3408]; int cnt; void M(char* p); };
void S::M(char* p){ *p=0; this->cnt--; }