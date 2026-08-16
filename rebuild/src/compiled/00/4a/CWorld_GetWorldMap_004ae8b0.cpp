struct M_004ae8b0;
struct C_004ae8b0 { char pad[0x14]; M_004ae8b0* p; M_004ae8b0& G(); };
M_004ae8b0& C_004ae8b0::G(){ return *this->p; }