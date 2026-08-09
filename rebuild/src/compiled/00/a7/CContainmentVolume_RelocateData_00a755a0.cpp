struct CContainmentVolume {
  int dummy0;
  char* data;
};

void __fastcall RelocateData(CContainmentVolume* self, int /*edx*/, long a1, void* oldBase, void* newBase, long a4) {
  (void)a1; (void)a4;
  if (self->data) {
    self->data = self->data - (char*)oldBase + (char*)newBase;
  }
}