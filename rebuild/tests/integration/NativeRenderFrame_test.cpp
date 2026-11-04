#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_display_viewport.h"
#include "fable_render_state.h"
static unsigned long coreStorage[0x3d20/4],displayStorage[0x2c8/4],deviceTables[2][101],devices[2];
static unsigned char expected[0x3d20];
static CRenderManagerCore* core=(CRenderManagerCore*)coreStorage;
static CDisplayManager* display=(CDisplayManager*)displayStorage;
static long beginResult,endResult;
static unsigned mutation,phase,timeCalls,streamCalls,beginCalls,endCalls;
static double endTime;
static void requireAt(bool ok,unsigned line){if(!ok){fprintf(stderr,"render failure line=%u phase=%u mutation=%u begin=%ld end=%ld time=%.3f\n",line,phase,mutation,beginResult,endResult,endTime);for(unsigned i=0;i<sizeof(expected);++i)if(((unsigned char*)coreStorage)[i]!=expected[i]){fprintf(stderr,"first difference +%x actual=%u expected=%u\n",i,((unsigned char*)coreStorage)[i],expected[i]);break;}fflush(stderr);abort();}}
#define require(value) requireAt((value),__LINE__)
static unsigned long& word(unsigned char* data,unsigned offset){return *(unsigned long*)(data+offset);}
double FABLE_FASTCALL GFGetTime(){
    ++timeCalls;
    if(phase==0)return 1.0;
    if(mutation){core->LastRenderEndTime=2;core->NoTrisDrawn=12;word((unsigned char*)displayStorage,0x58)=(unsigned)&devices[1];}
    return endTime;
}
static long __stdcall Begin(void* self){require(self==&devices[0]&&phase==0);++beginCalls;return beginResult;}
static long __stdcall End(void* self){require(self==&devices[(mutation&&timeCalls)?1:0]&&phase==1);++endCalls;return endResult;}
static long __stdcall Stream(void* self,unsigned stream,void* buffer,unsigned offset,unsigned stride){
    require(self==&devices[0]&&phase==0&&!stream&&!buffer&&!offset&&!stride);++streamCalls;
    require(core->CurrentVertexFormat==0&&core->PSelectedIndexBuffer==0);
    if(mutation){core->NoZSortedPolys=31;core->NoNonZSortedPolys=32;core->NoPolys=33;}
    return -1; // Retail deliberately continues after this device result.
}
int main(){
    setbuf(stderr,0);
    const long results[]={0,1,(long)0x80004005};const double times[]={0,1,1.25,3,-1};unsigned cases=0;
    deviceTables[0][41]=(unsigned)&Begin;deviceTables[0][42]=deviceTables[1][42]=(unsigned)&End;
    deviceTables[0][100]=(unsigned)&Stream;devices[0]=(unsigned)deviceTables[0];devices[1]=(unsigned)deviceTables[1];
    for(unsigned bi=0;bi!=3;++bi)for(unsigned ei=0;ei!=3;++ei)for(unsigned rendering=0;rendering!=2;++rendering)
    for(unsigned timing=0;timing!=5;++timing)for(mutation=0;mutation!=2;++mutation){
        memset(coreStorage,0xa5,sizeof(coreStorage));memset(displayStorage,0,sizeof(displayStorage));
        core->PD3DDevice=(IDirect3DDevice9*)&devices[0];core->Rendering=rendering!=0;
        core->LastRenderEndTime=1;core->NoZSortedPolys=7;core->NoNonZSortedPolys=8;core->NoPolys=9;
        word((unsigned char*)displayStorage,8)=(unsigned)core;display->D3DDevice=(IDirect3DDevice9*)&devices[0];
        display->CanBeginRender=rendering!=0;display->unknown215_[0]=1;
        memcpy(expected,coreStorage,sizeof(expected));beginResult=results[bi];endResult=results[ei];endTime=times[timing];
        phase=timeCalls=streamCalls=beginCalls=endCalls=0;
        bool started=beginResult>=0&&!rendering;
        if(started){
            *(float*)(expected+0x3a40)=1;word(expected,0x3cf8)=0;word(expected,0x3cf4)=0;
            word(expected,0x3cdc)=mutation?31:7;word(expected,0x3ce0)=mutation?32:8;word(expected,0x3ce4)=mutation?33:9;
            unsigned offsets[]={0x3cbc,0x3cc0,0x3cc8,0x3cd0,0x3cd4,0x3cd8};for(unsigned i=0;i!=6;++i)word(expected,offsets[i])=0;
            expected[0x3a54]=1;
        }
        bool result=display->BeginRender();
        require(result==(beginResult>=0)&&beginCalls==1&&streamCalls==(unsigned)started&&timeCalls==(unsigned)started);
        require(!memcmp(coreStorage,expected,sizeof(expected)));require(display->CanBeginRender==(beginResult>=0||rendering));
        core->NoTrisDrawn=9;word(expected,0x3cd0)=9;phase=1;timeCalls=0;
        bool finishing=started||rendering;
        if(finishing){
            if(mutation){*(float*)(expected+0x3a40)=2;word(expected,0x3cd0)=12;}
            double elapsed=endTime-(mutation?2:1);double rate=(mutation?12:9)/elapsed;
            long count=elapsed==0?0:(long)rate;
            word(expected,0x3a44)=(unsigned long)count;word(expected,0x3cc4)=0;expected[0x3a54]=0;
        }
        display->EndRender();require(endCalls==1&&timeCalls==(unsigned)finishing);
        require(!memcmp(coreStorage,expected,sizeof(expected)));
        require(display->CanBeginRender==(endResult>=0?false:(beginResult>=0||rendering)));
        require(display->unknown215_[0]==(endResult>=0?0:1));++cases;
    }
    printf("NATIVE_RENDER_FRAME_PASS cases=%u\n",cases);return 0;
}
