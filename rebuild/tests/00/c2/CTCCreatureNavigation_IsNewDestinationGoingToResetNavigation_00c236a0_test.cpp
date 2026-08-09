#include <cstdio>
struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;
struct Sub;
struct SubVtbl {
  bool (*slot6)(Sub*, CTCCreatureNavigation*, C3DVector*);
};
struct Sub { void* vt; };
struct CTCCreatureNavigation { void* pad0; Sub* sub; };

static bool g_ret;
static Sub* g_seen_sub;
static CTCCreatureNavigation* g_seen_nav;
static C3DVector* g_seen_dest;

static bool model(CTCCreatureNavigation* self, C3DVector* dest) {
  Sub* s = self->sub;
  g_seen_sub = s;
  g_seen_nav = self;
  g_seen_dest = dest;
  return g_ret;
}

int main() {
  Sub subObj;
  CTCCreatureNavigation nav;
  C3DVector vecD;
  Sub* subPtr = &subObj;
  CTCCreatureNavigation* navPtr = &nav;
  C3DVector* dPtr = &vecD;
  nav.sub = subPtr;
  g_ret = true;
  bool r1 = model(navPtr, dPtr);
  bool ok1 = (r1 == true) && (g_seen_sub == subPtr) && (g_seen_nav == navPtr) && (g_seen_dest == dPtr);
  g_ret = false;
  bool r2 = model(navPtr, dPtr);
  bool ok2 = (r2 == false);
  if (ok1 && ok2) { printf("OK_00c236a0\n"); return 0; }
  printf("FAIL\n"); return 1;
}