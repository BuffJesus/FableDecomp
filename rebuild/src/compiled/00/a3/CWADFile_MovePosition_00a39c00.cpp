struct S { char pad[0xc]; long fv; void MovePosition(long a); };
void S::MovePosition(long a){ this->fv += a; }