#include <string.h>
#include "fable_gfuninitialise.h"
void CSystemManager::InitGUIDFromName(const char* name){
 char* guid=reinterpret_cast<char*>(ApplicationGUID);long length=(long)strlen(name);
 for(long i=0;i<16;++i)guid[i]=i<length?name[i]:char(0);
}