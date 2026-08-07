struct CRegion;
struct S { char pad[0x2c]; char* f2c; CRegion& GetRegion(long i); };
CRegion& S::GetRegion(long i){ return *(CRegion*)(this->f2c + i*0x58); }