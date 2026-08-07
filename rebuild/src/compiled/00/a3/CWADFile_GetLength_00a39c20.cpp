struct Sub { int Method(int); };
struct C { char pad[4]; Sub* f4; int f8; int GetLength(); };
int C::GetLength(){ return this->f4->Method(this->f8); }