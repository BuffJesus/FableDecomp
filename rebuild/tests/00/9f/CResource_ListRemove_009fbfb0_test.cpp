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
int main(){Raw<CResource> ar,br,cr;CResource& a=ar.get();CResource& b=br.get();CResource& c=cr.get();
 a.PrevResource=&b;a.NextResource=0;CResourceTestAccess::Remove(a);if(a.PrevResource!=&b||a.NextResource)return 1;
 a.NextResource=&c;b.NextResource=&a;c.PrevResource=&a;a.ResourceSize=73;a.LastUsedFrame=11;
 CResourceTestAccess::Remove(a);if(a.PrevResource||a.NextResource||b.NextResource!=&c||c.PrevResource!=&b||a.ResourceSize!=73||a.LastUsedFrame!=11)return 2;
 a.PrevResource=&b;a.NextResource=&c;b.NextResource=&c;c.PrevResource=&a;
 CResourceTestAccess::Remove(a);if(a.PrevResource||a.NextResource||b.NextResource!=&c||c.PrevResource!=&a)return 3;
 a.PrevResource=&b;a.NextResource=&c;b.NextResource=&a;c.PrevResource=&b;
 CResourceTestAccess::Remove(a);if(a.PrevResource||a.NextResource||b.NextResource!=&a||c.PrevResource!=&b)return 4;
 a.PrevResource=a.NextResource=&b;b.PrevResource=b.NextResource=&a;
 CResourceTestAccess::Remove(a);if(a.PrevResource||a.NextResource||b.PrevResource!=&b||b.NextResource!=&b)return 5;
 puts("RESOURCE_REMOVE_PASS");return 0;}
