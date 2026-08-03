struct T {
  char pad0[4];
  unsigned char f4;
  char pad5[0x26-5];
  unsigned char f26;
  T* InitFlags_Field4_26();
};

T* T::InitFlags_Field4_26() {
  this->f4 = 0xff;
  this->f26 = 0;
  return this;
}