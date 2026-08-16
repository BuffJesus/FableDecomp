struct V4 { int m[4]; };
struct V6 { int m[6]; };
struct V12 { int m[12]; };
struct P { char a[0xc]; V4 blkc; V6 blk1c; V12 blk34; unsigned long f64, f68; };
struct C {
  char a[0xc]; unsigned long fc;
  char b[0x48-0x10]; V4 blk48; V6 blk58; V12 blk70; unsigned long fa0, fa4;
  void Method(int x);
  void U(P& p, unsigned long k);
};
void C::U(P& p, unsigned long k){
  this->fc = k;
  this->blk48 = p.blkc;
  this->blk58 = p.blk1c;
  this->blk70 = p.blk34;
  this->fa0 = p.f64;
  this->fa4 = p.f68;
  this->Method(0);
}