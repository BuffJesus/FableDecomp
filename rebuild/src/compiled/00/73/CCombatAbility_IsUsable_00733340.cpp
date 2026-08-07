struct S { int f0; int f4; char M(); };
char S::M(){ if(this->f4 >= 0) return 1; return this->f0 == -1; }