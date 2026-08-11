#include <cstdio>
struct CThingTrackNode;
struct IDrawableObj { void* vtbl; };
static IDrawableObj g_obj;
static void* g_self=0; static void* g_node=0;
int __fastcall rec(IDrawableObj* self, int /*edx*/, CThingTrackNode* node){ g_self=self; g_node=node; return 7; }
IDrawableObj* __fastcall GetDrawableObject(CThingTrackNode* self){ (void)self; return &g_obj; }
struct CThingTrackNode { IDrawableObj* DrawIsDrawable(); };
int main(){
  static void* vt[0x13];
  vt[0x12]=(void*)&rec;
  g_obj.vtbl=vt;
  CThingTrackNode node;
  IDrawableObj* r=node.DrawIsDrawable();
  if(r==&g_obj && g_self==&g_obj && g_node==&node) printf("00b0f970_TEST PASS\n");
  else printf("FAIL r=%p self=%p node=%p\n",(void*)r,g_self,g_node);
  return 0;
}