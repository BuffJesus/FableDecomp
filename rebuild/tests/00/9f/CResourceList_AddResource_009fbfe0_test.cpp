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
int main(){Raw<CResourceList> lr;Raw<CResource> ar,br;CResourceList& list=lr.get();CResource& a=ar.get();CResource& b=br.get();
 list.Head.PrevResource=list.Head.NextResource=&list.Head;list.CurrentFrame=77;list.MaximumMemory=100;a.ResourceSize=7;b.ResourceSize=11;
 list.AddResource(a);
 if(list.ResourceCount!=1||list.AllocatedMemory!=7||a.ResourceList!=&list||a.LastUsedFrame!=77||a.NextResource!=&list.Head||a.PrevResource!=&list.Head||list.Head.NextResource!=&a||list.Head.PrevResource!=&a)return 1;
 list.CurrentFrame=78;list.AddResource(b);
 if(list.ResourceCount!=2||list.AllocatedMemory!=18||b.ResourceList!=&list||b.LastUsedFrame!=78||a.LastUsedFrame!=77||b.PrevResource!=&a||b.NextResource!=&list.Head||a.NextResource!=&b||list.Head.NextResource!=&a||list.Head.PrevResource!=&b||list.MaximumMemory!=100)return 2;
 puts("RESOURCE_ADD_PASS");return 0;}
