#include <cstdio>
static long g_data; static char* g_name; static long g_default_seen; static void* g_self;
struct CPersistContext { void Inner(char*,long*,long*); void Transfer_long_(char*,long*); };
void CPersistContext::Inner(char* pName,long* pData,long* pDefault){ g_self=(void*)this; g_name=pName; g_data=pData?*pData:-1; g_default_seen=*pDefault; }
void CPersistContext::Transfer_long_(char* pName,long* pData){ long dflt=0; Inner(pName,pData,&dflt); }
int main(){ long val=12345; char nm[]="field"; CPersistContext ctx; ctx.Transfer_long_(nm,&val); int ok=(g_self==(void*)&ctx)&&(g_data==12345)&&(g_name==nm)&&(g_default_seen==0); if(ok) printf("PARITY_OK\n"); else printf("FAIL\n"); return 0; }