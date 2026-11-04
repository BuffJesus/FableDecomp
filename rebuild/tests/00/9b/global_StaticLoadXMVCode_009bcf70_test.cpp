#include <stdio.h>
#include "engine/CCodeSectionManager.h"
CCountedPointer<CCodeSectionManager> g_FableCodeSectionManager_013CAA68;
unsigned long g_FableXMVCodeHandle_013CA7E0=77;
unsigned long g_FableXMVCodeReferences_013CA7E4=0;
static int constructions,resets,loads,bad;static unsigned long seenHandle;
CCodeSectionManager::CCodeSectionManager():SectionLoadingEnabled(false){++constructions;}
static void FreeHeaders(CCodeSectionManager& m){
 free(m.ShortNameToHandle._M_t._M_header._M_data);
 free(m.LongNameToHandle._M_t._M_header._M_data);
 free(m.SectionDesc._M_t._M_header._M_data);
}
CCodeSectionManager::~CCodeSectionManager(){FreeHeaders(*this);}
template<> void CCountedPointer<CCodeSectionManager>::Reset(CCodeSectionManager* value){++resets;Data=value;Info=0;}
void CCodeSectionManager::LoadSection(unsigned long handle){++loads;seenHandle=handle;if(this!=g_FableCodeSectionManager_013CAA68.Data)bad=1;}
int main(){
 StaticLoadXMVCode();if(constructions!=1||resets!=1||loads!=1||seenHandle!=77||g_FableXMVCodeReferences_013CA7E4!=1||bad)return 1;
 CCodeSectionManager* original=g_FableCodeSectionManager_013CAA68.Data;
 StaticLoadXMVCode();if(constructions!=1||loads!=1||g_FableXMVCodeReferences_013CA7E4!=2)return 2;
 g_FableXMVCodeReferences_013CA7E4=0;g_FableXMVCodeHandle_013CA7E0=0xffffffff;StaticLoadXMVCode();
 if(constructions!=1||resets!=1||loads!=2||seenHandle!=0xffffffff||g_FableCodeSectionManager_013CAA68.Data!=original||g_FableXMVCodeReferences_013CA7E4!=1)return 3;
 delete original;g_FableCodeSectionManager_013CAA68.Data=0;g_FableXMVCodeReferences_013CA7E4=8;StaticLoadXMVCode();
 if(constructions!=1||loads!=2||g_FableCodeSectionManager_013CAA68.Data||g_FableXMVCodeReferences_013CA7E4!=9)return 4;
 puts("LOAD_XMV_PASS");return 0;
}