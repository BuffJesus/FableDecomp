#include <cstdio>
#include <cstring>
extern "C" unsigned char __fastcall CUserProfileManager_ValidateSaveFile_0047ed0a(void*,void*,void*);
enum E{kFind=1,kVerify,kCopy,kKey,kInsert,kDestroyKey,kDestroyCopy};
static E g_e[7];static int g_n;static void*g_cache,*g_sentinel,*g_found,*g_path,*g_copy,*g_key,*g_insertKey;static unsigned char g_node[0x18],g_result;
static void A(E e){g_e[g_n++]=e;}
extern "C" void* __fastcall FableValidateSaveFind(void*c,void*,void*p){A(kFind);return c==g_cache&&p==g_path?g_found:g_sentinel;}
extern "C" unsigned char __fastcall FableValidateSaveSignature(void*p,void*){A(kVerify);return p==g_path?g_result:0;}
extern "C" void __fastcall FableValidateSaveCopyWide(void*d,void*,void*p){A(kCopy);g_copy=d;*(void**)d=p;}
extern "C" void __fastcall FableValidateSaveConstructKey(void*d,void*,void*s){A(kKey);g_key=d;*(void**)d=*(void**)s;}
extern "C" void __fastcall FableValidateSaveInsert(void*c,void*,void*,void*k){A(kInsert);if(c==g_cache)g_insertKey=k;}
extern "C" void __fastcall FableValidateSaveDestroyWide(void*p,void*){A(p==g_key?kDestroyKey:kDestroyCopy);}
static bool Run(bool cached,unsigned char result){unsigned char m[0x100];std::memset(m,0,sizeof(m));std::memset(g_node,0,sizeof(g_node));g_cache=m+0xe0;g_sentinel=(void*)0x11110000;*(void**)g_cache=g_sentinel;g_found=cached?g_node:g_sentinel;g_node[0x14]=result;g_result=result;g_path=(void*)0x22223333;g_copy=g_key=g_insertKey=0;g_n=0;unsigned char r=CUserProfileManager_ValidateSaveFile_0047ed0a(m,0,g_path);if(cached)return r==result&&g_n==1&&g_e[0]==kFind;static const E x[]={kFind,kVerify,kCopy,kKey,kInsert,kDestroyKey,kDestroyCopy};return r==result&&g_n==7&&std::memcmp(g_e,x,sizeof(x))==0&&g_insertKey==g_key;}
int main(){bool a=Run(true,0),b=Run(true,1),c=Run(false,0),d=Run(false,1);if(!a||!b||!c||!d){std::printf("FSE2_0047ed0a_TEST FAIL %d%d%d%d\n",a,b,c,d);return 1;}std::puts("FSE2_0047ed0a_TEST PASS");return 0;}
