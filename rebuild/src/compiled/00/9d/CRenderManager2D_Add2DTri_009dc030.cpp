#include <new>
#include "fable_display_viewport.h"
namespace _STL {
template<> void vector<CTVertexRHWColSpecTex1>::_M_insert_overflow(CTVertexRHWColSpecTex1*,const CTVertexRHWColSpecTex1&,const __false_type&,size_t,bool);
template<> void vector<CQuickDrawTriInfo>::_M_insert_overflow(CQuickDrawTriInfo*,const CQuickDrawTriInfo&,const __false_type&,size_t,bool);
}
#include "fable_stlport_vector_impl.h"
void CRenderManager2D::Add2DTri(const CTVertexRHWColSpecTex1& a,const CTVertexRHWColSpecTex1& b,const CTVertexRHWColSpecTex1& c,long blendMode,CTexture* texture){
 if(DisplayManager->ViewportOffscreen)return;
 CTVertexRHWColSpecTex1 vertex;
 vertex=a;
 {const C2DBoxF& window=GetCurrentAWindow();vertex.X=vertex.X/(window.right-window.left);}
 vertex.X=(vertex.X+vertex.X)-1.0f;
 {const C2DBoxF& window=GetCurrentAWindow();vertex.Y=vertex.Y/(window.bottom-window.top);}
 vertex.Z=0.0f;vertex.RHW=1.0f;
 vertex.Y=1.0f-(vertex.Y+vertex.Y);
 QuickDraw2DVertices.push_back(vertex);
 vertex=b;
 {const C2DBoxF& window=GetCurrentAWindow();vertex.X=vertex.X/(window.right-window.left);}
 vertex.X=(vertex.X+vertex.X)-1.0f;
 {const C2DBoxF& window=GetCurrentAWindow();vertex.Y=vertex.Y/(window.bottom-window.top);}
 vertex.Z=0.0f;vertex.RHW=1.0f;
 vertex.Y=1.0f-(vertex.Y+vertex.Y);
 QuickDraw2DVertices.push_back(vertex);
 vertex=c;
 {const C2DBoxF& window=GetCurrentAWindow();vertex.X=vertex.X/(window.right-window.left);}
 vertex.X=(vertex.X+vertex.X)-1.0f;
 {const C2DBoxF& window=GetCurrentAWindow();vertex.Y=vertex.Y/(window.bottom-window.top);}
 vertex.Z=0.0f;vertex.RHW=1.0f;
 vertex.Y=1.0f-(vertex.Y+vertex.Y);
 QuickDraw2DVertices.push_back(vertex);
 CQuickDrawTriInfo info;
 info.BlendMode=blendMode;
 if(texture)info.UploadedTexture=*texture;
 info.Window=GetCurrentAWindow();info.Line=false;info.IsText=false;
 QuickDraw2DTriInfo.push_back(info);
}
