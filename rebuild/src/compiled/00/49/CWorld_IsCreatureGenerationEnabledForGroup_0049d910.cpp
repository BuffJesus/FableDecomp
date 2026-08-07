struct S { char pad[0xd0]; unsigned int fd0; int M(unsigned int a0); };
int S::M(unsigned int a0){ unsigned int v = this->fd0 & a0; return v == 0; }