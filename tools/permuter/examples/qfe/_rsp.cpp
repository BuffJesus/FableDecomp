struct Y { char pad[0x48]; unsigned char f48; };
struct X { char pad[0xc]; Y* y; };
extern "C" void __stdcall RenderY(Y* y);
struct C { void RenderSubPrimitive(X* p, int extra); };
void C::RenderSubPrimitive(X* p, int extra) { if (p->y->f48) RenderY(p->y); }