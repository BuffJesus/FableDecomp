extern unsigned short g_frame;
struct S { char pad[0x36]; unsigned short w36; int M(); };
int S::M(){ unsigned short v=this->w36; return v == g_frame; }