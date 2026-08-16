struct Vec3 { int a, b, c; };
struct C { char pad[0x48]; Vec3 pos; unsigned char GetWorldPosition(Vec3* out); };
unsigned char C::GetWorldPosition(Vec3* out) { *out = this->pos; return 1; }