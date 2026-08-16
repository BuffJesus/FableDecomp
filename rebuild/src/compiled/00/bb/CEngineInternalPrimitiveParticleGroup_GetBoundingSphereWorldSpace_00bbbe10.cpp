struct P_00bbbe10 { int m[4]; };
struct C_00bbbe10 { char pad[0x5c]; P_00bbbe10 v; unsigned char M(P_00bbbe10* out); };
unsigned char C_00bbbe10::M(P_00bbbe10* out) { *out = this->v; return 1; }