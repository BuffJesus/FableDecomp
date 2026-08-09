struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct IDelegate {
  virtual bool m0(CTCCreatureNavigation*, C3DVector*);
  virtual bool m1(CTCCreatureNavigation*, C3DVector*);
  virtual bool m2(CTCCreatureNavigation*, C3DVector*);
  virtual bool m3(CTCCreatureNavigation*, C3DVector*);
  virtual bool m4(CTCCreatureNavigation*, C3DVector*);
  virtual bool m5(CTCCreatureNavigation*, C3DVector*);
  virtual bool m6(CTCCreatureNavigation*, C3DVector*);
};
struct CTCCreatureNavigation {
  void* f0;
  IDelegate* f4;
  bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
  return this->f4->m6(this, dest);
}