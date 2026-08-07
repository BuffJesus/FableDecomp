struct CCategory;
struct S { char pad[0x2c]; char* f2c; CCategory* GetCategoryWithIndex(long i); };
CCategory* S::GetCategoryWithIndex(long i){ return (CCategory*)(this->f2c + i*0x58); }