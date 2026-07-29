#include <cstdio>
#include <cstring>
extern "C" void __fastcall CFrontEndManager_GetScoreboardOptionsToProfile_00596550(void*,void*);
extern "C" const char FableScoreboardCommitLogin[]="UI_SCOREBOARD_LOGIN_EDITBOX_FE";
extern "C" const char FableScoreboardCommitPassword[]="UI_SCOREBOARD_PASSWORD_EDITBOX_FE";
extern "C" const char FableScoreboardCommitProxyIp[]="UI_SCOREBOARD_PROXY_IP_EDITBOX_FE";
extern "C" const char FableScoreboardCommitProxyPort[]="UI_SCOREBOARD_PROXY_PORT_EDITBOX_FE";
extern "C" const char FableScoreboardCommitServerPort[]="UI_SCOREBOARD_SERVER_PORT_EDITBOX_FE";
struct Probe{const void* value;};
static unsigned char g_service[0x124],g_manager[0x80],g_current,g_other;
static void* g_map; static void* g_top; static unsigned long g_key;
static int g_resolves,g_tops,g_constructs,g_gets,g_assigns,g_dw,g_dn,g_converts,g_parses,g_services;
static unsigned long g_values[5]; static const char* g_names[5];
extern "C" void** __fastcall FableScoreboardCommitResolve(void* p,void*,const unsigned long*k){++g_resolves;if(p==g_manager+0x54)g_key=*k;return &g_map;}
extern "C" void** __fastcall FableScoreboardCommitStackTop(void* p,void*){++g_tops;if(p!=g_manager+0x20)g_top=0;return &g_top;}
extern "C" void __fastcall FableScoreboardCommitConstructName(void*n,void*,const char*t,long l){if(l==-1)++g_constructs;((Probe*)n)->value=t;}
extern "C" void* __fastcall FableScoreboardCommitGetEdit(void*m,void*,void*out,void*n){if(m==g_manager){g_names[g_gets]= (const char*)((Probe*)n)->value;((Probe*)out)->value=&g_values[g_gets++];}return out;}
extern "C" void* __cdecl FableScoreboardCommitGetService(){++g_services;return g_service;}
extern "C" void __fastcall FableScoreboardCommitAssignWide(void*d,void*,void*s){++g_assigns;((Probe*)d)->value=((Probe*)s)->value;}
extern "C" void __fastcall FableScoreboardCommitDestroyWide(void*,void*){++g_dw;}
extern "C" void __fastcall FableScoreboardCommitDestroyName(void*,void*){++g_dn;}
extern "C" void* __fastcall FableScoreboardCommitConvertWide(void*w,void*,void*out){++g_converts;((Probe*)out)->value=((Probe*)w)->value;return out;}
extern "C" long __fastcall FableScoreboardCommitParseInteger(void*c,void*){++g_parses;const void*v=((Probe*)c)->value;return v==&g_values[3]?8080:6500;}
static void Reset(bool current){std::memset(g_manager,0,sizeof(g_manager));std::memset(g_service,0,sizeof(g_service));g_map=&g_current;g_top=current?&g_current:&g_other;g_key=~0u;g_resolves=g_tops=g_constructs=g_gets=g_assigns=g_dw=g_dn=g_converts=g_parses=g_services=0;std::memset(g_names,0,sizeof(g_names));}
int main(){
 Reset(false); CFrontEndManager_GetScoreboardOptionsToProfile_00596550(g_manager,0);
 bool gated=g_resolves==1&&g_tops==1&&g_constructs==0&&g_services==0&&g_key==0x19;
 Reset(true); CFrontEndManager_GetScoreboardOptionsToProfile_00596550(g_manager,0);
 const char* exp[]={FableScoreboardCommitLogin,FableScoreboardCommitPassword,FableScoreboardCommitProxyIp,FableScoreboardCommitProxyPort,FableScoreboardCommitServerPort};
 bool names=true;for(int i=0;i<5;++i)names=names&&std::strcmp(g_names[i],exp[i])==0;
 bool fields=((Probe*)(g_service+0x110))->value==&g_values[0]&&((Probe*)(g_service+0x114))->value==&g_values[1]&&((Probe*)(g_service+0x118))->value==&g_values[2]&&*(long*)(g_service+0x11c)==8080&&*(long*)(g_service+0x120)==6500;
 bool pass=gated&&names&&fields&&g_constructs==5&&g_gets==5&&g_assigns==3&&g_dw==5&&g_dn==7&&g_converts==2&&g_parses==2&&g_services==5;
 if(!pass){std::printf("FSE2_00596550_TEST FAIL gate=%d names=%d fields=%d c=%d g=%d a=%d dw=%d dn=%d cv=%d p=%d s=%d\n",gated?1:0,names?1:0,fields?1:0,g_constructs,g_gets,g_assigns,g_dw,g_dn,g_converts,g_parses,g_services);return 1;}
 std::puts("FSE2_00596550_TEST PASS");return 0;
}
