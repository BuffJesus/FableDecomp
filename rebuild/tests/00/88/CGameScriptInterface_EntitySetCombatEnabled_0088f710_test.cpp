#include <cstdio>
#include <cstring>
struct Thing { int marker; };
static const char* seen[4]; static const Thing* owners[4]; static bool flags[4]; static int calls; static int live;
static void Dispatch(const Thing& t,const char* n,bool e){ if(calls<4){seen[calls]=n;owners[calls]=&t;flags[calls]=e;} ++calls; }
static void Run(const Thing& t,bool e){ const char* n[4]={"SG_SEEK_ENEMY","SG_MELEE_COMBAT_NEARBY","SG_MELEE_COMBAT_NEARBY2","SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE"}; for(int i=0;i<4;++i){++live;Dispatch(t,n[i],e);--live;} }
int main(){const char* x[4]={"SG_SEEK_ENEMY","SG_MELEE_COMBAT_NEARBY","SG_MELEE_COMBAT_NEARBY2","SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE"};Thing t={1234};Run(t,true);if(calls!=4||live)return 1;for(int i=0;i<4;++i)if(std::strcmp(seen[i],x[i])||owners[i]!=&t||!flags[i])return 2;std::printf("ENTITY_SET_COMBAT_ENABLED_PASS\n");return 0;}