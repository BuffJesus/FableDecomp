#include "test_support/string_vector_fixture.h"
#include "engine/CCodeSectionManager.h"
static CCodeSectionManager::CSectionDesc first,second;
static int queries,bad;static const void* expected;static unsigned long expectedHandle;
CCodeSectionManager::CSectionDesc& CCodeSectionManager::GetSectionDesc(unsigned long handle){if(this!=expected||handle!=expectedHandle)bad=1;++queries;return queries==1?first:second;}
int main(){
 double raw[6];memset(raw,0,sizeof(raw));CCodeSectionManager& m=*reinterpret_cast<CCodeSectionManager*>(raw);expected=&m;
 m.LoadSection(0xffffffff);if(queries)return 1;
 expectedHandle=7;first.Loaded=true;second.Loaded=false;m.LoadSection(7);if(queries!=1||second.Loaded||bad)return 2;
 for(int enabled=0;enabled<2;++enabled){queries=0;m.SectionLoadingEnabled=enabled!=0;first.Loaded=false;second.Loaded=false;first.Size=99;second.Size=88;
  m.LoadSection(7);if(queries!=2||first.Loaded||!second.Loaded||first.Size!=99||second.Size!=88||bad)return 3;}
 puts("LOAD_SECTION_PASS");return 0;
}