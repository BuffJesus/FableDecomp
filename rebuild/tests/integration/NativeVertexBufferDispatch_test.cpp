#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "fable_font_vertex_buffer.h"
#include "fable_gfuninitialise.h"
#include "fable_display_viewport.h"
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
#pragma comment(lib,"kernel32.lib")
static unsigned cases,paths,phase,mode,mutation,flags,offset,bytes,outputKind,status,deleteFlag;static long originalBytes;static CVertexBufferWin32* active;static unsigned getterCalls,createCalls,lockCalls,unlockCalls,releaseCalls,allocations,frees;static void* owned;static unsigned char outputA[32],outputB[32];
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"VERTEX_BUFFER_FAILURE case=%u path=%u mode=%u phase=%u line=%d\n",cases,paths,mode,phase,line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
void* operator new(size_t size){CHECK(!owned && size==sizeof(CVertexBufferWin32));unsigned char* raw=(unsigned char*)malloc(size+32);CHECK(raw);memset(raw,0xa5,size+32);owned=raw+16;++allocations;return owned;}
void operator delete(void* p){CHECK(p==owned && phase==4);unsigned char* data=(unsigned char*)p;for(unsigned i=0;i<16;++i)CHECK(data[int(i)-16]==0xa5 && data[sizeof(CVertexBufferWin32)+i]==0xa5);CHECK(!active->Buffer && releaseCalls==1);owned=0;++frees;free(data-16);}
extern "C" unsigned long Win32Vtable[],BaseVtable[];
#pragma comment(linker,"/alternatename:_Win32Vtable=??_7CVertexBufferWin32@@6B@")
#pragma comment(linker,"/alternatename:_BaseVtable=??_7CBaseClass@@6B@")
static void* comTable[14];static void* deviceTable[119];static void* objectA[1]={comTable};static void* objectB[1]={comTable};static void* deviceObject[1]={deviceTable};
__declspec(align(16)) static unsigned char systemStorage[sizeof(CSystemManager)],displayStorage[sizeof(CDisplayManager)],renderStorage[sizeof(CRenderManager)];static CSystemManager* fixtureSystem=(CSystemManager*)systemStorage;static CDisplayManager* display=(CDisplayManager*)displayStorage;static CRenderManager* render=(CRenderManager*)renderStorage;
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){CHECK(phase==1 || phase==2 || phase==4);++getterCalls;if(phase==2){CHECK(getterCalls==2);if(mutation&1)active->Buffer=(IDirect3DVertexBuffer9*)objectB;if(mutation&2)active->ByteSize=0x11223344;}return fixtureSystem;}
static HRESULT __stdcall Create(void* self,UINT size,DWORD usage,DWORD fvf,D3DPOOL pool,IDirect3DVertexBuffer9** output,HANDLE* shared){CHECK(phase==1 && getterCalls==1 && self==deviceObject && size==8400 && usage==8 && fvf==0x144 && (unsigned)pool==1 && output==&active->Buffer && !shared && !active->Buffer && active->ByteSize==0 && active->VertexType==0 && !active->VertexStride && active->ScaleX==1 && active->ScaleY==1 && active->ScaleZ==1 && active->ScaleW==1 && !active->Extra24 && !active->Extra28 && !active->Extra2c && !active->Extra30 && *(unsigned long*)active==(unsigned long)Win32Vtable);*output=(IDirect3DVertexBuffer9*)objectA;++createCalls;return (HRESULT)status;}
static HRESULT __stdcall Lock(void* self,UINT actualOffset,UINT size,void** output,DWORD actualFlags){CHECK(phase==2 && getterCalls==2 && !lockCalls && self==((mutation&1)?objectB:objectA) && actualOffset==offset && actualFlags==flags && size==(bytes?bytes:(unsigned long)originalBytes) && output && *output==0);if(outputKind==1)*output=outputA+7;if(outputKind==2)*output=outputB+19;++lockCalls;return (HRESULT)status;}
static HRESULT __stdcall Unlock(void* self){CHECK(phase==3 && self==((mutation&1)?objectB:objectA) && getterCalls==2 && lockCalls==1 && !unlockCalls);++unlockCalls;return (HRESULT)status;}
static unsigned long __stdcall Release(void* self){CHECK(phase==4 && self==((mutation&1)?objectB:objectA) && getterCalls==3 && lockCalls==1 && unlockCalls==1 && !releaseCalls);++releaseCalls;return 7;}
extern "C" unsigned char* __fastcall BaseLock(CVertexBufferBase*,void*,unsigned long,unsigned long,unsigned long);
extern "C" void __fastcall BaseUnlock(CVertexBufferBase*,void*);
extern "C" bool __fastcall BaseBusy(const CVertexBufferBase*,void*);
extern "C" unsigned long __fastcall BaseSize(const CVertexBufferBase*,void*);
extern "C" void* __fastcall Scalar(CVertexBufferWin32*,void*,unsigned);
#pragma comment(linker,"/alternatename:@BaseLock@20=?Lock@CVertexBufferBase@@QAEPAEKKK@Z")
#pragma comment(linker,"/alternatename:@BaseUnlock@8=?Unlock@CVertexBufferBase@@QAEXXZ")
#pragma comment(linker,"/alternatename:@BaseBusy@8=?IsBusy@CVertexBufferBase@@QBE_NXZ")
#pragma comment(linker,"/alternatename:@BaseSize@8=?Sizeof@CVertexBufferBase@@QBEKXZ")
#pragma comment(linker,"/alternatename:@Scalar@12=??_GCVertexBufferWin32@@UAEPAXI@Z")
typedef unsigned char*(__fastcall* LockFn)(CVertexBufferBase*,void*,unsigned long,unsigned long,unsigned long);typedef void(__fastcall* UnlockFn)(CVertexBufferBase*,void*);typedef bool(__fastcall* BusyFn)(const CVertexBufferBase*,void*);typedef unsigned long(__fastcall* SizeFn)(const CVertexBufferBase*,void*);typedef void*(__fastcall* ScalarFn)(CVertexBufferWin32*,void*,unsigned);
static unsigned char* originalCode;
static void Run(){CHECK(allocations==frees && !owned);phase=1;getterCalls=createCalls=lockCalls=unlockCalls=releaseCalls=0;void* raw=::operator new(sizeof(CVertexBufferWin32));active=(CVertexBufferWin32*)raw;new(raw) CVertexBufferWin32;CHECK(createCalls==1 && active->Buffer==(IDirect3DVertexBuffer9*)objectA && active->ByteSize==8400 && active->VertexStride==28 && active->VertexType==0x144);active->ByteSize=originalBytes;unsigned bits=0x7f812345;memcpy(&active->ScaleX,&bits,4);unsigned char expected[sizeof(CVertexBufferWin32)];memcpy(expected,active,sizeof(expected));CVertexBufferWin32* want=(CVertexBufferWin32*)expected;if(mutation&1)want->Buffer=(IDirect3DVertexBuffer9*)objectB;if(mutation&2)want->ByteSize=0x11223344;
 if(mode&2)*(unsigned long*)active=(unsigned long)(originalCode+256);*(unsigned long*)want=*(unsigned long*)active;
 LockFn lock=mode&1?(LockFn)(originalCode+0):BaseLock;UnlockFn unlock=mode&1?(UnlockFn)(originalCode+16):BaseUnlock;BusyFn busy=mode&1?(BusyFn)(originalCode+32):BaseBusy;SizeFn size=mode&1?(SizeFn)(originalCode+48):BaseSize;
 phase=2;unsigned fp=_statusfp(),control=_controlfp(0,0);unsigned char* returned=lock(active,0,flags,offset,bytes);CHECK(returned==(outputKind==0?0:(outputKind==1?outputA+7:outputB+19)) && getterCalls==2 && lockCalls==1 && !memcmp(active,expected,sizeof(expected)));CHECK(!busy(active,0) && size(active,0)==(unsigned long)want->ByteSize+56 && !memcmp(active,expected,sizeof(expected)));phase=3;unlock(active,0);CHECK(unlockCalls==1 && getterCalls==2 && !memcmp(active,expected,sizeof(expected)) && _statusfp()==fp && _controlfp(0,0)==control);
 phase=4;ScalarFn scalar=mode&1?(ScalarFn)(originalCode+208):Scalar;void* dead=scalar(active,0,deleteFlag);CHECK(dead==active && getterCalls==3 && releaseCalls==1);if(!(deleteFlag&1)){want->Buffer=0;*(unsigned long*)want=(unsigned long)BaseVtable;CHECK(!memcmp(active,expected,sizeof(expected)));::operator delete(active);}CHECK(allocations==frees && !owned);++paths;}
static void Patch(unsigned char* code,unsigned at,void* target){*(unsigned long*)(code+at)=(unsigned long)target-((unsigned long)code+at+4);}
int main(int argc,char** argv){CHECK(argc==1 || argc==3);if(argc==3){originalCode=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(originalCode);FILE* f=fopen(argv[1],"rb");CHECK(f && fread(originalCode,1,276,f)==276 && fgetc(f)==EOF && !fclose(f));f=fopen(argv[2],"rb");CHECK(f);unsigned n;CHECK(fread(&n,4,1,f)==1 && n<80);for(unsigned i=0;i<n;++i){unsigned row[3];CHECK(fread(row,4,3,f)==3 && row[0]+4<=276);*(unsigned*)(originalCode+row[0])=row[1]-(row[2]?(unsigned)(originalCode+row[0]+4):0);}CHECK(fgetc(f)==EOF && !fclose(f));unsigned table[]={208,64,160,176,192};for(unsigned j=0;j<5;++j)*(unsigned*)(originalCode+256+4*j)=(unsigned)(originalCode+table[j]);}
 memset(systemStorage,0,sizeof(systemStorage));memset(displayStorage,0,sizeof(displayStorage));fixtureSystem->PDisplayManager.Data=display;display->D3DDevice=(IDirect3DDevice9*)deviceObject;memcpy(&display->PRenderManager,&render,4);deviceTable[26]=(void*)&Create;comTable[2]=(void*)&Release;comTable[11]=(void*)&Lock;comTable[12]=(void*)&Unlock;memset(outputA,0x3c,sizeof(outputA));memset(outputB,0x69,sizeof(outputB));
 const unsigned byteSizes[]={0,1,8400,0x7fffffff,0x80000000,0xffffffff},lengths[]={0,3,0xffffffff},offsets[]={0,17,0xffffffff},flagValues[]={0,8,0xffffffff},deleting[]={0,1,2,3,0x80,0x81};
 for(unsigned b=0;b<6;++b)for(unsigned l=0;l<3;++l)for(unsigned o=0;o<3;++o)for(unsigned f=0;f<3;++f)for(outputKind=0;outputKind<3;++outputKind)for(unsigned hr=0;hr<2;++hr)for(mutation=0;mutation<4;++mutation)for(unsigned d=0;d<6;++d){originalBytes=(long)byteSizes[b];bytes=lengths[l];offset=offsets[o];flags=flagValues[f];status=hr?0x80004005:0;deleteFlag=deleting[d];for(mode=0;mode<(originalCode?4U:1U);++mode)Run();++cases;}
 CHECK(cases==23328 && paths==cases*(originalCode?4:1) && allocations==frees);if(originalCode)CHECK(VirtualFree(originalCode,0,0x8000));printf("VERTEX_BUFFER_DISPATCH_PASS cases=%u variants=%u paths=%u allocations=%u frees=%u\n",cases,originalCode?4:1,paths,allocations,frees);return 0;}
