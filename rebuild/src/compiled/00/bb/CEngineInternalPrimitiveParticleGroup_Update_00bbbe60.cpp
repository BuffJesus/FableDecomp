struct V4 { int m[4]; };
struct P { char pad0[0xc]; unsigned long fc; unsigned char f10, f11; char pad12[2]; unsigned long f14; char pad18[1]; unsigned char f19, f1a, f1b; V4 blk1c; };
struct C {
  char pad0[0xc]; unsigned long fc;
  char pad10[0x27-0x10]; unsigned char f27;
  char pad28[0x30-0x28]; unsigned long f30;
  char pad34[0x5c-0x34]; V4 blk5c;
  unsigned long f6c;
  char pad70[0xa0-0x70]; unsigned char fa0, fa1;
  char pada2[0xa4-0xa2]; unsigned long fa4;
  unsigned char fa8, fa9, faa;
  void U(P& p, unsigned long k);
};
void C::U(P& p, unsigned long k){
  this->fc = k;
  this->fa0 = p.f10;
  this->fa1 = p.f11;
  this->f27 = 0xff;
  this->f6c = p.fc;
  this->blk5c = p.blk1c;
  this->fa4 = p.f14;
  this->fa8 = p.f19;
  this->fa9 = p.f1a;
  this->faa = p.f1b;
  switch (this->fa4) {
    case 0: this->f30 = 0x8000; break;
    case 1: this->f30 = 0x100000; break;
    case 2: this->f30 = 0x80000; break;
  }
}