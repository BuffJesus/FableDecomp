#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_system.h"
static int strings;static void* stringThis;
CWideString::CWideString(){++strings;stringThis=this;*(void**)this=0;}
CWideString::~CWideString(){}
#define CHECK(x) do{if(!(x)){printf("FAIL %d\n",__LINE__);return 1;}}while(0)
int main(){union{double align;unsigned char bytes[0x44];}u;memset(u.bytes,0xa5,sizeof(u.bytes));CDisplayManagerInit* p=new(u.bytes+4)CDisplayManagerInit;
CHECK(sizeof(*p)==0x3c&&strings==1&&stringThis==&p->ShaderResourcePath);CHECK(p->Width==0&&p->Height==0&&p->Depth==0&&p->RefreshRate==0&&!p->WindowHandle&&p->MultisampleType==0&&p->RenderManagerInitFlags==0);
CHECK(p->UseRenderManager&&p->UseShaderRenderManager&&p->PresentationIntevalVSyncCount==1&&p->PresentImmediatelyIfMissedVSync&&p->PresentImmediate&&p->ZBufferDepth==16&&p->NoBackBuffers==1&&p->PushBufferSize==0x80000&&p->KickOffSize==0x8000);
CHECK(*(unsigned char*)&p->SkipConfigDetection==0xa5);for(int i=0;i<4;++i)CHECK(u.bytes[i]==0xa5&&u.bytes[0x40+i]==0xa5);p->~CDisplayManagerInit();puts("DISPLAY_INIT_CTOR_PASS");return 0;}