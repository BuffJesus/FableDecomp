struct CSystemManager; struct S { char pad[4]; void* f4; int Initialise(CSystemManager* a); };
int S::Initialise(CSystemManager* a){ this->f4 = a; return 1; }