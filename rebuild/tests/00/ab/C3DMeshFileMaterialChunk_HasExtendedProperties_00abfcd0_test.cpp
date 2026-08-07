#include "rebuild_abi.h"
#include <cstdio>
const char kExtPropName_abfcd0[] = "ext";
int FABLE_STDCALL FableHasExtHelper_abfcd0(const char *name){ return name[0]=='x' ? 1 : 0; }
struct C3DMeshFileMaterialChunk { bool HasExtendedProperties(); };
bool C3DMeshFileMaterialChunk::HasExtendedProperties(){ return (unsigned int)FableHasExtHelper_abfcd0(kExtPropName_abfcd0)>0; }
int main(){
    C3DMeshFileMaterialChunk c;
    if(c.HasExtendedProperties()){std::printf("BAD\n");return 1;}
    std::printf("HASEXT_OK\n"); return 0;
}