struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;
struct Helper {
  virtual void slot0();
  virtual void slot1();
  virtual void slot2();
  virtual void slot3();
  virtual void slot4();
  virtual void slot5();
  virtual void slot6();
  virtual bool slot7(CTCCreatureNavigation* nav, C3DVector* dest);
};
struct CTCCreatureNavigation {
  void* f0;
  Helper* f4;
};
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest) {
  return self->f4->slot7(self, dest);
}