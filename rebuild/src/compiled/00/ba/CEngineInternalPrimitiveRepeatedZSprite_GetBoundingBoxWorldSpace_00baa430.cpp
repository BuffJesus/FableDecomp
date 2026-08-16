struct P_00baa430 { int m[6]; };
struct C_00baa430 { char pad[0x74]; P_00baa430 v; unsigned char M(P_00baa430* out); };
unsigned char C_00baa430::M(P_00baa430* out) { *out = this->v; return 1; }