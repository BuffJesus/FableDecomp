#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct IDelegate {
  virtual bool m6(CTCCreatureNavigation* n, C3DVector* v) = 0;
};
struct MockDelegate : IDelegate {
  int calls;
  CTCCreatureNavigation* lastN;
  C3DVector* lastV;
  bool ret;
  bool m6(CTCCreatureNavigation* n, C3DVector* v) { calls++; lastN=n; lastV=v; return ret; }
};
struct CTCCreatureNavigation {
  void* f0;
  IDelegate* f4;
  bool IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
    return this->f4->m6(this, dest);
  }
};
int main() {
  MockDelegate d; d.calls=0; d.lastN=0; d.lastV=0; d.ret=true;
  CTCCreatureNavigation nav; nav.f0=0; nav.f4=&d;
  C3DVector v; v.x=1; v.y=2; v.z=3;
  bool r = nav.IsNewDestinationGoingToResetNavigation(&v);
  if (!(r==true && d.calls==1 && d.lastN==&nav && d.lastV==&v)) { printf("FAIL\n"); return 1; }
  d.ret=false;
  bool r2 = nav.IsNewDestinationGoingToResetNavigation(&v);
  if (!(r2==false && d.calls==2)) { printf("FAIL\n"); return 1; }
  printf("OK_00c22f80\n");
  return 0;
}