#include <cstdio>
struct CInterpolationInfo;
struct IRenderable { virtual void Draw(CInterpolationInfo* info, bool b)=0; };
struct CDisplayViewManager { IRenderable* pView; void Render(CInterpolationInfo* info, bool b); };
extern IRenderable* __fastcall GetFallbackView_c31a70(CDisplayViewManager* self);
static int g_drawn=0; static bool g_b=false; static CInterpolationInfo* g_info=0;
struct View : IRenderable { void Draw(CInterpolationInfo* info, bool b){ g_drawn++; g_info=info; g_b=b; } };
static View g_fallback;
IRenderable* __fastcall GetFallbackView_c31a70(CDisplayViewManager*){ return &g_fallback; }
int main(){
  CInterpolationInfo* info=(CInterpolationInfo*)0x1234;
  View v; CDisplayViewManager m; m.pView=&v;
  m.Render(info,true);
  bool ok1=(g_drawn==1 && g_b==true && g_info==info && &v==(IRenderable*)m.pView);
  g_drawn=0; CDisplayViewManager m2; m2.pView=0;
  m2.Render(info,false);
  bool ok2=(g_drawn==1 && g_b==false && g_info==info);
  if(ok1&&ok2) printf("00c31b30_TEST PASS\n"); else printf("FAIL d=%d b=%d\n",g_drawn,(int)g_b);
  return 0;
}