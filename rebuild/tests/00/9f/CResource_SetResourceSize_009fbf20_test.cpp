#include "fable_resource_list.h"
#include <stdio.h>
#include <string.h>
template<class T> struct Raw {
 double bytes[(sizeof(T)+7)/8];
 Raw(){memset(bytes,0,sizeof(bytes));}
 T& get(){return *reinterpret_cast<T*>(bytes);}
};
class CResourceTestAccess {
public:
 static void Insert(CResource& r,CResource* prev,CResource* next){r.ListInsert(prev,next);}
 static void Remove(CResource& r){r.ListRemove();}
};
int main(){Raw<CResource> a;Raw<CResourceList> b;CResource& resource=a.get();CResourceList& list=b.get();
 resource.ResourceSize=16;resource.LastUsedFrame=44;list.AllocatedMemory=1000;list.ResourceCount=9;resource.ResourceList=&list;
 resource.SetResourceSize(32);if(resource.ResourceSize!=32||list.AllocatedMemory!=1016)return 1;
 resource.SetResourceSize(0);if(resource.ResourceSize||list.AllocatedMemory!=984)return 2;
 resource.SetResourceSize(0);if(list.AllocatedMemory!=984||resource.LastUsedFrame!=44||list.ResourceCount!=9)return 3;
 resource.ResourceList=0;resource.SetResourceSize(0xffffffff);if(resource.ResourceSize!=0xffffffff||list.AllocatedMemory!=984)return 4;
 puts("RESOURCE_SIZE_PASS");return 0;}