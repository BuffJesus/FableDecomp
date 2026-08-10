#include <cstdio>
struct C3DVector { float x,y,z; };
struct CTCCreatureNavigation;
static int g_called = 0;
struct Helper {
  virtual void slot0(){}
  virtual void slot1(){}
  virtual void slot2(){}
  virtual void slot3(){}
  virtual void slot4(){}
  virtual void slot5(){}
  virtual void slot6(){}
  virtual bool slot7(CTCCreatureNavigation* nav, C3DVector* dest){ g_called=1; return dest->x > 0.0f; }
};
struct CTCCreatureNavigation {
  void* f0;
  Helper* f4;
};
bool __fastcall IsNewDestinationGoingToResetNavigation(CTCCreatureNavigation* self, int, C3DVector* dest) {
  return self->f4->slot7(self, dest);
}
int main(){
  Helper h; CTCCreatureNavigation nav; nav.f0=0; nav.f4=&h;
  C3DVector pos; pos.x=1.0f; pos.y=0; pos.z=0;
  bool r = IsNewDestinationGoingToResetNavigation(&nav, 0, &pos);
  if(!g_called){ printf("FAIL not called\n"); return 1; }
  if(r != true){ printf("FAIL true branch\n"); return 1; }
  pos.x = -1.0f; g_called=0;
  bool r2 = IsNewDestinationGoingToResetNavigation(&nav, 0, &pos);
  if(!g_called){ printf("FAIL not called 2\n"); return 1; }
  if(r2 != false){ printf("FAIL false branch\n"); return 1; }
  printf("OK_00c3b2a0\n");
  return 0;
}