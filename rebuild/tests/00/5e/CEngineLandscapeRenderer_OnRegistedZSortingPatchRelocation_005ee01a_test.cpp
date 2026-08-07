#include "rebuild_abi.h"
#include <cstdio>
struct CEntry { void *a; void *b; };
struct CEngineLandscapeRenderer { char pad00[0x160]; CEntry *m_table; void OnRegistedZSortingPatchRelocation(long index, void *val); };
void CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation(long index, void *val){ *(void**)((char*)&m_table[index]) = val; }
int main(){
    CEntry tbl[4]; CEngineLandscapeRenderer r; r.m_table=tbl;
    r.OnRegistedZSortingPatchRelocation(2,(void*)0x99);
    if(tbl[2].a!=(void*)0x99){std::printf("BAD\n");return 1;}
    std::printf("ZSORT_OK\n"); return 0;
}