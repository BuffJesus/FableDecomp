
#define private public
#include "fable_game_text_bank.h"
#undef private
#pragma comment(linker,"/alternatename:?ConvertPathToFilename@CBankFile@@QBE?AVCCharString@@ABV2@@Z=?ConvertPathToFilename@CBankFile@@ABE?AVCCharString@@ABV2@@Z")
#include "fable_filesystem.h"
#include <stdlib.h>
#include <stdio.h>
#include <new>
#include <string.h>
fable_i32 g_CCharStringInstanceCount_013BD800=0;
fable_i32 g_CWideStringInstanceCount_013BCA20=0;
char g_FableEmptyNarrowString_0129AAF4[1]={0};
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
// Cold siblings share these translation units; fail closed if reached.
CCharString::CCharString(){exit(90);}
const CCharString& CCharString::operator=(const char*){exit(90);return *this;}
CCharString& CCharString::operator=(const CCharString&){exit(90);return *this;}
CCharString CCharString::GetSubString(unsigned long,unsigned long)const{exit(90);return *this;}
long CCharString::FindCharPos(char)const{exit(90);return 0;}
CWideString::CWideString(const wchar_t*){exit(90);}
CWideString __fastcall CAFile::ExpandPath(const CWideString& a,const CWideString&){exit(90);return a;}
CWideString __fastcall CAFile::GetCurrentPath(){exit(90);return CWideString();}
CWideString __fastcall operator+(const CWideString& a,const CWideString&){exit(90);return a;}
bool __fastcall CAFile::PathExists(const CWideString&){exit(90);return false;}

static void require(bool ok){if(!ok){fprintf(stderr,"FILENAME_NATIVE_FAILURE\n");exit(80);}}
struct Block {unsigned char* p;size_t size;int kind;bool freed;};
static Block blocks[128];static unsigned used;
static void* allocate(size_t size,int kind){require(used<128&&size<1024);unsigned char* p=(unsigned char*)malloc(size+32);require(p!=0);memset(p,0x7d,size+32);Block& b=blocks[used++];b.p=p+16;b.size=size;b.kind=kind;b.freed=false;return b.p;}
static void dispose(void* p,int kind){if(!p)return;Block* b=0;for(unsigned i=0;i<used;++i)if(blocks[i].p==p&&!blocks[i].freed)b=&blocks[i];require(b&&b->kind==kind);for(int i=0;i<16;++i)require(b->p[i-16]==0x7d&&b->p[b->size+i]==0x7d);b->freed=true;free(b->p-16);}
extern "C" void* __cdecl PathMalloc(size_t n){return allocate(n,0);}
extern "C" void __cdecl PathFree(void* p){dispose(p,0);}
void* __cdecl operator new(size_t n){return allocate(n,1);}
void* __cdecl operator new[](size_t n){return allocate(n,2);}
void __cdecl operator delete(void* p){dispose(p,1);}
void __cdecl operator delete[](void* p){dispose(p,2);}
static void one(const char* path,const char* expected){
 require(g_CCharStringInstanceCount_013BD800==0&&g_CWideStringInstanceCount_013BCA20==0);used=0;
 {CCharString input(path);CCharString output=((CBankFile*)0x1234)->ConvertPathToFilename(input);
  CCharStringData* data=*(CCharStringData**)&output;const char* actual=data?data->Data.PData:"";
  require(strcmp(actual,expected)==0);data=*(CCharStringData**)&input;require(strcmp(data?data->Data.PData:"",path?path:"")==0);
  require(g_CWideStringInstanceCount_013BCA20==0&&g_CCharStringInstanceCount_013BD800==2);
 }
 require(g_CCharStringInstanceCount_013BD800==0&&g_CWideStringInstanceCount_013BCA20==0);
 for(unsigned i=0;i<used;++i)require(blocks[i].freed);
}
int main(){
 const char* dirs[]={"","C:\\","D:","folder\\","/root/","\\\\server\\share\\"};
 const char* names[]={"a","Mixed","two.parts",".hidden","","\x80\xff","has space","x"};
 const char* exts[]={"",".BIG",".wad",".",".a.b"};int cases=0;
 for(int d=0;d<6;++d)for(int n=0;n<8;++n)for(int e=0;e<5;++e){char path[256],expected[256];sprintf(expected,"%s%s",names[n],exts[e]);sprintf(path,"%s%s",dirs[d],expected);one(path,expected);++cases;}
 one(0,"");++cases;
 printf("FILENAME_NATIVE_PASS cases=%d\n",cases);return 0;
}
