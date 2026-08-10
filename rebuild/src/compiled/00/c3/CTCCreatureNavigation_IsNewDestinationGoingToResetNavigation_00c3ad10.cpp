struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct IHelper {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual bool v7(CTCCreatureNavigation* nav, C3DVector* dest);
};
struct CTCCreatureNavigation {
  int f0;
  IHelper* f4;
};
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int /*edx*/, C3DVector* dest){
  return self->f4->v7(self, dest);
}