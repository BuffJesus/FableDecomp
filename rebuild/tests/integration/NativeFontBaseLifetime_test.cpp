#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_font_bank.h"
#pragma comment(lib,"kernel32.lib")
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void*,unsigned long,unsigned long,unsigned long);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void*,unsigned long,unsigned long);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned long);
static void Check(bool ok,int line){if(!ok){fprintf(stderr,"FONT_LIFETIME_FAILURE line=%d\n",line);fflush(stderr);ExitProcess(80);}}
#define CHECK(x) Check((x)!=0,__LINE__)
fable_i32 g_CCharStringInstanceCount_013BD800=0;
char g_FableEmptyNarrowString_0129AAF4[1]={0};
struct Allocation{void* pointer;unsigned size;bool array;};static Allocation blocks[64];static unsigned allocated,freed,arrayFreed;
static void* fontPointer;static bool observeDelete;static unsigned fontDeletes;static unsigned char deletedFont[16];static long deleteChars;
static void* Allocate(size_t size,bool array){unsigned char* raw=(unsigned char*)malloc(size+32);CHECK(raw);memset(raw,0xcd,size+32);unsigned index=0;while(index<64 && blocks[index].pointer)++index;CHECK(index<64);blocks[index].pointer=raw+16;blocks[index].size=(unsigned)size;blocks[index].array=array;++allocated;return raw+16;}
static void Release(void* pointer,bool array){if(!pointer)return;unsigned i=0;while(i<64 && blocks[i].pointer!=pointer)++i;CHECK(i<64 && blocks[i].array==array);unsigned char* raw=(unsigned char*)pointer;for(unsigned n=0;n<16;++n)CHECK(raw[int(n)-16]==0xcd && raw[blocks[i].size+n]==0xcd);if(observeDelete && pointer==fontPointer){CHECK(!array && blocks[i].size==16);++fontDeletes;memcpy(deletedFont,pointer,16);deleteChars=g_CCharStringInstanceCount_013BD800;}blocks[i].pointer=0;++freed;if(array)++arrayFreed;free(raw-16);}
void* operator new(size_t size){return Allocate(size,false);}void operator delete(void* pointer){Release(pointer,false);}
void* operator new[](size_t size){return Allocate(size,true);}void operator delete[](void* pointer){Release(pointer,true);}
class GuardFont:public CFontBank {
public:
 GuardFont(){}
 virtual C2DExtentsF GetStringExtents(const wchar_t*,bool)const{CHECK(false);return C2DExtentsF();}
 virtual float GetFontMaxHeight()const{CHECK(false);return 0;}
 virtual void PrepareToRender(bool)const{CHECK(false);}
 virtual void Render(const CWideString&,const C3DVector&,float,const CRGBColour&,unsigned long,const CTexture*)const{CHECK(false);}
};
extern "C" void* __fastcall FontScalarCandidate(CFontBank*,void*,unsigned);
extern "C" void __fastcall FontNameDtor(CCharString*,void*);
extern "C" void __fastcall FontBaseDtor(CBaseClassNonCopyable*,void*);
extern "C" unsigned long FontVtable[];
extern "C" unsigned long RootBaseVtable[];
#pragma comment(linker,"/alternatename:_RootBaseVtable=??_7CBaseClass@@6B@")
#pragma comment(linker,"/alternatename:@FontScalarCandidate@12=??_GCFontBank@@UAEPAXI@Z")
#pragma comment(linker,"/alternatename:@FontNameDtor@8=??1CCharString@@QAE@XZ")
#pragma comment(linker,"/alternatename:@FontBaseDtor@8=??1CBaseClassNonCopyable@@UAE@XZ")
#pragma comment(linker,"/alternatename:_FontVtable=??_7CFontBank@@6B@")
typedef void* (__fastcall* Scalar)(CFontBank*,void*,unsigned);
struct Result {unsigned char bytes[16];unsigned deletes;long chars;unsigned allocations,frees,arrayFrees;};
static Result Run(Scalar scalar,unsigned kind,unsigned flags,unsigned pattern){
 CHECK(g_CCharStringInstanceCount_013BD800==0 && allocated==freed);unsigned allocBefore=allocated,freeBefore=freed,arrayBefore=arrayFreed;Result result;memset(&result,0,sizeof(result));
 {CCharString share(kind==2?"shared-name":"");GuardFont* font=new GuardFont;CHECK(sizeof(*font)==16 && font->FontHeight==0xcdcdcdcd && (unsigned long)font->PManager==0xcdcdcdcd && !font->Name.GetLength() && g_CCharStringInstanceCount_013BD800==2);
 font->FontHeight=pattern*0x01010101;
 if(kind==1){CCharString name("exclusive-name");font->Name=name;}else if(kind==2)font->Name=share;
 CCharStringData* nameData=*reinterpret_cast<CCharStringData**>(&font->Name);if(kind)CHECK(nameData && nameData->owners==(kind==2?2:1));else CHECK(!nameData);
 CHECK(g_CCharStringInstanceCount_013BD800==2);fontPointer=font;observeDelete=true;fontDeletes=0;deleteChars=-1;memset(deletedFont,0,sizeof(deletedFont));void* returned=scalar(font,0,flags);CHECK(returned==font && g_CCharStringInstanceCount_013BD800==1);
 result.deletes=fontDeletes;result.chars=g_CCharStringInstanceCount_013BD800;
 if(flags&1){CHECK(fontDeletes==1 && deleteChars==1);memcpy(result.bytes,deletedFont,16);}else{CHECK(!fontDeletes);memcpy(result.bytes,font,16);}
 unsigned long* words=(unsigned long*)result.bytes;CHECK(words[0]==(unsigned long)RootBaseVtable);CHECK(words[1]==pattern*0x01010101 && !words[2] && words[3]==0xcdcdcdcd);
 if(kind==2){CHECK(share.GetLength()==11 && !strcmp((const char*)share,"shared-name") && nameData->owners==1);}
 observeDelete=false;if(!(flags&1))::operator delete(font);fontPointer=0;
 }
 CHECK(!g_CCharStringInstanceCount_013BD800 && allocated==freed);result.allocations=allocated-allocBefore;result.frees=freed-freeBefore;result.arrayFrees=arrayFreed-arrayBefore;return result;
}
static void Relative(unsigned char* code,unsigned site,void* target){*(unsigned long*)(code+site)=(unsigned long)target-((unsigned long)code+site+4);}
int main(int argc,char** argv){
 CHECK(argc==1 || argc==2);unsigned char* code=0;Scalar original=0;
 if(argc==2){FILE* file=fopen(argv[1],"rb");unsigned char bytes[46];CHECK(file && fread(bytes,1,46,file)==46 && fgetc(file)==EOF);CHECK(!fclose(file));code=(unsigned char*)VirtualAlloc(0,4096,0x3000,0x40);CHECK(code);memcpy(code,bytes,46);*(unsigned long*)(code+8)=(unsigned long)FontVtable;Relative(code,0xd,(void*)FontNameDtor);Relative(code,0x14,(void*)FontBaseDtor);typedef void(__cdecl* Delete)(void*);Delete release=&::operator delete;Relative(code,0x21,(void*)release);original=reinterpret_cast<Scalar>(code);}
 unsigned flags[]={0,1,0x80,0x81},cases=0,originalCases=0;
 for(unsigned pattern=0;pattern<32;++pattern)for(unsigned kind=0;kind<3;++kind)for(unsigned f=0;f<4;++f){
  Result candidate=Run(FontScalarCandidate,kind,flags[f],pattern);
  if(original){Result native=Run(original,kind,flags[f],pattern);CHECK(!memcmp(&native,&candidate,sizeof(Result)));++originalCases;}++cases;
 }
 CHECK(cases==384 && allocated==freed && !g_CCharStringInstanceCount_013BD800);if(code)CHECK(VirtualFree(code,0,0x8000));
 printf("NATIVE_FONT_BASE_LIFETIME_PASS cases=%u original=%u paths=%u empty=128 exclusive=128 shared=128 delete_flags=4 allocations=%u frees=%u\n",cases,originalCases,cases+originalCases,allocated,freed);return 0;
}
