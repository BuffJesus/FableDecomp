struct P_00b98d70 { int m[6]; };
struct C_00b98d70 { char pad[0x58]; P_00b98d70 v; unsigned char M(P_00b98d70* out); };
unsigned char C_00b98d70::M(P_00b98d70* out) { *out = this->v; return 1; }