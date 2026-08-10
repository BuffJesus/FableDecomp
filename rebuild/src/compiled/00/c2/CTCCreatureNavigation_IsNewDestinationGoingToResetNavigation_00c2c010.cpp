struct C3DVector { float x, y, z; };
struct Sub {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual bool slot6(void* a, C3DVector* b);
};
struct CTCCreatureNavigation {
  void* f0;
  Sub* f4;
  bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};
bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
  return this->f4->slot6(this, dest);
}