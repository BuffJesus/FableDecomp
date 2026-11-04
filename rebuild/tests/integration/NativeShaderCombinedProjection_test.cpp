// CShaderRenderManager::UpdateCombinedProjectionTransform (retail 0x00988A50) against the transplanted
// retail bytes: same generated world/view/projection matrices, GEnableSIMD off and on, x87 precision
// 24/53/64; combined matrix, uploaded constants, call arguments and update flags must be bit-identical.
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <stddef.h>
#include <math.h>
#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "fable_matrix4x4_simd.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static unsigned row,mode,approximate;static double worst;
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"COMBINED_PROJECTION_FAILURE row=%u mode=%u line=%d\n",row,mode,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
struct Call{unsigned count;void* device;long start;const void* data;unsigned long vectors;unsigned char values[64];};
static Call calls[2];
static void* deviceTable[119];static void* device[1]={deviceTable};
static HRESULT __stdcall Capture(void* self,long start,const void* data,unsigned long vectors){Call& c=calls[mode];++c.count;c.device=self;c.start=start;c.data=data;c.vectors=vectors;if(vectors==4)memcpy(c.values,data,64);return 0x80004005;}
static unsigned seed=0x12345678;
static unsigned Next(){seed=seed*1664525u+1013904223u;return seed;}
static float Value(unsigned kind){unsigned bits;switch(kind){
 case 0:return (float)((int)(Next()%65)-32)/4.0f; // quarter steps in [-8,8]: every intermediate exact in 24 bits
 case 1:bits=(Next()&0x807fffff)|((Next()%254+1)<<23);break; // any normal
 case 2:bits=Next()&0x807fffff;break;                         // denormal or zero
 case 3:return (Next()&1)?0.0f:-0.0f;
 case 5:return (float)((int)(Next()%2001)-1000)/64.0f; // moderate: products and sums round
 default:bits=(Next()&1)?0x7f800000u|(Next()&0x80000000u):0x7fc00000u|(Next()&0x003fffff);break;} // inf / nan
 float f;memcpy(&f,&bits,4);return f;}
static unsigned char* Original(const char* name,const char* patchName){FILE* input=fopen(name,"rb");CHECK(input!=0);unsigned char* code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code && fread(code,1,978,input)==978 && fgetc(input)==EOF && !fclose(input));input=fopen(patchName,"rb");unsigned count;CHECK(input && fread(&count,4,1,input)==1);for(unsigned i=0;i<count;++i){unsigned patch[3];CHECK(fread(patch,4,3,input)==3 && patch[0]<=974 && !patch[2]);*(unsigned*)(code+patch[0])=patch[1];}CHECK(fgetc(input)==EOF && !fclose(input));return code;}
int main(int argc,char** argv){CHECK(argc==1 || argc==3);unsigned char* code=argc==3?Original(argv[1],argv[2]):0;typedef void(CShaderRenderManager::*Method)();Method original;CHECK(sizeof(original)==4);memcpy(&original,&code,4);
 deviceTable[94]=(void*)&Capture;const unsigned savedControl=_controlfp(0,0);const unsigned precisions[3]={_PC_24,_PC_53,_PC_64};
 __declspec(align(16)) static unsigned char storage[2][(sizeof(CShaderRenderManager)+32+15)&~15];unsigned char layoutStorage[sizeof(CVertexShaderConstantLayout)];
 for(unsigned matrices=0;matrices<1024;++matrices){
  __declspec(align(16)) float source[3][4][4];const unsigned kind=matrices<384?0:matrices<512?5:matrices<768?1:matrices%5;
  for(unsigned m=0;m<3;++m)for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c)source[m][r][c]=Value(kind==0?0:((Next()%7)==0?(Next()%5):kind));
  for(unsigned simd=0;simd<2;++simd)for(unsigned precision=0;precision<3;++precision){
   memset(layoutStorage,0xcc,sizeof(layoutStorage));CVertexShaderConstantLayout& layout=*(CVertexShaderConstantLayout*)layoutStorage;layout.CombinedProjectionMatrix.Offset=(long)(row%96);
   memset(calls,0,sizeof(calls));unsigned status[2],control[2];
   // One initial image (guard bytes around the manager included) for both modes.
   __declspec(align(16)) static unsigned char initial[(sizeof(CShaderRenderManager)+32+15)&~15];memset(initial,0xa5,sizeof(initial));
   {CShaderRenderManager& manager=*(CShaderRenderManager*)(initial+16);manager.CurrentVSConstantLayout=&layout;manager.D3DDevice=(IDirect3DDevice9*)device;manager.updateFlags3D8=Next()|((row&1)?0x80u:0u);
    memcpy(manager.WorldMatrix.Data,source[0],64);memcpy(manager.ViewMatrix.Data,source[1],64);memcpy(manager.ProjectionMatrix.Data,source[2],64);}
   for(mode=0;mode<(code?2:1);++mode){
    unsigned char* bytes=storage[mode];memcpy(bytes,initial,sizeof(initial));CShaderRenderManager& manager=*(CShaderRenderManager*)(bytes+16);
    GEnableSIMD=simd!=0;_controlfp(precisions[precision]|_RC_NEAR,_MCW_PC|_MCW_RC);_clearfp();
    if(mode)(manager.*original)();else manager.UpdateCombinedProjectionTransform();
    control[mode]=_controlfp(0,0);status[mode]=_statusfp();_controlfp(savedControl,_MCW_PC|_MCW_RC);
    Call& c=calls[mode];CHECK(c.count==1 && c.device==device && c.start==layout.CombinedProjectionMatrix.Offset && c.data==&manager.CombinedProjectionMatrix && c.vectors==4 && !memcmp(c.values,manager.CombinedProjectionMatrix.Data,64) && !(manager.updateFlags3D8&0x80));
   }
   // Only the combined matrix and bit 0x80 of the flags may change; guards and every other byte stay.
   {const unsigned changedFrom=16+offsetof(CShaderRenderManager,CombinedProjectionMatrix),flags=16+offsetof(CShaderRenderManager,updateFlags3D8);
    for(unsigned k=0;k<sizeof(initial);++k)if((k<changedFrom || k>=changedFrom+64) && (k<flags || k>=flags+4))CHECK(storage[0][k]==initial[k]);
    CHECK((*(unsigned*)(storage[0]+flags))==((*(unsigned*)(initial+flags))&~0x80u));}
   if(code){
    // SSE path (and every byte outside the combined matrix): bit-identical. The x87 fallback is
    // bit-identical for exactly representable inputs; otherwise VC7.1's association of the four-term
    // sums differs from retail's (whose order follows its inline-asm SSE block's register pressure), so
    // each element must agree within 16*2^-24 of |world|*|view|*|projection| plus a denormal floor.
    const bool exact=simd || kind==0;const unsigned matrixFrom=16+offsetof(CShaderRenderManager,CombinedProjectionMatrix);
    CHECK(control[0]==control[1]);if(simd)CHECK(status[0]==status[1]);
    for(unsigned k=0;k<sizeof(storage[0]);++k)if((exact || k<matrixFrom || k>=matrixFrom+64) && storage[0][k]!=storage[1][k]){fprintf(stderr,"STATE_DIFF offset=%u candidate=%02x original=%02x simd=%u precision=%u\n",k,storage[0][k],storage[1][k],simd,precision);CHECK(false);}
    if(exact)CHECK(!memcmp(calls[0].values,calls[1].values,64));
    else{double W[4][4],B[4][4];for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c){W[r][c]=0;for(unsigned q=0;q<4;++q)W[r][c]+=fabs((double)source[1][r][q])*fabs((double)source[0][q][c]);}
     for(unsigned r=0;r<4;++r)for(unsigned c=0;c<4;++c){B[r][c]=0;for(unsigned q=0;q<4;++q)B[r][c]+=fabs((double)source[2][r][q])*W[q][c];}
     const float* x=(const float*)(storage[0]+matrixFrom);const float* y=(const float*)(storage[1]+matrixFrom);
     for(unsigned e=0;e<16;++e){const double bound=B[e/4][e%4];const bool nx=x[e]!=x[e],ny=y[e]!=y[e];
      if(nx || ny || !(bound<=3.4e38)){CHECK(nx==ny);if(!nx)CHECK(x[e]==y[e] || (!(fabs(x[e])<=3.4e38f) && !(fabs(y[e])<=3.4e38f)));continue;}
      if(x[e]==y[e])continue; // equal values, equal infinities included
      const double difference=fabs((double)x[e]-(double)y[e]);if(!(difference<=bound*16.0/16777216.0+8.0*1.4e-45))fprintf(stderr,"BOUND element=%u candidate=%.9g original=%.9g bound=%.9g precision=%u\n",e,x[e],y[e],bound,precision);CHECK(difference<=bound*16.0/16777216.0+8.0*1.4e-45);if(difference>worst)worst=difference/(bound>0?bound:1);}
     ++approximate;}
   }
   ++row;
  }
 }
 printf("COMBINED_PROJECTION_PASS cases=%u paths=%u approximate=%u worst_relative=%.3g\n",row,row*(code?2:1),approximate,worst);return 0;
}
