#include <cstdio>
#include <cstring>
extern "C" unsigned char __fastcall CUserProfileManager_VerifyAndLoadProfile_005957d9(void*,void*,void*);
enum Event{kFind=1,kVerify,kCopy,kRecord,kInsert,kDestroyRecord,kDestroyCopy};
static Event g_e[8];static int g_n;static void* g_container;static void* g_sentinel;static unsigned char g_node[0x18];static void* g_find;static unsigned char g_verified;static void* g_profile;static void* g_copy;static void* g_record;static void* g_insertResult;static void* g_insertRecord;
static void A(Event e){g_e[g_n++]=e;}
extern "C" void* __fastcall FableProfileVerifyFind(void*c,void*,void*p){A(kFind);if(c==g_container&&p==g_profile)return g_find;return g_sentinel;}
extern "C" unsigned char __fastcall FableProfileVerifyName(void*p,void*){A(kVerify);return p==g_profile?g_verified:0;}
extern "C" void __fastcall FableProfileVerifyCopyWide(void*d,void*,void*p){A(kCopy);g_copy=d;*(void**)d=p;}
extern "C" void __fastcall FableProfileVerifyConstructRecord(void*d,void*,void*s){A(kRecord);g_record=d;*(void**)d=*(void**)s;}
extern "C" void __fastcall FableProfileVerifyInsert(void*c,void*,void*r,void*rec){A(kInsert);if(c==g_container){g_insertResult=r;g_insertRecord=rec;}}
extern "C" void __fastcall FableProfileVerifyDestroyWide(void*p,void*){if(p==g_record)A(kDestroyRecord);else if(p==g_copy)A(kDestroyCopy);}
static void Reset(unsigned char*m,bool cached,unsigned char result){std::memset(m,0,0xe0);std::memset(g_node,0,sizeof(g_node));g_container=m+0xc4;g_sentinel=(void*)0x12340000;*(void**)g_container=g_sentinel;g_find=cached?g_node:g_sentinel;g_node[0x14]=result;g_verified=result;g_profile=(void*)0x55667788;g_n=0;g_copy=g_record=g_insertResult=g_insertRecord=0;}
static bool Run(bool cached,unsigned char result){unsigned char m[0xe0];Reset(m,cached,result);unsigned char got=CUserProfileManager_VerifyAndLoadProfile_005957d9(m,0,g_profile);if(cached)return got==result&&g_n==1&&g_e[0]==kFind;static const Event x[]={kFind,kVerify,kCopy,kRecord,kInsert,kDestroyRecord,kDestroyCopy};return got==result&&g_n==7&&std::memcmp(g_e,x,sizeof(x))==0&&g_insertResult!=0&&g_insertRecord==g_record;}
int main(){bool a=Run(true,1),b=Run(true,0),c=Run(false,1),d=Run(false,0);if(!a||!b||!c||!d){std::printf("FSE2_005957d9_TEST FAIL %d%d%d%d\n",a,b,c,d);return 1;}std::puts("FSE2_005957d9_TEST PASS");return 0;}
