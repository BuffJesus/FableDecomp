#include <cstdio>
struct CQuestMgr { void Activate(const void* q, int a, int b); };
CQuestMgr* g_questMgr;
static const void* seen_q; static int seen_a, seen_b; static int seen_this;
void CQuestMgr::Activate(const void* q, int a, int b){ seen_this=(this==g_questMgr); seen_q=q; seen_a=a; seen_b=b; }
void __stdcall CGameScriptInterface_ActivateMultipleQuests(const void* quests);
int main(){
    static CQuestMgr mgr; g_questMgr=&mgr;
    int arr[3]={7,8,9};
    CGameScriptInterface_ActivateMultipleQuests(arr);
    if(seen_this && seen_q==(const void*)arr && seen_a==1 && seen_b==1) printf("OK_00892ea0\n");
    else printf("FAIL\n");
    return 0;
}