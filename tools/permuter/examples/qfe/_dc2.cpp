struct V3 { int x,y,z; };
struct V6 { int m[6]; };
struct P { char a[0xc]; unsigned char fc; char b[0x10-0xd]; unsigned long f10, f14; V3 blk18; unsigned long f24, f28, f2c; V6 blk30; unsigned long f48, f4c, f50; unsigned char f54; };
struct C {
  char a[0xc]; unsigned long fc;
  char b[0x48-0x10]; unsigned char f48;
  char c[0x4c-0x49]; unsigned long f4c, f50;
  V3 blk54;
  unsigned long f60, f64, f68;
  V6 blk6c;
  unsigned long f84, f88;
  unsigned char f8c;
  char d[0x90-0x8d]; unsigned long f90;
  void U(P& p, unsigned long k);
};
void C::U(P& p, unsigned long k){
  this->fc = k;
  this->f48 = p.fc;
  this->f4c = p.f10;
  this->f50 = p.f14;
  this->blk54 = p.blk18;
  this->f60 = p.f24;
  this->f64 = p.f28;
  this->f68 = p.f2c;
  this->blk6c = p.blk30;
  this->f84 = p.f48;
  this->f88 = p.f4c;
  this->f90 = p.f50;
  this->f8c = (unsigned char)((p.f54 >> 7) & 1);
}