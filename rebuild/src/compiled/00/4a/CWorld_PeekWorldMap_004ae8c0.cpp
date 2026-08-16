struct M_004ae8c0;
struct C_004ae8c0 { char pad[0x14]; M_004ae8c0* p; M_004ae8c0& G(); };
M_004ae8c0& C_004ae8c0::G(){ return *this->p; }